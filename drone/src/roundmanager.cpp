#include "roundmanager.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

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
    m_seq.clearSequence();
    m_user.clearSequence();
    m_new_move = false;
    m_current_move = 0;
    m_seq.addRandomMove();
    M_playSequence();
    m_scoremgr.gameInit();
}

void RoundManager::M_playSequence() {
    std::ostringstream ss;
    ss << m_seq << std::endl;
    M_message(ss.str());

    for (auto m : m_seq.getSequence())
    {
        playMove(m);
    }

    m_scoremgr.setStart();
}

void RoundManager::clearAndStart() {
    this->gameInit();
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
                m_scoremgr.calculateScore();
                m_scoremgr.printScore();
                M_playSequence();
            } else {
                ++m_current_move;
                M_message("good");
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
    playMove(lmoves::tmove::UP, 100);
    m_new_move = true;
}

void RoundManager::userDown() {
    m_user.addMove(lmoves::tmove::DOWN);
    playMove(lmoves::tmove::DOWN, 100);
    m_new_move = true;
}

void RoundManager::userLeft() {
    m_user.addMove(lmoves::tmove::LEFT);
    playMove(lmoves::tmove::LEFT, 100);
    m_new_move = true;
}

void RoundManager::userRight() {
    m_user.addMove(lmoves::tmove::RIGHT);
    playMove(lmoves::tmove::RIGHT, 100);
    m_new_move = true;
}

void RoundManager::playMove(lmoves::tmove m, int delay) {
    LEDController::Led led;
    if (m == lmoves::tmove::RIGHT)
        led = LEDController::Yellow;
    else if (m == lmoves::tmove::UP)
        led = LEDController::Green;
    else if (m == lmoves::tmove::LEFT)
        led = LEDController::Red;
    else if (m == lmoves::tmove::DOWN)
        led = LEDController::Blue;

    m_system.ledController().setLed(led);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    m_system.ledController().unsetLed(led);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay/2));
}

void RoundManager::playWin() {
    int delay = 500;

    m_system.ledController().setLed(LEDController::Green);
    m_system.ledController().setLed(LEDController::Blue);
    m_system.ledController().setLed(LEDController::Yellow);
    m_system.ledController().setLed(LEDController::Red);

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    m_system.ledController().unsetLed(LEDController::Green);
    m_system.ledController().unsetLed(LEDController::Blue);
    m_system.ledController().unsetLed(LEDController::Yellow);
    m_system.ledController().unsetLed(LEDController::Red);

    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

void RoundManager::playLose() {
    int delay = 100;

    for (int times = 0; times < 10; ++times)
    {
        m_system.ledController().setLed(LEDController::Green);
        m_system.ledController().setLed(LEDController::Blue);
        m_system.ledController().setLed(LEDController::Yellow);
        m_system.ledController().setLed(LEDController::Red);

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        m_system.ledController().unsetLed(LEDController::Green);
        m_system.ledController().unsetLed(LEDController::Blue);
        m_system.ledController().unsetLed(LEDController::Yellow);
        m_system.ledController().unsetLed(LEDController::Red);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}
