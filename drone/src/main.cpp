#include <iostream>
#include <thread>
#include <iomanip>

#include "lcomm/lcomm.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "randomsequence.h"
#include "navdata.h"

using namespace std::literals;

int main() {
    using namespace lcomm;
    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();

    std::cout << "App's per-thread stack size (kB): " << std::endl;
    std::system("ulimit -s");

    GameSystem system;

    for(; system.alive();)
        ;

    return 0;
}
