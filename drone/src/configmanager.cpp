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

#include "configmanager.h"
#include "gamesystem.h"
#include "navdatacontroller.h"
#include "lcontrol/control.h"
#include <thread>
#include <chrono>

ConfigManager::ConfigManager(GameSystem& system)
        : GameElement(system)
        , m_control("127.0.0.1", 5559) {
}

ConfigManager::~ConfigManager() {
}

void ConfigManager::init() {
    using namespace lcontrol;

    M_trace("Opening control port...");
    m_control.connect();
    M_trace("OK");

    M_trace("Sending init sequence");
    Control::strangeInit();

    M_trace("Clearing session id");
    Control::clearSessionId();
    M_clearAck();

    M_trace("Setting session id");
    Control::setSessionId();
    M_clearAck();

    M_trace("Setting app id");
    Control::setAppId();
    M_clearAck();

    M_trace("Setting session desc");
    Control::setSessionDesc();
    M_clearAck();

    M_trace("Setting app desc");
    Control::setAppDesc();
    M_clearAck();

    getConfig();
}

std::string ConfigManager::getConfig() {
    using namespace lcontrol;

    M_trace("Asking for the configuration");
    Control::getCfgControl();

    std::string config = "";
    std::string data = "";
    while(!m_control.read(&data))
        ;

    do {
        config += data;
    } while(m_control.read(&data));

    return config;
}

void ConfigManager::M_clearAck() {
    using namespace lcontrol;

    // Wait for the command_ack bit
    for(int tm = 0;;) {
        // Get some navdata
        while(!m_system.navdataController().available())
            ;
        Navdata nav = m_system.navdataController().grab();

        M_trace("waiting for ack #" + std::to_string(tm));

        // Check flag
        if(nav.header.state & navdata::command_ack)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        if(++tm > 50) {
            M_error("command ack not set");
            throw std::runtime_error("ConfigManager::M_clearAck: ack not sent !");
        }
    }
    M_trace("got command ack");

    // Clear it, and we're OK !
    Control::ackControl();
    M_trace("ack clear sent");

    // Wait for the command_ack bit to be cleared
    for(int tm = 0;;) {
        // Get some navdata
        while(!m_system.navdataController().available())
            ;
        Navdata nav = m_system.navdataController().grab();

        M_trace("waiting for cleared ack #" + std::to_string(tm));

        // Check flag
        if(!(nav.header.state & navdata::command_ack))
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        if(++tm > 50) {
            M_error("command ack not cleared");
            throw std::runtime_error("ConfigManager::M_clearAck: ack not cleared !");
        }
    }
    M_trace("command ack cleared");
}
