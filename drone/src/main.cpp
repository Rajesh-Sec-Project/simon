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

    std::getchar();

    std::exit(0);

    return 0;
}
