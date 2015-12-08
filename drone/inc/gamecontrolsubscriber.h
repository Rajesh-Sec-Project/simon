//
// Created by remi on 03/12/15.
//

#ifndef SIMON_GAMECONTROLSUBSCRIBER_H
#define SIMON_GAMECONTROLSUBSCRIBER_H

#include "lcomm/lcomm.h"
#include "gameelement.h"
#include <memory>
#include "../inc/moves.h"

class GameControlSubscriber : public GameElement, public lcomm::Subscriber {
public:
    GameControlSubscriber(GameSystem& system);

    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;
};

#endif // SIMON_GAMECONTROLSUBSCRIBER_H
