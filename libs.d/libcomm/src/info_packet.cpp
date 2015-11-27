#include "lcomm/info_packet.h"

namespace lcomm {
    InfoPacket::InfoPacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    InfoPacket::InfoPacket(InfoPacket::State state, int detect_x, int detect_y)
            : m_state(state)
            , m_detect_x(detect_x)
            , m_detect_y(detect_y) {
        M_setup();
    }

    InfoPacket::State InfoPacket::state() const {
        return m_state;
    }

    int InfoPacket::detectX() const {
        return m_detect_x;
    }

    int InfoPacket::detectY() const {
        return m_detect_y;
    }

    void InfoPacket::M_setup() {
        bind("state", (int&)m_state);
        bind("detect_x", (int&)m_detect_x);
        bind("detect_y", (int&)m_detect_y);
    }
}
