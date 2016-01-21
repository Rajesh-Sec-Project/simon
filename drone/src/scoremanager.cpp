#include "scoremanager.h"
#include "gamesystem.h"
#include "math.h"
#include "lcomm/score_packet.h"

#include <sstream>

ScoreManager::ScoreManager(GameSystem& system)
        : GameElement(system) {
}

ScoreManager::~ScoreManager() {
}


void ScoreManager::gameInit() {
}

void ScoreManager::gameLoop() {
}

void ScoreManager::setStart() {
    m_start = lchrono::clock();
}

void ScoreManager::setEnd() {
    m_end = lchrono::clock();
}

void ScoreManager::calculateScore(size_t seqLen) {
    float timer = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count() / (float)seqLen;
    float added_score = (float)(100 / log(1 + timer));
    m_current_score += (int)added_score;
}

void ScoreManager::printScore() {
    lcomm::ScorePacket score(m_current_score);
    m_system.endpoint().write(score);
    // M_message(std::to_string(this->m_added_score )) ;
    M_message(std::to_string(m_current_score));
}

void ScoreManager::gameOver() {
    lcomm::ScorePacket score(-1);
    m_system.endpoint().write(score);
    // M_message(std::to_string(this->m_added_score )) ;
    M_message(std::to_string(m_current_score));
}

void ScoreManager::clear() {
    m_current_score = 0;
    this->printScore();
}
// int a = 10;
// char *intStr = itoa(a);
// string str = string(intStr);
