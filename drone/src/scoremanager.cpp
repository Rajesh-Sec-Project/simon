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
    m_current_score = 0;
}

void ScoreManager::gameLoop() {
}

void ScoreManager::setStart() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    m_start = (long long)tp.tv_sec * 1000L + tp.tv_usec / 1000;
}

void ScoreManager::setEnd() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    m_end = (long long)tp.tv_sec * 1000L + tp.tv_usec / 1000;
}

void ScoreManager::calculateScore() {
    float timer = (float)(m_end - m_start) / 1000;
    float added_score = (float)(100 / log(1 + timer));
    m_current_score += (int)added_score;
}

void ScoreManager::printScore() {
    lcomm::ScorePacket score(m_current_score);
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
