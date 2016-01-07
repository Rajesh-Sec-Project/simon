#ifndef VIEWMANAGER
#define VIEWMANAGER

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "mainmenu.h"
#include <memory>
#include "gamewindow.h"

class ViewManager {
public:
    static void init(GameWindow &window) {
        m_window = &window;
        m_mainMenu = std::make_unique<MainMenu>();
    }

    static void switchToMainMenu() {
        m_window->setCentralWidget(m_mainMenu.get());
    }

private:
    static std::unique_ptr<MainMenu> m_mainMenu;
    static GameWindow *m_window;
};

#endif // VIEWMANAGER
