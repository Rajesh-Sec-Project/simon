//
// Created by remi on 03/12/15.
//

#include "gamecontrolsubscriber.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamecontrol_packet.h"
#include "gamesystem.h"

using namespace lcomm;

using lcomm::GameState;

GameControlSubscriber::GameControlSubscriber(GameSystem& system)
        : GameElement(system) {
}

void GameControlSubscriber::notify(Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    if(GameControlPacket* ctrl = packet->downcast<GameControlPacket>()) {
        if(ctrl->gameState() == GameState::Stopped) {
            M_message("The game has been stopped!");
        } else if(ctrl->gameState() == GameState::Paused) {
            M_message("The game has been paused!");
        } else if(ctrl->gameState() == GameState::Running) {
            M_message("The game has been started!");
        }
    }
}
