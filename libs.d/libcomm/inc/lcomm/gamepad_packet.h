#ifndef LCOMM_GAMEPAD_PACKET_H
#define LCOMM_GAMEPAD_PACKET_H

#include "lcomm/packet.h"

namespace lcomm {
    class GamepadPacket : public Packet<GamepadPacket> {
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
        GamepadPacket(lconf::json::Node* node);
        GamepadPacket(Keys keys);
        ~GamepadPacket();

        Keys keys() const;
        void setKeys(Keys keys);

    private:
        void M_setup();

    private:
        Keys m_keys;
    };
}

#endif // LCOMM_GAMEPAD_PACKET_H
