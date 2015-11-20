#include <iostream>

#include "lcomm/lcomm.h"
#include "gameSystem.h"
#include <thread>
#include "lcomm/gamepad_packet.h"

using namespace std::literals;

int main() {
    using namespace lcomm;
    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();

    std::cout << "App's per-thread stack size (kB): " << std::endl;
    std::system("ulimit -s");

    { GameSystem system; }

    /*while(true) {
        std::this_thread::sleep_for(1s);
    }*/

    return 0;
}
