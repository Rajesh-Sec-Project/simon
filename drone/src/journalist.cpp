#include "journalist.h"
#include "gamesystem.h"
#include "navdatacontroller.h"
#include "lcomm/info_packet.h"

Journalist::Journalist(GameSystem& system)
        : GameElement(system)
        , m_landed(true)
        , m_detect(false)
        , m_detect_x(0)
        , m_detect_y(0)
        , m_speed_x(0.0f)
        , m_speed_y(0.0f) {
}

Journalist::~Journalist() {
}

void Journalist::gameInit() {
    M_sendInfoPacket();
}

void Journalist::gameLoop() {
    Navdata nav = m_system.navdataController().grab();

    if(m_landed && (nav.header.state & navdata::fly)) {
        m_landed = false;
        M_sendInfoPacket();
        M_message("Drone took off");
    } else if(!m_landed && (!(nav.header.state & navdata::fly))) {
        m_landed = true;
        M_sendInfoPacket();
        M_message("Drone landed");
    }

    if(m_detect && nav.vision_detect.nb_detected == 0) {
        m_detect = false;
        M_sendInfoPacket();
    } else if(!m_detect && nav.vision_detect.nb_detected != 0) {
        m_detect = true;
    }

    if(m_detect) {
        m_detect_x = (int) m_system.tagController().tagPositionX();
        m_detect_y = (int) m_system.tagController().tagPositionY();
        m_speed_x = m_system.tagController().tagSpeedX();
        m_speed_y = m_system.tagController().tagSpeedY();

        M_sendInfoPacket();
    }
}

void Journalist::M_sendInfoPacket() {
    using namespace lcomm;

    int state = m_landed ? InfoPacket::Landed : InfoPacket::Flying;
    if(m_detect)
        state |= InfoPacket::Detection;

    InfoPacket info((InfoPacket::State)state, m_detect_x, m_detect_y, m_speed_x, m_speed_y);
    m_system.endpoint().write(info);
}
