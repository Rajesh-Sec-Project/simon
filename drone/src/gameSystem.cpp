//
// Created by remi on 13/11/15.
//

#include "gameSystem.h"
#include <chrono>
#include "lcomm/gamepad_packet.h"
#include "lcontrol/control.h"

using namespace std::literals;
using namespace lcomm;
using namespace lcontrol;

void GamePadSubscriber::notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) {
    GamepadPacket* ctrl = packet.downcast<GamepadPacket>();
    if(ctrl) {
        if(ctrl->keys() & GamepadPacket::Land) {
            Control::land();
        } else if(ctrl->keys() & GamepadPacket::TakeOff) {
            Control::takeoff();
        } else if(ctrl->keys() & GamepadPacket::Up) {
            std::cout << "Detected a movement to the top:\n"
                         "        / \\\n"
                         "      /     \\\n"
                         "    /__     __\\\n"
                         "       |   |\n"
                         "       |   |\n"
                         "       |   |\n"
                         "       |   |\n"
                         "       |___|\n"
                      << std::endl;
        } else if(ctrl->keys() & GamepadPacket::Down) {
            std::cout << "Detected a movement to the bottom:\n"
                         "        ___\n"
                         "       |   |\n"
                         "       |   |\n"
                         "       |   |\n"
                         "       |   |\n"
                         "     __|   |__\n"
                         "    \\         /\n"
                         "      \\     /\n"
                         "        \\ /\n"
                      << std::endl;
        } else if(ctrl->keys() & GamepadPacket::Left) {
            std::cout << "Detected a movement to the left:\n"
                         "  /|\n"
                         " / |_____________\n"
                         "/                |\n"
                         "\\   _____________|\n"
                         " \\ |\n"
                         "  \\|\n"
                      << std::endl;
        } else if(ctrl->keys() & GamepadPacket::Right) {
            std::cout << "Detected a movement to the right:\n"
                         "              |\\\n"
                         " _____________| \\\n"
                         "|                \\\n"
                         "|_____________   /\n"
                         "              | /\n"
                         "              |/\n"
                      << std::endl;
        } else if(ctrl->keys() & GamepadPacket::Stop) {
            std::cout << "Stop requested:\n"
                         "         ________________\n"
                         "       /.--------------.\\\n"
                         "      //                \\\\\n"
                         "     //                  \\\\\n"
                         "    || .-..----. .-. .--. ||\n"
                         "    ||( ( '-..-'|.-.||.-.|||\n"
                         "    || \\ \\  ||  || ||||_||||\n"
                         "    ||._) ) ||  \\'-'/||-' ||\n"
                         "     \\\\'-'  `'   `-' `'  //\n"
                         "      \\\\                //\n"
                         "       \\\\______________//\n"
                         "        '--------------'\n"
                         "              |_|_\n"
                         "       ____ _/ _)_)\n"
                         "           '  | (_)\n"
                         "        .--'\"\\| ()\n"
                         "              | |\n"
                         "              | |\n"
                         "              |_|\n"
                      << std::endl;

            Control::land();
            std::exit(0);
        }
    }
}

GameSystem::GameSystem()
        : m_endpoint(std::make_unique<ServerSocket>(50001)) {
    // m_clientComThread = std::thread(&GameSystem::M_clientComThread, this);
    m_endpoint.registerSubscriber(m_gamePadSubscriber);

    this->M_droneSetup();
}

void GameSystem::stop() {
    m_alive = false;
}

void GameSystem::M_droneSetup() {
    Control::init();
    Control::enableStabilization();
    std::cout << "Stabilization OK!" << std::endl;
}

void GameSystem::M_clientComThread() {
    while(m_alive) {
        std::this_thread::sleep_for(10ms);
    }
}