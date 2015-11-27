#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <memory>
#include "lcomm/lcomm.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void M_up();
    void M_down();
    void M_left();
    void M_right();
    void M_stop();
    void M_takeOff();
    void M_land();

    void M_receivedLog(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);
    void M_receivedInfo(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    QGraphicsScene* m_scene;
    QGraphicsItem* m_dot;
};

#endif // MAINWINDOW_H
