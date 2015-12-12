#include "roundmanager.h"
#include "gamesystem.h"
#include "lcomm/gamepad_packet.h"
#include <sstream>
#include <ctime>

RoundManager::RoundManager(GameSystem& system)
        : GameElement(system) 
        , m_scoremgr(system) {
}

RoundManager::~RoundManager() {
}

void RoundManager::notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    lcomm::GamepadPacket* pkt = packet->downcast<lcomm::GamepadPacket>();

    unsigned int oldsz = m_user.size();

    if(pkt) {
        if(pkt->keys() & lcomm::GamepadPacket::Up) {
            m_user.addMove(lmoves::tmove::UP);
        } else if(pkt->keys() & lcomm::GamepadPacket::Down) {
            m_user.addMove(lmoves::tmove::DOWN);
        } else if(pkt->keys() & lcomm::GamepadPacket::Left) {
            m_user.addMove(lmoves::tmove::LEFT);
        } else if(pkt->keys() & lcomm::GamepadPacket::Right) {
            m_user.addMove(lmoves::tmove::RIGHT);
        }
    }

    if(oldsz != m_user.size())
        m_new_move = true;
}

void RoundManager::gameInit() {
    m_new_move = false;
    m_current_move = 0;
    m_needs_print = true;
    m_seq.addRandomMove();
    m_scoremgr.gameInit();
}

void RoundManager::gameLoop() {
    if(m_needs_print) {
        std::ostringstream ss;
        ss << m_seq << std::endl;
        M_message(ss.str());
        m_needs_print = false;
        m_scoremgr.setStart();
    }

    if(m_new_move) {
        m_scoremgr.setEnd();
        m_new_move = false;

        if(m_seq.getSequence()[m_current_move] == m_user.getSequence()[m_current_move]) {
            if(m_current_move == (m_seq.getSequence().size() - 1)) {
                m_seq.addRandomMove();
                m_user.clearSequence();
                m_needs_print = true;
                m_current_move = 0;

                m_scoremgr.calculateScore();
                m_scoremgr.printScore();

                //M_message("well done !");
            } else {
                m_current_move++;
                //M_message("good");
            }
            
            

        } else {
            M_message("Game OVER");
        }
    }
}
