//
// Created by remi on 30/11/15.
//

#ifndef SIMON_GAMEPAD_PACKET_BASE_H
#define SIMON_GAMEPAD_PACKET_BASE_H

#include "lcomm/packet.h"

namespace lcomm {
    template <typename PacketType>
    class GamepadPacketBase : public Packet<PacketType> {
    public:
        enum Keys : std::int32_t {
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

        GamepadPacketBase(Keys keys)
                : m_keys(keys) {
            M_setup();
        }

        Keys keys() const {
            return static_cast<Keys>(m_keys);
        }
        void setKeys(Keys keys) {
            m_keys = keys;
        }

    private:
        void M_setup() {
            this->bind("keys", m_keys);
        }

    private:
        int m_keys;
    };
}

#endif // SIMON_GAMEPAD_PACKET_BASE_H
