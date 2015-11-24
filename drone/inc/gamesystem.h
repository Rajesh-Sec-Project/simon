//
// Created by remi on 13/11/15.
//

#ifndef SIMON_GAMESYSTEM_H
#define SIMON_GAMESYSTEM_H

#include <thread>
#include <atomic>
#include "lcomm/lcomm.h"

#include "gamepadsubscriber.h"
#include "navdatacontroller.h"
#include "roundelcontroller.h"

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

private:
    lcomm::Endpoint m_endpoint;
    GamePadSubscriber m_gamePadSubscriber;
    std::atomic_bool m_inited = {false};
    std::atomic_bool m_alive = {false};
    std::thread m_gameLoop;

    NavdataController m_navctrl;
    RoundelController m_roundelctrl;
};


#endif // SIMON_GAMESYSTEM_H
