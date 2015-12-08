//
// Created by remi on 03/12/15.
//

#ifndef SIMON_GAMECONTROL_PACKET_BASE_H
#define SIMON_GAMECONTROL_PACKET_BASE_H

#include "lcomm/packet.h"

namespace lcomm {
    enum class GameState : int { Stopped, Running, Paused };

    class GameControlPacket : public Packet<GameControlPacket> {
    public:
        GameControlPacket(lconf::json::Node* node);
        GameControlPacket(GameState state);

        GameState gameState() const;

    private:
        void M_setup();

        int m_state;
    };
}

#endif // SIMON_GAMECONTROL_PACKET_BASE_H
