#ifndef LCOMM_GAMEPAD_PACKET_H
#define LCOMM_GAMEPAD_PACKET_H

#include "lcomm/gamepad_packet_base.h"

namespace lcomm {
    class GamepadPacket : public GamepadPacketBase<GamepadPacket> {
    public:
    public:
        using GamepadPacketBase::GamepadPacketBase;
    };
}

#endif // LCOMM_GAMEPAD_PACKET_H
