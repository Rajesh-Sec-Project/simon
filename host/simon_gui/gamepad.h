#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QWidget>

namespace Ui {
    class GamePad;
}

class GamePad : public QWidget {
    Q_OBJECT

public:
    explicit GamePad(QWidget* parent = 0);
    ~GamePad();

signals:
    void up();
    void down();
    void left();
    void right();
    void stop();
    void takeOff();
    void land();

private slots:
    void M_up();
    void M_down();
    void M_left();
    void M_right();
    void M_stop();
    void M_takeOff();
    void M_land();

private:
    void keyPressEvent(QKeyEvent* evt);

private:
    Ui::GamePad* m_ui;
};

#endif // GAMEPAD_H
