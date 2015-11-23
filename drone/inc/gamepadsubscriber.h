#ifndef SIMON_GAMEPADSUBSCRIBER_H
#define SIMON_GAMEPADSUBSCRIBER_H

#include "lcomm/lcomm.h"

class GameSystem;

class GamePadSubscriber : public lcomm::Subscriber {
public:
    GamePadSubscriber(GameSystem& gs)
            : m_gs(gs) {
    }
    void notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) override;

private:
    GameSystem& m_gs;
};

#endif // SIMON_GAMEPADSUBSCRIBER_H
