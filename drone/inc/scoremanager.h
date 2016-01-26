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
