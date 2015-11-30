//
// Created by remi on 30/11/15.
//

#ifndef SIMON_GAMEPAD_PACKET_BASE_H
#define SIMON_GAMEPAD_PACKET_BASE_H

#include "lcomm/packet.h"

namespace lcomm {
    template<typename PacketType>
    class GamepadPacketBase : public Packet<PacketType> {
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
        GamepadPacketBase(lconf::json::Node* node) {
            M_setup();
            this->fromJson(node);
        }

        GamepadPacketBase(Keys keys) : m_keys(keys) {
            M_setup();
        }

        Keys keys() const {
            return m_keys;
        }
        void setKeys(Keys keys) {
            m_keys = keys;
        }

    private:
        void M_setup() {
            this->bind("keys", (int&)m_keys);
        }

    private:
        Keys m_keys;
    };
}

#endif //SIMON_GAMEPAD_PACKET_BASE_H
