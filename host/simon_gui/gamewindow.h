#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <memory>
#include "lcomm/lcomm.h"
#include <atomic>
#include "lcomm/log_packet.h"
#include "lcomm/gamecontrol_packet.h"

namespace Ui {
    class GameWindow;
}

using lcomm::GameState;

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow();

private slots:
    void M_startPause();
    void M_stop();

private:
    void M_updateState();

    std::unique_ptr<Ui::GameWindow> m_ui;
    GameState m_state = GameState::Stopped;
};

#endif // MAINWINDOW_H