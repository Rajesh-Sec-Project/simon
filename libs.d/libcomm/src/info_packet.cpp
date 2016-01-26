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

#include "lcomm/info_packet.h"

namespace lcomm {
    InfoPacket::InfoPacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    InfoPacket::InfoPacket(InfoPacket::State state, int detect_x, int detect_y, float speed_x, float speed_y)
            : m_state(state)
            , m_detect_x(detect_x)
            , m_detect_y(detect_y)
            , m_speed_x(speed_x)
            , m_speed_y(speed_y) {
        M_setup();
    }

    InfoPacket::State InfoPacket::state() const {
        return m_state;
    }

    int InfoPacket::detectX() const {
        return m_detect_x;
    }

    int InfoPacket::detectY() const {
        return m_detect_y;
    }

    float InfoPacket::speedX() const {
        return m_speed_x;
    }

    float InfoPacket::speedY() const {
        return m_speed_y;
    }

    void InfoPacket::M_setup() {
        bind("state", (int&)m_state);
        bind("detect_x", (int&)m_detect_x);
        bind("detect_y", (int&)m_detect_y);
        bind("speed_x", m_speed_x);
        bind("speed_y", m_speed_y);
    }
}
