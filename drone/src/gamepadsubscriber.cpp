#include "gamepadsubscriber.h"
#include "lcontrol/control.h"
#include "lcontrol/positioncontrol.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "gamesystem.h"

using namespace lcontrol;
using namespace lcomm;
using namespace lmoves;

GamePadSubscriber::GamePadSubscriber(GameSystem& system)
        : GameElement(system) {
}

void GamePadSubscriber::notify(Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    std::cout << "1234567" << std::endl;
    if(GamepadPositionPacket* ctrl = packet->downcast<GamepadPositionPacket>()) {
     std::cout << "abd7" << std::endl;
       if(ctrl->keys() & GamepadPacket::Land) {
            Control::land();
        } else if(ctrl->keys() & GamepadPacket::TakeOff) {
            Control::takeoff();
        } else if(ctrl->keys() & GamepadPacket::Up) {

            PositionControl::up(50);
        }
        else if(ctrl->keys() & GamepadPacket::Down) {
            PositionControl::down(50);
        }
        else if(ctrl->keys() & GamepadPacket::Left) {

            PositionControl::up(50);
        } else if(ctrl->keys() & GamepadPacket::Down) {
            PositionControl::down(50);
        }
    }
    else if(GamepadPacket* ctrl = packet->downcast<GamepadPacket>()) {
        std::cout << "sdfsdfdfsd" << std::endl;
        if(ctrl->keys() & GamepadPacket::Stop) {
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
