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
