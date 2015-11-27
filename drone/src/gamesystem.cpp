//
// Created by remi on 13/11/15.
//

#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcontrol/control.h"
#include "navdatacontroller.h"
#include <iomanip>
#include <chrono>
#include <iostream>
#include <time.h>

//! Define if you want to also print
//!   logging messages to stdout / stderr
#define LOCAL_LOGS

using namespace std::literals;
using namespace lcomm;
using namespace lcontrol;

unsigned long GameSystem::m_gameLoopActivationTimeNs = 10000;

GameSystem::GameSystem()
        : m_endpoint(std::make_unique<ServerSocket>(50001))
        , m_gamePadSubscriber(*this)
        , m_confmgr(*this)
        , m_navctrl(*this)
        , m_roundelctrl(*this)
        , m_journalist(*this) {
    m_gameLoop = std::thread(&GameSystem::M_gameLoop, this);
    m_endpoint.registerSubscriber(m_gamePadSubscriber);

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

lcomm::Endpoint& GameSystem::endpoint() {
    return m_endpoint;
}

void GameSystem::trace(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Trace, str);
    m_endpoint.write(log);

#if defined(LOCAL_LOGS)
    std::cout << "[TRACE]  " << str << std::endl;
#endif
}

void GameSystem::message(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Message, str);
    m_endpoint.write(log);

#if defined(LOCAL_LOGS)
    std::cout << "[INFO]   " << str << std::endl;
#endif
}

void GameSystem::warning(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Warning, str);
    m_endpoint.write(log);

#if defined(LOCAL_LOGS)
    std::cout << "[WARN]   " << str << std::endl;
#endif
}

void GameSystem::error(std::string const& nm, std::string const& msg) {
    std::string str = "(" + nm + ") " + msg;
    lcomm::LogPacket log(lcomm::LogPacket::Error, str);
    m_endpoint.write(log);

#if defined(LOCAL_LOGS)
    std::cerr << "[ERROR]  " << str << std::endl;
#endif
}

void GameSystem::M_droneSetup() {
    // Init AT command stuff
    Control::init();

    m_navctrl.init();
    while (!m_navctrl.inited())
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

void GameSystem::M_gameLoop() {

    while(!m_inited)
        ;

    // OK, we're alive !
    m_alive = true;
    message("GameSystem", "starting main game loop");

    // Initialize components
    message("GameSystem", "initializing components");
    m_roundelctrl.gameInit();
    m_journalist.gameInit();
    /*** Add your own elements ***/

    // Send several FTRIM commands
    Control::enableStabilization();
    trace("GameSystem", "stabilization ok");

    // Main game loop
    while(m_alive) {
        /*timespec loop_start;
        clock_gettime(CLOCK_REALTIME, &loop_start);*/

        // Be sure to send the watchdog packet
        Control::watchdog();

        // Do stuff (regulations loops will go there for ex.)
        m_confmgr.gameLoop();
        m_roundelctrl.gameLoop();
        m_journalist.gameLoop();
        /*** Add you own elements here ***/

        /****************************************************************************************************************/
        Navdata nav = m_navctrl.grab();
        std::string clr = "                      ";

        std::cout << "vision:" << nav.header.vision << clr << std::endl;
        std::cout << "theta: " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
                  << nav.demo.theta / 100.0f << clr << std::endl;
        std::cout << "phi:   " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
                  << nav.demo.phi / 100.0f << clr << std::endl;
        std::cout << "psi:   " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
                  << nav.demo.psi / 100.0f << clr << std::endl;
        std::cout << "vx:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0') << nav.demo.vx
                  << clr << std::endl;
        std::cout << "vy:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0') << nav.demo.vy
                  << clr << std::endl;
        std::cout << "vz:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0') << nav.demo.vz
                  << clr << std::endl;
        std::cout << "vbat:  " << std::fixed << std::setw(4) << std::setfill('0') << nav.demo.vbat_flying_percentage
                  << clr << std::endl;
        std::cout << "alt:   " << std::fixed << std::setw(4) << std::setfill('0') << nav.demo.altitude << clr << std::endl;
        std::cout << "tag:   " << nav.demo.detection_camera_type << clr << std::endl;
        std::cout << "nb:    " << nav.vision_detect.nb_detected << clr << std::endl;
        std::cout << "xc[0]: " << nav.vision_detect.xc[0] << clr << std::endl;
        std::cout << "yc[0]: " << nav.vision_detect.yc[0] << clr << std::endl;
        std::cout << "video_thread: " << ((nav.header.state & navdata::video_thread) ? "yes" : "no") << clr << std::endl;
        std::cout << "acq_thread: " << ((nav.header.state & navdata::acq_thread) ? "yes" : "no") << clr << std::endl;
        std::cout << std::endl;

        std::cout << "\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A";
        /****************************************************************************************************************/

        /*timespec loop_end;
        clock_gettime(CLOCK_REALTIME, &loop_end);
        unsigned long loop_time = ((loop_end.tv_sec   * 1000000000UL) + loop_end.tv_nsec)
                                - ((loop_start.tv_sec * 1000000000UL) + loop_start.tv_nsec);
        
        if (loop_time > m_gameLoopActivationTimeNs)
            continue;

        std::this_thread::sleep_for(std::chrono::nanoseconds(m_gameLoopActivationTimeNs - loop_time));*/

        std::this_thread::sleep_for(10ms);
    }
}
