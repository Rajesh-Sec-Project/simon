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

#include "lcomm/pid_packet.h"
using namespace lcomm;

PIDPacket::PIDPacket(lconf::json::Node* node) {
    M_setup();
    fromJson(node);
}

PIDPacket::PIDPacket(float kp_z, float ki_z, float kd_z, float kp, float ki, float kd)
        : m_kp_z(kp_z)
        , m_ki_z(ki_z)
        , m_kd_z(kd_z)
        , m_kp(kp)
        , m_ki(ki)
        , m_kd(kd) {
    M_setup();
}

float PIDPacket::kp_z() const {
    return m_kp_z;
}
float PIDPacket::ki_z() const {
    return m_ki_z;
}
float PIDPacket::kd_z() const {
    return m_kd_z;
}

float PIDPacket::kp() const {
    return m_kp;
}
float PIDPacket::ki() const {
    return m_ki;
}
float PIDPacket::kd() const {
    return m_kd;
}

void PIDPacket::M_setup() {
    bind("kp_z", m_kp_z);
    bind("ki_z", m_ki_z);
    bind("kd_z", m_kd_z);
    bind("kp", m_kp);
    bind("ki", m_ki);
    bind("kd", m_kd);
}
