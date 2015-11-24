#ifndef SIMON_GAMEPADSUBSCRIBER_H
#define SIMON_GAMEPADSUBSCRIBER_H

#include "lcomm/lcomm.h"
#include "gameelement.h"

class GameSystem;

//! A simple gamepad subscriber class, that responds
//!   to host's gamepad messages
class GamePadSubscriber : public GameElement, public lcomm::Subscriber {
public:
    GamePadSubscriber(GameSystem& system);

    void notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) override;
};

#endif // SIMON_GAMEPADSUBSCRIBER_H
