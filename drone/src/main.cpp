#include <iostream>
#include <thread>
#include <iomanip>

#include "lcomm/lcomm.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "navdatacontroller.h"
#include "lcomm/gamecontrol_packet.h"
#include "lcomm/pid_packet.h"

using namespace std::literals;

int main() {
    srand(time(nullptr));

    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPositionPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::LogPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::InfoPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::GameControlPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::ScorePacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::PIDPacket>();

    std::cout << "App's per-thread stack size (kB): " << std::endl;
    std::system("ulimit -s");

    GameSystem system;

    while(system.alive())
        std::this_thread::sleep_for(100ms);

    return 0;
}
