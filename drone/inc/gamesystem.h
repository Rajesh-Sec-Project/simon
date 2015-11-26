//
// Created by remi on 13/11/15.
//

#ifndef SIMON_GAMESYSTEM_H
#define SIMON_GAMESYSTEM_H

#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include "lcomm/lcomm.h"

#include "gamepadsubscriber.h"
#include "navdatacontroller.h"
#include "roundelcontroller.h"
#include "journalist.h"

//! The game system class, that manages :
//!   - all the game components
//!   - the main game loop
//!   - the communication with the host system
//!     (through an lcomm::Endpoint)
class GameSystem {
    friend class GameElement;

public:
    GameSystem();
    ~GameSystem();

    //! Stop the game system's loop
    void stop();

    //! Is the game alive ?
    bool alive() const;

    //! Get the system's navdata controller
    NavdataController& navdataController();

    //! Get the system's navdata controller (const version)
    NavdataController const& navdataController() const;

    //! Get the communication endpoint of this game system
    lcomm::Endpoint& endpoint();

    //! Send out a trace log to the host
    void trace(std::string const& nm, std::string const& msg);

    //! Send out a message log to the host
    void message(std::string const& nm, std::string const& msg);

    //! Send out a warning log to the host
    void warning(std::string const& nm, std::string const& msg);

    //! Send out an error log to the host
    void error(std::string const& nm, std::string const& msg);

protected:
    void M_droneSetup();
    void M_gameLoop();

private:
    lcomm::Endpoint m_endpoint;

    std::atomic_bool m_inited = {false};
    std::atomic_bool m_alive = {false};
    std::thread m_gameLoop;

    GamePadSubscriber m_gamePadSubscriber;
    NavdataController m_navctrl;
    RoundelController m_roundelctrl;
    Journalist m_journalist;

private:
    static unsigned long m_gameLoopActivationTimeNs;
};


#endif // SIMON_GAMESYSTEM_H
