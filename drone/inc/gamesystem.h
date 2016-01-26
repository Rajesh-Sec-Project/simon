/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
#include "lchrono/chrono.h"
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

    LEDController& ledController();

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

    static lchrono::duration const m_gameLoopActivationTime;
};


#endif // SIMON_GAMESYSTEM_H
