#include <iostream>

#include "lcomm/lcomm.h"
#include "gameSystem.h"
#include <thread>
#include "lcomm/gamepad_packet.h"

using namespace std::literals;

int main() {
    using namespace lcomm;
    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();


    GameSystem system;

    while(true) {
        std::this_thread::sleep_for(10ms);
    }

    return 0;
}
