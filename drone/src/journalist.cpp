#include "journalist.h"
#include "gamesystem.h"
#include "navdatacontroller.h"
#include "lcomm/info_packet.h"

Journalist::Journalist(GameSystem& system)
        : GameElement(system)
        , m_landed(true) {
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
}

void Journalist::M_sendInfoPacket() {
    using namespace lcomm;

    InfoPacket info(m_landed ? InfoPacket::Landed : InfoPacket::Flying);
    m_system.endpoint().write(info);
}
