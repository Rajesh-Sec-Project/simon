#ifndef VIEWMANAGER
#define VIEWMANAGER

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "mainmenu.h"
#include "highscores.h"
#include "lost.h"
#include <memory>
#include "gamewindow.h"
#include <QStackedWidget>

class ViewManager {
public:
    static void init(GameWindow &window) {
        m_window = &window;
        m_mainMenu = std::make_unique<MainMenu>();
        m_highScores = std::make_unique<HighScores>();
        m_lost = std::make_unique<Lost>();
    }

    static void switchToMainMenu() {
        m_window->setCentralWidget(m_mainMenu.get());

    }

    static void switchToHighScores(){

        m_window->setCentralWidget(m_highScores.get());

    }

    static void switchToLost(){
        m_window->setCentralWidget(m_lost.get());
    }

    static void closeWindow(){
       m_window->close();
    }

    static void set_score(int value){
        m_score = value ;
    }

    static int get_score() {
        return m_score;
    }

//    static void switchToSettings() {
//        m_window->setCentralWidget(m_settings.get());
//    }

private:
    static std::unique_ptr<MainMenu> m_mainMenu;
    static std::unique_ptr<HighScores> m_highScores;
    static std::unique_ptr<Lost> m_lost;
    //static std::unique_ptr<> m_settings;
    static GameWindow *m_window;
    static int m_score;
};

#endif // VIEWMANAGER
