//
// Created by remi on 13/11/15.
//

#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "lcontrol/control.h"
#include "navdata.h"
#include <iomanip>
#include <chrono>
#include <iostream>
#include <ctime>

using namespace std::literals;
using namespace lcomm;
using namespace lcontrol;

GameSystem::GameSystem()
        : m_endpoint(std::make_unique<ServerSocket>(50001))
        , m_gamePadSubscriber(*this) {
    m_gameLoop = std::thread(&GameSystem::M_gameLoop, this);
    m_endpoint.registerSubscriber(m_gamePadSubscriber);

    this->M_droneSetup();
}

GameSystem::~GameSystem() {
    m_alive = false;
    m_gameLoop.join();
}

void GameSystem::stop() {
    m_alive = false;
}

bool GameSystem::alive() const {
    return m_alive;
}

void GameSystem::M_droneSetup() {
    // Init AT command stuff
    Control::init();

    // Send several FTRIM commands
    Control::enableStabilization();
    M_trace("stabilization ok");

    // Init the navdata system
    m_navctrl.init();

    // Wait for the game loop to be started
    while(!m_alive)
        ;
}

void GameSystem::M_gameLoop() {
    // Wait for the navdata controller to be
    // fully inited
    while(!m_navctrl.inited())
        ;

    // OK, we're alive !
    m_alive = true;
    M_trace("starting main game loop");

    // Main game loop
    while(m_alive) {
        // Be sure to send the watchdog packet
        Control::watchdog();

        // Print out navdata
        Navdata nav = m_navctrl.grab();

        std::cout << "theta: " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
                  << nav.demo.theta / 100.0f << std::endl;
        std::cout << "phi:   " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
                  << nav.demo.phi / 100.0f << std::endl;
        std::cout << "psi:   " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
                  << nav.demo.psi / 100.0f << std::endl;
        std::cout << "vx:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0') << nav.demo.vx
                  << std::endl;
        std::cout << "vy:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0') << nav.demo.vy
                  << std::endl;
        std::cout << "vz:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0') << nav.demo.vz
                  << std::endl;
        std::cout << "vbat:  " << std::fixed << std::setw(4) << std::setfill('0') << nav.demo.vbat_flying_percentage << std::endl;
        std::cout << "alt:   " << std::fixed << std::setw(4) << std::setfill('0') << nav.demo.altitude << std::endl;
        std::cout << "\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A";

        //@TODO: fix this time so the loop's
        //       activation time is accurate
        std::this_thread::sleep_for(5ms);
    }
}

void GameSystem::M_trace(std::string const& msg) {
    std::cout << "[GameSystem] " << msg << std::endl;
}
