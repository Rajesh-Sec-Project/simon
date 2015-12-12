#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QWidget>
#include <memory>

namespace Ui {
    class GamePad;
}

class GamePad : public QWidget {
    Q_OBJECT

public:
    explicit GamePad(QWidget* parent = nullptr);
    ~GamePad();

signals:
    void up();
    void down();
    void left();
    void right();
    void stop();

private slots:
    void M_up();
    void M_down();
    void M_left();
    void M_right();
    void M_stop();

protected:
    void keyPressEvent(QKeyEvent* evt) override;

protected:
    std::unique_ptr<Ui::GamePad> m_ui;
};

#endif // GAMEPAD_H
