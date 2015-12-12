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
    void M_stop();
    void M_takeOff();
    void M_land();

    void M_positionUp();
    void M_positionDown();
    void M_positionLeft();
    void M_positionRight();

    void M_logLevelChanged(int newIndex);

    void M_receivedLog(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);
    void M_receivedInfo(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);
    void M_receivedScore(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);

private:
    std::unique_ptr<Ui::DebugWindow> m_ui;
    std::unique_ptr<QGraphicsScene> m_scene;
    QGraphicsItem* m_dot;
    std::atomic<lcomm::LogPacket::Level> m_logLevel;
};

#endif // DEBUGWINDOW_H
