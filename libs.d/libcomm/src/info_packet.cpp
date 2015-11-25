#include "lcomm/info_packet.h"

namespace lcomm {
    InfoPacket::InfoPacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    InfoPacket::InfoPacket(InfoPacket::State state)
            : m_state(state) {
        M_setup();
    }

    InfoPacket::State InfoPacket::state() const {
        return m_state;
    }

    void InfoPacket::M_setup() {
        bind("state", (int&)m_state);
    }
}
