#ifndef SIMON_ROUNDMANAGER_H
#define SIMON_ROUNDMANAGER_H

#include "gameelement.h"
#include "lcomm/lcomm.h"
#include "moves.h"

class GameSystem;

class RoundManager : public GameElement, public lcomm::Subscriber {
public:
    RoundManager(GameSystem& system);
    ~RoundManager();

    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;

    void gameInit() override;
    void gameLoop() override;

private:
    bool m_new_move;
    lmoves::Moves m_seq, m_user;
    std::size_t m_current_move;
    bool m_needs_print;
};

#endif // SIMON_ROUNDMANAGER_H
