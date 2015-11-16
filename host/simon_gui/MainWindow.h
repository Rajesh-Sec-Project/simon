#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "gamepad.h"

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

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    std::unique_ptr<GamePad> m_gamepad;
};

#endif // MAINWINDOW_H
