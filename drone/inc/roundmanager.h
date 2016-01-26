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

#ifndef SIMON_ROUNDMANAGER_H
#define SIMON_ROUNDMANAGER_H

#include "gameelement.h"
#include "lcomm/lcomm.h"
#include "moves.h"
#include "scoremanager.h"
#include <chrono>
#include "lchrono/chrono.h"

using namespace std::literals;

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

    void playMove(lmoves::tmove move, lchrono::duration delay = 500ms);
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
