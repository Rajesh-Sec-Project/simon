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
#include <QStackedWidget>

namespace Ui {
    class GameWindow;
}

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow();

    QStackedWidget& stackedWidget();

private:
    std::unique_ptr<Ui::GameWindow> m_ui;
};

#endif // MAINWINDOW_H
