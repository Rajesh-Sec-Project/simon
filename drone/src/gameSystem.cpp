//
// Created by remi on 13/11/15.
//

#include "gameSystem.h"
#include <chrono>
#include "lcomm/gamepad_packet.h"
#include "lcontrol/control.h"

using namespace std::literals;
using namespace lcomm;

GamePadSubscriber::GamePadSubscriber(int& seqNum, ClientSocket& sock)
        : m_sequenceNum(seqNum)
        , m_socket(sock) {
}


void GamePadSubscriber::notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet) {
    GamepadPacket* ctrl = packet->downcast<GamepadPacket>();
    if(ctrl) {
        std::cout << "Received: " << ctrl->keys() << std::endl;
        if(ctrl->keys() & GamepadPacket::Land) {
            for(int i = 0; i < 20; ++i) {
                lcontrol::control::land(m_sequenceNum++, m_socket);
            }
        } else if(ctrl->keys() & GamepadPacket::TakeOff) {
            for(int i = 0; i < 20; ++i) {
                lcontrol::control::takeoff(m_sequenceNum++, m_socket);
            }
        }
    }
}


GameSystem::GameSystem()
        : m_endpoint(std::make_unique<ServerSocket>(50001))
        , m_socket("127.0.0.1", 5556, false)
        , m_gamePadSubscriber(m_sequenceNum, m_socket) {
    // m_clientComThread = std::thread(&GameSystem::M_clientComThread, this);
    m_endpoint.registerSubscriber(&m_gamePadSubscriber);

    this->M_droneSetup();
}

GameSystem::~GameSystem() {
}

void GameSystem::stop() {
    m_alive = false;
}

void GameSystem::M_droneSetup() {
    for(int j = 0; j < 20; ++j) {
        lcontrol::control::horizontalPlan(m_sequenceNum++, m_socket);
        std::this_thread::sleep_for(20ms);
    }
    std::cout << "Stabilization OK!" << std::endl;
}

void GameSystem::M_clientComThread() {
    while(m_alive) {
        std::this_thread::sleep_for(10ms);
    }
}