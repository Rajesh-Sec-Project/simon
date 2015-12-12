#ifndef SIMON_SCOREMANAGER_H
#define SIMON_SCOREMANAGER_H

#include "gameelement.h"
#include <ctime>

class GameSystem;

class ScoreManager : public GameElement{
public:
    ScoreManager(GameSystem& system);
    ~ScoreManager();

    void gameInit() override;
    void gameLoop() override;
    void setStart();
    void setEnd();
    void calculateScore();
    void printScore();

private:
	int m_current_score;
    float m_added_score;
    long long m_start;
    long long m_end;

};

#endif // SIMON_SCOREMANAGER_H
