#include "gamepadsubscriber.h"
#include "lcontrol/control.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "gamesystem.h"

using namespace lcontrol;
using namespace lcomm;
using namespace lmoves;

GamePadSubscriber::GamePadSubscriber(GameSystem& system)
        : GameElement(system), m_gs(system) {
}

/*
GenerateRound& GamePadSubscriber::get_m_gs(){
	return this->m_gs ;
}*/

void GamePadSubscriber::notify(Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    GamepadPacket* ctrl = packet->downcast<GamepadPacket>();
    if(ctrl) {
        if(ctrl->keys() & GamepadPacket::Land) {
            Control::land();
        } else if(ctrl->keys() & GamepadPacket::TakeOff) {
            Control::takeoff();
        } else if(ctrl->keys() & GamepadPacket::Up) {
            this->m_gs.lock_mtx1();
	    this->m_gs.set_last_move(tmove::UP) ;
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
             this->m_gs.unlock_mtx2();
        } else if(ctrl->keys() & GamepadPacket::Down) {
            this->m_gs.lock_mtx1();
	    this->m_gs.set_last_move(tmove::DOWN) ;
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
             this->m_gs.unlock_mtx2();
        } else if(ctrl->keys() & GamepadPacket::Left) {
            this->m_gs.lock_mtx1();
	    this->m_gs.set_last_move(tmove::LEFT) ;
            std::cout << "Detected a movement to the left:\n"
                         "  /|\n"
                         " / |_____________\n"
                         "/                |\n"
                         "\\   _____________|\n"
                         " \\ |\n"
                         "  \\|\n"
                      << std::endl;
             this->m_gs.unlock_mtx2();
        } else if(ctrl->keys() & GamepadPacket::Right) {
            this->m_gs.lock_mtx1();
	    this->m_gs.set_last_move(tmove::RIGHT) ;
            std::cout << "Detected a movement to the right:\n"
                         "              |\\\n"
                         " _____________| \\\n"
                         "|                \\\n"
                         "|_____________   /\n"
                         "              | /\n"
                         "              |/\n"
                      << std::endl;
             this->m_gs.unlock_mtx2();
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
