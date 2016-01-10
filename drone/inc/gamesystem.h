//
// Created by remi on 13/11/15.
//

#ifndef SIMON_GAMESYSTEM_H
#define SIMON_GAMESYSTEM_H

#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <mutex>
#include "lcomm/lcomm.h"
#include <ctime>
#include <sys/time.h>

#include "gamepadsubscriber.h"
#include "configmanager.h"
#include "navdatacontroller.h"
#include "tagcontroller.h"
#include "journalist.h"
#include "moves.h"
#include "mouvement_stalker.h"
#include "roundmanager.h"
#include "gamecontrolsubscriber.h"
#include "ledcontroller.h"

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

    //! Get the system's configuration manager
    ConfigManager& configManager();

    //! Get the system's tag detection manager
    TagController& tagController();

    //! Get the system's round manager
    RoundManager& roundManager();

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

    //! Send the score value to the host
    void score(int score);

    //! Return the elasped time since the creation of the game system.
    std::chrono::nanoseconds clock();

    bool started() const {
        return m_started;
    }

    void startGame();
    void stopGame();

protected:
    void M_droneSetup();
    void M_gameLoop();

private:
    lcomm::Endpoint m_endpoint;

    std::atomic_bool m_inited = {false};
    std::atomic_bool m_alive = {false};
    std::atomic_bool m_started = {false};
    std::thread m_gameLoop;

    GamePadSubscriber m_gamePadSubscriber;
    GameControlSubscriber m_gameControlSubscriber;

    ConfigManager m_confmgr;
    NavdataController m_navctrl;
    TagController m_tagctrl;
    Journalist m_journalist;
    Mouvement_Stalker m_mouvement_stalker;
    RoundManager m_roundmgr;
    LEDController m_ledcontroller;

    struct timeval m_timeref = {.tv_sec = -1, .tv_usec = 0};
    static std::chrono::nanoseconds const m_gameLoopActivationTime;
};


#endif // SIMON_GAMESYSTEM_H
