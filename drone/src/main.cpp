#include <iostream>
#include <thread>
#include <iomanip>

#include "lcomm/lcomm.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "navdatacontroller.h"
#include "lcomm/gamecontrol_packet.h"


using namespace std::literals;

int main() {
    srand(time(NULL));

    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPositionPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::LogPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::InfoPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::GameControlPacket>();

    std::cout << "App's per-thread stack size (kB): " << std::endl;
    std::system("ulimit -s");

    GameSystem system;

    while(system.alive())
        std::this_thread::sleep_for(100ms);

    return 0;
}
