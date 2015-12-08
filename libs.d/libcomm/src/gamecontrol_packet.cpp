#include "lcomm/gamecontrol_packet.h"

namespace lcomm {
    GameControlPacket::GameControlPacket(lconf::json::Node* node) {
        M_setup();
        this->fromJson(node);
    }

    GameControlPacket::GameControlPacket(GameState state)
            : m_state(static_cast<int>(state)) {
        M_setup();
    }

    GameState GameControlPacket::gameState() const {
        return static_cast<GameState>(m_state);
    }

    void GameControlPacket::M_setup() {
        this->bind("state", m_state);
    }
}
