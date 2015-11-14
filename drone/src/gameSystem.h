//
// Created by remi on 13/11/15.
//

#ifndef SIMON_GAMESYSTEM_H
#define SIMON_GAMESYSTEM_H

#include <thread>
#include <atomic>
#include "lcomm/lcomm.h"

class GamePadSubscriber : public lcomm::Subscriber {
public:
    void notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) override;
};

class GameSystem {
public:
    GameSystem();
    ~GameSystem() = default;

    void stop();

protected:
    void M_clientComThread();
    void M_droneSetup();

    lcomm::Endpoint m_endpoint;
    GamePadSubscriber m_gamePadSubscriber;
    std::atomic_bool m_alive = {true};
    std::thread m_clientComThread;
};


#endif // SIMON_GAMESYSTEM_H
