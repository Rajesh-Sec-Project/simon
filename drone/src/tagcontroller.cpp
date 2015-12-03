#include "tagcontroller.h"
#include "lcontrol/control.h"
#include "navdatacontroller.h"
#include "gamesystem.h"

#include <thread>
#include <iostream>
#include <iomanip>
#include <chrono>

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
        , m_navctrl(system.navdataController()) {
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

    Control::config("detect:detect_type", std::to_string(Mode_Multiple));
    M_clearAck();

    Control::config("detect:detections_select_h", std::to_string((0x01 << (Tag_ShellsV2 - 1))));
    M_clearAck();

    Control::config("detect:enemy_colors", "3");
    M_clearAck();

    Control::config("detect:enemy_without_shell", "0");
    M_clearAck();

    m_avg_update = 0.75f;
    m_avg_corr_update = 0.25f;
    m_tag_x = 0.0f;
    m_tag_y = 0.0f;
    m_avg_vx = 0.0f;
    m_avg_vy = 0.0f;
    m_avg_cor_vx = 0.0f;
    m_avg_cor_vy = 0.0f;
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

    /**/ std::string clr = "                      ";
    /**/ int nlines = 0;
    /**/ #define FMT std::fixed << std::setw(3) << std::setprecision(2) << std::setfill('0')
        /**/ #define ENDL clr << std::endl;
    nlines++;

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

        // Debug prints
        std::cout << "vx:  " << FMT << m_avg_cor_vx << ENDL;
        std::cout << "vy:  " << FMT << m_avg_cor_vy << ENDL;
    }

    /**/ for(int i = 0; i < nlines; ++i)
        /**/ std::cout << "\e[A";
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
