#ifndef LCOMM_GAMEPAD_POSITION_PACKET_H
#define LCOMM_GAMEPAD_POSITION_PACKET_H

#include "lcomm/packet.h"

namespace lcomm {
    class GamepadPositionPacket : public Packet<GamepadPositionPacket> {
    public:
        enum Keys {
            Up = 0x01,
            Down = 0x02,
            Left = 0x04,
            Right = 0x08,
            Stop = 0x10,

            TakeOff = 0x20,
            Land = 0x40
        };

    public:
        GamepadPositionPacket(lconf::json::Node* node);
        GamepadPositionPacket(Keys keys);

        Keys keys() const;
        void setKeys(Keys keys);

    private:
        void M_setup();

    private:
        Keys m_keys;
    };
}

#endif // LCOMM_GAMEPAD_PACKET_H
