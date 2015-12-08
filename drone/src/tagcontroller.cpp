#include "tagcontroller.h"
#include "lcontrol/control.h"
#include "navdatacontroller.h"
#include "gamesystem.h"
#include "roundmanager.h"

#include <thread>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>

enum DetectionMode {
    Mode_Deprecated1 = 0,
    Mode_Deprecated2,
    //!< 2D horizontal tags on drone shells
    Mode_F_HShells,
    Mode_None,
    //!< Roundel under the drone
    Mode_U_Cocarde,
    //!< Oriented roundel under the drone
    Mode_U_OrientedCocarde,
    //!< Uniform stripe on the ground
    Mode_U_Stripe,
    //!< Roundel in front of the drone
    Mode_F_Cocarde,
    //!< Oriented roundel in front of the drone
    Mode_F_OrientedCocarde,
    Mode_F_Stripe,
    Mode_Multiple,
    Mode_F_Cap,
    Mode_OrientedCocardeBW,
    Mode_F_HShellsV2,
    Mode_F_TowerSide
};

enum Tag {
    Tag_None = 0,
    Tag_ShellTag,
    Tag_Roundel,
    Tag_OrientedRoundel,
    Tag_Stripe,
    Tag_Cap,
    Tag_ShellsV2,
    Tag_TowerSize,
    Tag_RoundelBW
};

using namespace lcontrol;

TagController::TagController(GameSystem& system)
        : GameElement(system)
        , m_navctrl(system.navdataController())
        , m_log("tag_samples.txt") {
}

TagController::~TagController() {
}

void TagController::gameInit() {
    while(!m_navctrl.inited())
        ;

    Control::config("control:flight_without_shell", "FALSE");
    M_clearAck();

    Control::config("control:outdoor", "FALSE");
    M_clearAck();

    Control::config("detect:detect_type", std::to_string(Mode_F_HShellsV2)); // Mode_Multiple));
    M_clearAck();

    Control::config("detect:detections_select_h", std::to_string((0x01 << (Tag_ShellsV2 - 1))));
    M_clearAck();

    Control::config("detect:enemy_colors", "3");
    M_clearAck();

    Control::config("detect:enemy_without_shell", "0");
    M_clearAck();

    m_avg_update = 0.25f;
    m_avg_corr_update = 0.25f;
    m_tag_x = 0.0f;
    m_tag_y = 0.0f;
    m_avg_vx = 0.0f;
    m_avg_vy = 0.0f;
    m_avg_cor_vx = 0.0f;
    m_avg_cor_vy = 0.0f;

    m_delayCounter = 0;
    m_delay = 15;

    M_initMoveDetection();
}

bool TagController::hasDetection() const {
    return m_has_detection;
}

float TagController::tagPositionX() const {
    return m_tag_x;
}

float TagController::tagPositionY() const {
    return m_tag_y;
}

float TagController::tagSpeedX() const {
    return m_avg_cor_vx;
}

float TagController::tagSpeedY() const {
    return m_avg_cor_vy;
}

void TagController::gameLoop() {
    Navdata nav = m_system.navdataController().grab();
    m_has_detection = nav.vision_detect.nb_detected != 0;

    if(m_has_detection) {
        // Get detection results (that's all we have..)
        float x = nav.vision_detect.xc[0];
        float y = nav.vision_detect.yc[0];
        float d = nav.vision_detect.dist[0];

        // Convert tag position into centimeters (see docs.d/tag_calibration.md
        //   for explanations on this black magic)
        x = 1e-3 * (1.158 * d * (x - 500.0f));
        y = 1e-3 * (0.658 * d * (y - 500.0f));

        // Compute average speeds (in mm/s)
        m_avg_vx = (1.0f - m_avg_update) * m_avg_vx + m_avg_update * (10.0 * (x - m_tag_x));
        m_avg_vy = (1.0f - m_avg_update) * m_avg_vy + m_avg_update * (10.0 * (y - m_tag_y));

        // Compute average corrected speeds (in mm/s)
        m_avg_cor_vx = (1.0f - m_avg_corr_update) * m_avg_cor_vx + m_avg_corr_update * (m_avg_vx - nav.demo.vx / 10.0f);
        m_avg_cor_vy = (1.0f - m_avg_corr_update) * m_avg_cor_vy + m_avg_corr_update * (m_avg_vy - nav.demo.vz / 10.0f);

        // Update internal state
        m_tag_x = x;
        m_tag_y = y;

        // Run move detections
        int h = M_moveDetection(m_horiz_detect, m_avg_vx);
        int v = M_moveDetection(m_vert_detect, m_avg_vy);

        if(++m_delayCounter > m_delay) {
            if(h < 0)
                m_system.roundManager().userLeft();
            else if(h > 0)
                m_system.roundManager().userRight();

            if(v < 0)
                m_system.roundManager().userDown();
            else if(v > 0)
                m_system.roundManager().userUp();
        }

        if(h || v)
            m_delayCounter = 0;
    }
}

void TagController::M_clearAck() {
    // Wait for the command_ack bit
    for(int tm = 0;;) {
        // Get some navdata
        while(!m_navctrl.available())
            ;
        Navdata nav = m_navctrl.grab();

        M_trace("waiting for ack #" + std::to_string(tm));

        // Check flag
        if(nav.header.state & navdata::command_ack)
            break;

        if(++tm > 10) {
            M_error("command ack not set");
            throw std::runtime_error("TagController::M_clearAck: ack not sent !");
        }
    }
    M_trace("got command ack");

    // Clear it, and we're OK !
    Control::ackControl();
    M_trace("ack clear sent");

    // Wait for the command_ack bit to be cleared
    for(int tm = 0;;) {
        // Get some navdata
        while(!m_navctrl.available())
            ;
        Navdata nav = m_navctrl.grab();

        M_trace("waiting for cleared ack #" + std::to_string(tm));

        // Check flag
        if(!(nav.header.state & navdata::command_ack))
            break;

        if(++tm > 10) {
            M_error("command ack not cleared");
            throw std::runtime_error("TagController::M_clearAck: ack not cleared !");
        }
    }
    M_trace("command ack cleared");
}

void TagController::M_initMoveDetection() {
    m_horiz_detect.window = 20;
    m_horiz_detect.step = m_horiz_detect.window / 8;
    m_horiz_detect.amplitude = 50.0f;
    m_horiz_detect.data = std::vector<float>(m_horiz_detect.window, 0.0f);
    m_horiz_detect.reference_pattern = {0.000000,
                                        0.111111,
                                        0.222222,
                                        0.333333,
                                        0.444444,
                                        0.555556,
                                        0.666667,
                                        0.777778,
                                        0.888889,
                                        1.000000,
                                        0.888889,
                                        0.777778,
                                        0.666667,
                                        0.555556,
                                        0.444444,
                                        0.333333,
                                        0.222222,
                                        0.111111,
                                        0.000000,
                                        0.000000};
    m_horiz_detect.step_counter = 0;

    m_vert_detect.window = 20;
    m_vert_detect.step = m_vert_detect.window / 8;
    m_vert_detect.amplitude = 50.0f;
    m_vert_detect.data = std::vector<float>(m_vert_detect.window, 0.0f);
    m_vert_detect.reference_pattern = {0.000000,
                                       0.111111,
                                       0.222222,
                                       0.333333,
                                       0.444444,
                                       0.555556,
                                       0.666667,
                                       0.777778,
                                       0.888889,
                                       1.000000,
                                       0.888889,
                                       0.777778,
                                       0.666667,
                                       0.555556,
                                       0.444444,
                                       0.333333,
                                       0.222222,
                                       0.111111,
                                       0.000000,
                                       0.000000};
    m_vert_detect.step_counter = 0;
}

int TagController::M_moveDetection(TagController::DContext& context, float v) {
    // Shift data vector and set new value
    std::rotate(context.data.begin(), context.data.begin() + 1, context.data.end());
    context.data.back() = v;

    if(++context.step_counter >= context.step) {
        context.step_counter = 0;

        // Compute the cross-correlation
        std::vector<float>& corr = M_xcorr(context.data, context.reference_pattern);

        // Get maximas
        int max = M_localMax(corr, 100.0f);
        int min = M_localMin(corr, -1.0f);

        if(max >= 0 && min >= 0) {
            if(max < min) {
                return 1;
            } else {
                return -1;
            }
        }
    }

    return 0;
}

float TagController::M_xcorr(std::vector<float> const& d, std::vector<float> const& p, int i) {
    int N = d.size();
    int k = i - N;

    if(k < 0)
        return M_xcorr(d, p, 2 * N - i);

    float acc = 0.0f;
    for(int l = 0; l < N - k; ++l)
        acc += d[l + k] * p[k];

    return acc;
}

std::vector<float>& TagController::M_xcorr(std::vector<float> const& d, std::vector<float> const& p) {
    static std::vector<float> xcorr;

    if(!xcorr.size())
        xcorr = std::vector<float>(2 * d.size(), 0.0f);

    for(int i = 0; i < (int)xcorr.size(); ++i)
        xcorr[i] = M_xcorr(d, p, i);

    return xcorr;
}

int TagController::M_localMax(std::vector<float>& v, float minval) {
    float max = minval;
    int maxid = -1;
    for(int i = 0; i < (int)v.size(); ++i) {
        if(v[i] > max) {
            maxid = i;
            max = v[i];
        }
    }

    return maxid;
}

int TagController::M_localMin(std::vector<float>& v, float maxval) {
    float min = maxval;
    int minid = -1;
    for(int i = 0; i < (int)v.size(); ++i) {
        if(v[i] < min) {
            minid = i;
            min = v[i];
        }
    }

    return minid;
}
