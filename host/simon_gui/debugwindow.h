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

#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <memory>
#include "lcomm/lcomm.h"
#include <atomic>
#include "lcomm/log_packet.h"

namespace Ui {
    class DebugWindow;
}

class DebugWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DebugWindow(QWidget* parent = nullptr);
    ~DebugWindow();

private slots:
    void M_up();
    void M_down();
    void M_left();
    void M_right();
    void M_takeOff();
    void M_land();

    void M_positionUp();
    void M_positionDown();
    void M_positionLeft();
    void M_positionRight();

    void M_logLevelChanged(int newIndex);

    void M_pidUpdated(double);

    void M_receivedLog(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);
    void M_receivedInfo(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);

private:
    std::unique_ptr<Ui::DebugWindow> m_ui;
    std::unique_ptr<QGraphicsScene> m_scene;
    QGraphicsItem* m_dot;
    std::atomic<lcomm::LogPacket::Level> m_logLevel;
};

#endif // DEBUGWINDOW_H
