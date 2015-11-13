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
    GamePadSubscriber(int &seqNum, lcomm::ClientSocket &sock);
    void notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet);

protected:
    int &m_sequenceNum;
    lcomm::ClientSocket &m_socket;
};

class GameSystem {
public:
    GameSystem();
    ~GameSystem();

    void stop();

protected:
    void M_clientComThread();
    void M_droneSetup();

    lcomm::ClientSocket m_socket;
    int m_sequenceNum = 0;
    lcomm::Endpoint m_endpoint;
    GamePadSubscriber m_gamePadSubscriber;
    std::atomic_bool m_alive = {true};
    std::thread m_clientComThread;

};


#endif //SIMON_GAMESYSTEM_H
