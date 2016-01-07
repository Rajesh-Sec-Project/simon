#ifndef VIEWMANAGER
#define VIEWMANAGER

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "mainmenu.h"

namespace Ui {
    class MainMenu;
}

class ViewManager {
public:
    static void init() {
        m_mainMenu = new MainMenu();
    }

    static QWidget &getMainMenu() {
       return *m_mainMenu;
    }

private:
    static MainMenu *m_mainMenu;
};

#endif // VIEWMANAGER
