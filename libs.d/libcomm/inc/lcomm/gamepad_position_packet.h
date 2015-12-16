#ifndef LCOMM_GAMEPAD_POSITION_PACKET_H
#define LCOMM_GAMEPAD_POSITION_PACKET_H

#include "lcomm/gamepad_packet_base.h"

namespace lcomm {
    class GamepadPositionPacket : public GamepadPacketBase<GamepadPositionPacket> {
    public:
    public:
        using GamepadPacketBase::GamepadPacketBase;
    };
}

#endif // LCOMM_GAMEPAD_POSITION_PACKET_H
