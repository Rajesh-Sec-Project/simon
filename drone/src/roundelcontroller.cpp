#include "roundelcontroller.h"
#include "lcontrol/control.h"
#include "navdatacontroller.h"
#include "gamesystem.h"

#include <thread>
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

RoundelController::RoundelController(GameSystem& system)
        : GameElement(system)
        , m_navctrl(system.navdataController()) {
}

RoundelController::~RoundelController() {
}

void RoundelController::init() {
    while(!m_navctrl.inited())
        ;

    Control::config("video:codec_fps", "30");
    M_clearAck();

    Control::config("video:video_codec", "129");
    M_clearAck();

    Control::config("video:max_bitrate", "1000");
    M_clearAck();

    Control::config("video:video_channel", "0");
    M_clearAck();

    Control::config("detect:groundstripe_colors", "3");
    M_clearAck();

    Control::config("detect:detect_type", "13");
    M_clearAck();

    Control::config("detect:detections_select_h", "32");
    M_clearAck();

    Control::config("detect:detections_select_v_hsync", "0");
    M_clearAck();

    Control::config("detect:detections_select_v", "0");
    M_clearAck();
}

void RoundelController::M_clearAck() {
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
            throw std::runtime_error("NavdataController::M_initNavdata: ack not sent !");
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
            throw std::runtime_error("NavdataController::M_initNavdata: ack not cleared !");
        }
    }
    M_trace("command ack cleared");
}
