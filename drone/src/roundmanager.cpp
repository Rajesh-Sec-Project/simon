#include "roundmanager.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include <sstream>

RoundManager::RoundManager(GameSystem& system)
        : GameElement(system) {
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
    m_new_move = false;
    m_current_move = 0;
    m_needs_print = true;
    m_seq.addRandomMove();
}

void RoundManager::gameLoop() {
    if(m_needs_print) {
        std::ostringstream ss;
        ss << m_seq << std::endl;
        M_message(ss.str());
        m_needs_print = false;
    }

    if(m_new_move) {
        m_new_move = false;

        if(m_seq.getSequence()[m_current_move] == m_user.getSequence()[m_current_move]) {
            if(m_current_move == (m_seq.getSequence().size() - 1)) {
                m_seq.addRandomMove();
                m_user.clearSequence();
                m_needs_print = true;
                m_current_move = 0;
                M_message("well done !");
            } else {
                m_current_move++;
                M_message("good");
            }
        } else {
            M_message("Game OVER");
        }
    }
}

void RoundManager::userUp() {
    m_user.addMove(lmoves::tmove::UP);
    m_new_move = true;
}

void RoundManager::userDown() {
    m_user.addMove(lmoves::tmove::DOWN);
    m_new_move = true;
}

void RoundManager::userLeft() {
    m_user.addMove(lmoves::tmove::LEFT);
    m_new_move = true;
}

void RoundManager::userRight() {
    m_user.addMove(lmoves::tmove::RIGHT);
    m_new_move = true;
}
