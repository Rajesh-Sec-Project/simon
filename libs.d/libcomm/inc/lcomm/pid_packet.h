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

#ifndef LCOMM_PID_PACKET_H
#define LCOMM_PID_PACKET_H

#include "lcomm/packet.h"
#include <cstdint>

namespace lcomm {
    class PIDPacket : public Packet<PIDPacket> {
    public:
        PIDPacket(lconf::json::Node* node);
        PIDPacket(float kp_z, float ki_z, float kd_z, float kp, float ki, float kd);

        float kp_z() const;
        float ki_z() const;
        float kd_z() const;

        float kp() const;
        float ki() const;
        float kd() const;

    private:
        void M_setup();

    private:
        float m_kp_z, m_ki_z, m_kd_z;
        float m_kp, m_ki, m_kd;
    };
}

#endif // LCOMM_PID_PACKET_H
