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

#include "roundmanager.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/sound_packet.h"
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std::literals;

RoundManager::RoundManager(GameSystem& system)
        : GameElement(system)
        , m_scoremgr(system) {
}

RoundManager::~RoundManager() {
}

void RoundManager::notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    lcomm::GamepadPacket* pkt = packet->downcast<lcomm::GamepadPacket>();

    if(pkt) {
        if(pkt->keys() & lcomm::GamepadPacket::Up) {
            userUp();
        } else if(pkt->keys() & lcomm::GamepadPacket::Down) {
            userDown();
        } else if(pkt->keys() & lcomm::GamepadPacket::Left) {
            userLeft();
        } else if(pkt->keys() & lcomm::GamepadPacket::Right) {
            userRight();
        }
    }
}

void RoundManager::gameInit() {
    m_scoremgr.gameInit();
}

void RoundManager::M_playSequence() {
    std::ostringstream ss;
    ss << m_seq << std::endl;
    M_message(ss.str());

    for(auto m : m_seq.getSequence()) {
        playMove(m);
    }

    m_scoremgr.setStart();
}

void RoundManager::clearAndStart() {
    m_seq.clearSequence();
    m_user.clearSequence();
    m_new_move = false;
    m_current_move = 0;
    m_seq.addRandomMove();
    M_playSequence();
    m_scoremgr.clear();
}

void RoundManager::clear() {
    m_scoremgr.clear();
}

void RoundManager::gameLoop() {
    if(m_new_move) {
        m_scoremgr.setEnd();
        m_new_move = false;

        if(m_seq.getSequence()[m_current_move] == m_user.getSequence()[m_current_move]) {
            if(m_current_move == (m_seq.getSequence().size() - 1)) {
                m_seq.addRandomMove();
                m_user.clearSequence();
                m_current_move = 0;
                M_message("well done !");
                playWin();
                m_scoremgr.calculateScore(m_seq.size());
                m_scoremgr.printScore();
                M_playSequence();
            } else {
                ++m_current_move;
                M_message("good");
                lcomm::SoundPacket sound(lcomm::SoundPacket::Good);
                m_system.endpoint().write(sound);
            }
        } else {
            playLose();
            M_message("Game OVER");
            m_scoremgr.gameOver();
            this->clear();
        }
    }
}

void RoundManager::userUp() {
    m_user.addMove(lmoves::tmove::UP);
    playMove(lmoves::tmove::UP, 100ms);
    m_new_move = true;
}

void RoundManager::userDown() {
    m_user.addMove(lmoves::tmove::DOWN);
    playMove(lmoves::tmove::DOWN, 100ms);
    m_new_move = true;
}

void RoundManager::userLeft() {
    m_user.addMove(lmoves::tmove::LEFT);
    playMove(lmoves::tmove::LEFT, 100ms);
    m_new_move = true;
}

void RoundManager::userRight() {
    m_user.addMove(lmoves::tmove::RIGHT);
    playMove(lmoves::tmove::RIGHT, 100ms);
    m_new_move = true;
}

void RoundManager::playMove(lmoves::tmove m, lchrono::duration delay) {
    LEDController::Led led;
    if(m == lmoves::tmove::RIGHT)
        led = LEDController::Yellow;
    else if(m == lmoves::tmove::UP)
        led = LEDController::Green;
    else if(m == lmoves::tmove::LEFT)
        led = LEDController::Red;
    else if(m == lmoves::tmove::DOWN)
        led = LEDController::Blue;

    m_system.ledController().setLed(led);
    std::this_thread::sleep_for(delay);
    m_system.ledController().unsetLed(led);
    std::this_thread::sleep_for(delay / 2);
}

void RoundManager::playWin() {
    auto delay = 500ms;

    lcomm::SoundPacket sound(lcomm::SoundPacket::Win);
    m_system.endpoint().write(sound);

    m_system.ledController().setLed(LEDController::Green);
    m_system.ledController().setLed(LEDController::Blue);
    m_system.ledController().setLed(LEDController::Yellow);
    m_system.ledController().setLed(LEDController::Red);

    std::this_thread::sleep_for(delay);

    m_system.ledController().unsetLed(LEDController::Green);
    m_system.ledController().unsetLed(LEDController::Blue);
    m_system.ledController().unsetLed(LEDController::Yellow);
    m_system.ledController().unsetLed(LEDController::Red);

    std::this_thread::sleep_for(delay);
}

void RoundManager::playLose() {
    auto delay = 100ms;

    lcomm::SoundPacket sound(lcomm::SoundPacket::Loose);
    m_system.endpoint().write(sound);

    for(int times = 0; times < 10; ++times) {
        m_system.ledController().setLed(LEDController::Green);
        m_system.ledController().setLed(LEDController::Blue);
        m_system.ledController().setLed(LEDController::Yellow);
        m_system.ledController().setLed(LEDController::Red);

        std::this_thread::sleep_for(delay);
        m_system.ledController().unsetLed(LEDController::Green);
        m_system.ledController().unsetLed(LEDController::Blue);
        m_system.ledController().unsetLed(LEDController::Yellow);
        m_system.ledController().unsetLed(LEDController::Red);
        std::this_thread::sleep_for(delay);
    }
}
