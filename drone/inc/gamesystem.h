//
// Created by remi on 13/11/15.
//

#ifndef SIMON_GAMESYSTEM_H
#define SIMON_GAMESYSTEM_H

#include <thread>
#include <atomic>
#include "lcomm/lcomm.h"
#include "gamepadsubscriber.h"
#include "navdata.h"

class GameSystem {
public:
    GameSystem();
    ~GameSystem();

    void stop();
    bool alive() const;

protected:
    void M_gameLoop();
    void M_droneSetup();
    void M_trace(std::string const& msg);

    lcomm::Endpoint m_endpoint;
    GamePadSubscriber m_gamePadSubscriber;
    std::atomic_bool m_alive = {false};
    std::thread m_gameLoop;
    NavdataController m_navctrl;
};


#endif // SIMON_GAMESYSTEM_H
