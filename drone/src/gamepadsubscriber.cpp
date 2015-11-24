#include "gamepadsubscriber.h"
#include "lcontrol/control.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "gamesystem.h"

using namespace lcontrol;
using namespace lcomm;

GamePadSubscriber::GamePadSubscriber(GameSystem& system)
    : GameElement(system)
{}

void GamePadSubscriber::notify(Endpoint& ep, PacketBase const& packet) {
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
            m_system.stop();
        }
    }
}
