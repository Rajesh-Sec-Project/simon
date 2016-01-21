#ifndef SIMON_SCOREMANAGER_H
#define SIMON_SCOREMANAGER_H

#include "gameelement.h"
#include <ctime>

#include <chrono>
#include "lchrono/chrono.h"

class GameSystem;

class ScoreManager : public GameElement {
public:
    ScoreManager(GameSystem& system);
    ~ScoreManager();

    void gameInit() override;
    void gameLoop() override;
    void setStart();
    void setEnd();
    void calculateScore(size_t seqLen);
    void printScore();
    void clear();

    void gameOver();

private:
    int m_current_score = 0;
    lchrono::timepoint m_start, m_end;
};

#endif // SIMON_SCOREMANAGER_H
