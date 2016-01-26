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

#include "journalist.h"
#include "gamesystem.h"
#include "navdatacontroller.h"
#include "lcomm/info_packet.h"

Journalist::Journalist(GameSystem& system)
        : GameElement(system)
        , m_landed(true)
        , m_detect(false)
        , m_detect_x(0)
        , m_detect_y(0)
        , m_speed_x(0.0f)
        , m_speed_y(0.0f) {
}

Journalist::~Journalist() {
}

void Journalist::gameInit() {
    M_sendInfoPacket();
}

void Journalist::gameLoop() {
    Navdata nav = m_system.navdataController().grab();

    if(m_landed && (nav.header.state & navdata::fly)) {
        m_landed = false;
        M_sendInfoPacket();
        M_message("Drone took off");
    } else if(!m_landed && (!(nav.header.state & navdata::fly))) {
        m_landed = true;
        M_sendInfoPacket();
        M_message("Drone landed");
    }

    if(m_detect && nav.vision_detect.nb_detected == 0) {
        m_detect = false;
        M_sendInfoPacket();
    } else if(!m_detect && nav.vision_detect.nb_detected != 0) {
        m_detect = true;
    }

    if(m_detect) {
        m_detect_x = m_system.tagController().tagRawX();
        m_detect_y = m_system.tagController().tagRawY();
        m_speed_x = m_system.tagController().tagSpeedX();
        m_speed_y = m_system.tagController().tagSpeedY();

        M_sendInfoPacket();
    }
}

void Journalist::M_sendInfoPacket() {
    using namespace lcomm;

    int state = m_landed ? InfoPacket::Landed : InfoPacket::Flying;
    if(m_detect)
        state |= InfoPacket::Detection;

    InfoPacket info((InfoPacket::State)state, m_detect_x, m_detect_y, m_speed_x, m_speed_y);
    m_system.endpoint().write(info);
}
