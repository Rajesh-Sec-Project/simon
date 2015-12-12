#ifndef SIMON_GAMEPADSUBSCRIBER_H
#define SIMON_GAMEPADSUBSCRIBER_H

#include "lcomm/lcomm.h"
#include "gameelement.h"
#include <memory>
#include "../inc/moves.h"

//! A simple gamepad subscriber class, that responds
//!   to host's gamepad messages
class GamePadSubscriber : public GameElement, public lcomm::Subscriber {
public:
    GamePadSubscriber(GameSystem& system);

    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;
};

#endif // SIMON_GAMEPADSUBSCRIBER_H
