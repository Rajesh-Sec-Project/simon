/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
