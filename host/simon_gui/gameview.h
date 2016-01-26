/*
 * Copyright (c) 2016 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
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

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "lcomm/lcomm.h"
#include <atomic>
#include "lcomm/log_packet.h"
#include "lcomm/gamecontrol_packet.h"

namespace Ui {
    class gameview;
}

using lcomm::GameState;

class GameView : public QWidget {
    Q_OBJECT

public:
    explicit GameView(QWidget* parent = 0);
    ~GameView();

private slots:
    void M_startPause();
    void M_stop();

    void M_up();
    void M_down();
    void M_left();
    void M_right();
    void M_receivedScore(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);
    void M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet);
    void M_updateUi();
    void M_sendStatusPacket();
    void M_waitProgress1();
    void M_waitProgress2();

private:
    void M_updateState(bool sendStatus = true, bool startDelay = false);
    void M_lost();

    std::unique_ptr<Ui::gameview> m_ui;
    GameState m_state = GameState::Stopped;
    QGraphicsScene* m_scene;
    QGraphicsItem* m_dot;
    QGraphicsItem* m_text;
};

#endif // GAMEVIEW_H
