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

class gameview : public QWidget {
    Q_OBJECT

public:
    explicit gameview(QWidget* parent = 0);
    ~gameview();

private slots:
    void M_startPause();
    void M_stop();

    void M_up();
    void M_down();
    void M_left();
    void M_right();
    void M_receivedScore(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);
    void M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet);

private:
    void M_updateState();
    void M_lost();

    std::unique_ptr<Ui::gameview> m_ui;
    GameState m_state = GameState::Stopped;
    QGraphicsScene* m_scene;
    QGraphicsItem* m_dot;
    QGraphicsItem* m_text;
};

#endif // GAMEVIEW_H
