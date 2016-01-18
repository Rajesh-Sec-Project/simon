#ifndef SIMON_ROUNDMANAGER_H
#define SIMON_ROUNDMANAGER_H

#include "gameelement.h"
#include "lcomm/lcomm.h"
#include "moves.h"
#include "scoremanager.h"

class GameSystem;

class RoundManager : public GameElement, public lcomm::Subscriber {
public:
    RoundManager(GameSystem& system);
    ~RoundManager();

    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;

    void gameInit() override;
    void gameLoop() override;

    void userLeft();
    void userRight();
    void userUp();
    void userDown();

    void clearAndStart();
    void clear();

    void playMove(lmoves::tmove move, int delay = 500 /* ms */);
    void playWin();
    void playLose();

private:
    void M_playSequence();

    bool m_new_move;
    lmoves::Moves m_seq, m_user;
    std::size_t m_current_move;
    ScoreManager m_scoremgr;
};

#endif // SIMON_ROUNDMANAGER_H
