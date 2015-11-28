#ifndef SIMON_GAMEPADSUBSCRIBER_H
#define SIMON_GAMEPADSUBSCRIBER_H

#include "lcomm/lcomm.h"
#include "gameelement.h"
#include <memory>
#include "../inc/moves.h"


class GameSystem;

//! A simple gamepad subscriber class, that responds
//!   to host's gamepad messages
class GamePadSubscriber : public GameElement, public lcomm::Subscriber {
public:
    GamePadSubscriber(GameSystem& system);

    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;

    //get m_gr
    //GenerateRound& GamePadSubscriber::get_m_gr() ;

private:
    GameSystem& m_gs;
};

#endif // SIMON_GAMEPADSUBSCRIBER_H
