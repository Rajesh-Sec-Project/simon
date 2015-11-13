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

private:
    std::unique_ptr<Ui::MainWindow> _ui;
    GamePad* m_gamepad;
};

#endif // MAINWINDOW_H
