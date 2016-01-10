#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

namespace Ui {
class mainmenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void M_highScores();
    void M_settings();
    void M_quit();

private:
    Ui::mainmenu *m_ui;
};

#endif // MAINMENU_H
