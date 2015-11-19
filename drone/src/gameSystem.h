//
// Created by remi on 13/11/15.
//

#ifndef SIMON_GAMESYSTEM_H
#define SIMON_GAMESYSTEM_H

#include <thread>
#include <atomic>
#include "lcomm/lcomm.h"

class GameSystem;

class GamePadSubscriber : public lcomm::Subscriber {
public:
    GamePadSubscriber(GameSystem &gs) : m_gs(gs) {}
    void notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) override;

private:
    GameSystem &m_gs;
};

class GameSystem {
public:
    GameSystem();
    ~GameSystem();

    void stop();

protected:
    void M_clientComThread();
    void M_droneSetup();

    lcomm::Endpoint m_endpoint;
    GamePadSubscriber m_gamePadSubscriber;
    std::atomic_bool m_alive = {false};
    std::thread m_clientComThread;
};


#endif // SIMON_GAMESYSTEM_H
