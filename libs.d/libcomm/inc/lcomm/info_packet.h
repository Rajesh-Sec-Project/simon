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

#ifndef LCOMM_INFO_PACKET_H
#define LCOMM_INFO_PACKET_H

#include "lcomm/packet.h"

namespace lcomm {
    class InfoPacket : public Packet<InfoPacket> {
    public:
        enum State { Landed = 0x01, Flying = 0x02, Detection = 0x04, AppReady = 0x08 };

    public:
        InfoPacket(lconf::json::Node* node);
        InfoPacket(State state, int detect_x, int detect_y, float speed_x, float speed_y);

        State state() const;
        int detectX() const;
        int detectY() const;
        float speedX() const;
        float speedY() const;

    private:
        void M_setup();

    private:
        State m_state;
        int m_detect_x, m_detect_y;
        float m_speed_x, m_speed_y;
    };
}

#endif // LCOMM_LOG_PACKET_H
