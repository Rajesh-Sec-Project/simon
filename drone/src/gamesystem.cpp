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
// Created by remi on 13/11/15.
//

#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "lcontrol/control.h"
#include "lcontrol/positioncontrol.h"
#include "navdatacontroller.h"
#include <iomanip>
#include <fstream> // file I/O
#include <chrono>
#include <iostream>
#include <time.h>
#include <unistd.h>


//! Only messages with the log level specified or the levels above will be output to stdout/stderr
//! No messages will be output with a min log level of lcomm::LogPacket::NoLog
#define LOCAL_MIN_LOG_LEVEL lcomm::LogPacket::Message

using namespace std::literals;
using namespace lcomm;
using namespace lcontrol;
using namespace lmoves;

lchrono::duration const GameSystem::m_gameLoopActivationTime = 30ms;

GameSystem::GameSystem()
        : m_endpoint(std::make_unique<ServerSocket>(50001))
        , m_gamePadSubscriber(*this)
        , m_gameControlSubscriber(*this)
        , m_confmgr(*this)
        , m_navctrl(*this)
        , m_tagctrl(*this)
        , m_journalist(*this)
        , m_mouvement_stalker(*this)
        , m_roundmgr(*this)
        , m_ledcontroller(*this) {

    m_gameLoop = std::thread(&GameSystem::M_gameLoop, this);
    m_endpoint.registerSubscriber(m_gamePadSubscriber);
    m_endpoint.registerSubscriber(m_gameControlSubscriber);
    m_endpoint.registerSubscriber(m_roundmgr);
    m_endpoint.registerSubscriber(m_mouvement_stalker);

    std::cout << "Waiting for host to connect... ";
    std::cout.flush();
    while(!m_endpoint.socket().opened())
        ;
    std::cout << "OK" << std::endl;

    this->M_droneSetup();
}

GameSystem::~GameSystem() {
    m_alive = false;
    m_gameLoop.join();
}

void GameSystem::stop() {
    m_alive = false;
    PositionControl::stop();
}

bool GameSystem::alive() const {
    return m_alive;
}

NavdataController& GameSystem::navdataController() {
    return m_navctrl;
}

NavdataController const& GameSystem::navdataController() const {
    return m_navctrl;
}

ConfigManager& GameSystem::configManager() {
    return m_confmgr;
}

TagController& GameSystem::tagController() {
    return m_tagctrl;
}

LEDController& GameSystem::ledController() {
    return m_ledcontroller;
}

RoundManager& GameSystem::roundManager() {
    return m_roundmgr;
}

lcomm::Endpoint& GameSystem::endpoint() {
    return m_endpoint;
}

void GameSystem::trace(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Trace, str);
    // m_endpoint.write(log);

    if(lcomm::LogPacket::Trace >= LOCAL_MIN_LOG_LEVEL) {
        std::cout << "[TRACE]  " << str << std::endl;
    }
}

void GameSystem::message(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Message, str);
    // m_endpoint.write(log);


    if(lcomm::LogPacket::Message >= LOCAL_MIN_LOG_LEVEL) {
        std::cout << "[INFO]   " << str << std::endl;
    }
}

void GameSystem::warning(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Warning, str);
    // m_endpoint.write(log);

    if(lcomm::LogPacket::Warning >= LOCAL_MIN_LOG_LEVEL) {
        std::cout << "[WARN]  " << str << std::endl;
    }
}

void GameSystem::error(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Error, str);
    // m_endpoint.write(log);

    if(lcomm::LogPacket::Error >= LOCAL_MIN_LOG_LEVEL) {
        std::cout << "[ERROR]  " << str << std::endl;
    }
}

void GameSystem::score(int score) {
    lcomm::ScorePacket log(score);
    m_endpoint.write(log);
}

void GameSystem::M_droneSetup() {
    // Init AT command stuff
    Control::init();
    PositionControl::init();

    m_navctrl.init();
    while(!m_navctrl.inited())
        ;
    message("GameSystem", "NAVDATA inited");

    m_confmgr.init();
    message("GameSystem", "configuration OK");

    m_navctrl.configure();
    while(!m_navctrl.configured())
        ;
    message("GameSystem", "NAVDATA configured");

    // Start game loop
    m_inited = true;

    // Wait for the game loop to be started
    while(!m_alive)
        ;
}

void GameSystem::startGame() {
    m_started = true;
    m_roundmgr.clearAndStart();
}

void GameSystem::stopGame() {
    m_started = false;
    m_roundmgr.clear();
}

void GameSystem::M_gameLoop() {

    while(!m_inited)
        ;

    // OK, we're alive !
    m_alive = true;
    message("GameSystem", "starting main game loop");

    // Initialize components
    message("GameSystem", "initializing components");
    m_tagctrl.gameInit();
    m_mouvement_stalker.gameInit();
    m_journalist.gameInit();
    m_roundmgr.gameInit();
    m_confmgr.gameInit();
    m_navctrl.gameInit();
    m_ledcontroller.gameInit();

    // Send several FTRIM commands
    Control::enableStabilization();
    trace("GameSystem", "stabilization ok");

    InfoPacket info(InfoPacket::State::AppReady, 0, 0, 0.f, 0.f);
    this->endpoint().write(info);

    // Main game loop
    auto lastTime = lchrono::clock();
    while(m_alive) {
        auto lastTime = lchrono::clock();
        // Be sure to send the watchdog packet
        Control::watchdog();

        // Wait for new navdata (only slightly blocking)
        while(!m_navctrl.available())
            ;

        // Do stuff (regulations loops will go there for ex.)
        /*** Add you own elements here ***/
        m_confmgr.gameLoop();
        m_tagctrl.gameLoop();
        m_journalist.gameLoop();

        if(m_started) {
            m_roundmgr.gameLoop();
        }

        m_mouvement_stalker.gameLoop();
        m_navctrl.gameLoop();

        Navdata nav = m_navctrl.grab();
        (void)nav;

        // We wait for a positive duration which is equal to the activation time minus the time actually spent in the
        // loop iteration.
        std::this_thread::sleep_for(std::max(0ns, m_gameLoopActivationTime - (lchrono::clock() - lastTime)));
        lastTime = lchrono::clock();
    }
}
