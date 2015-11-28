#include "lcomm/gamepad_position_packet.h"

namespace lcomm {
    GamepadPositionPacket::GamepadPositionPacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    GamepadPositionPacket::GamepadPositionPacket(GamepadPositionPacket::Keys keys)
            : m_keys(keys) {
        M_setup();
    }

    GamepadPositionPacket::Keys GamepadPositionPacket::keys() const {
        return m_keys;
    }

    void GamepadPositionPacket::setKeys(GamepadPositionPacket::Keys keys) {
        m_keys = keys;
    }

    void GamepadPositionPacket::M_setup() {
        bind("keys", (int&)m_keys);
    }
}
