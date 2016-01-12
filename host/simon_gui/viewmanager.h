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
    static void init(GameWindow& window) {
        m_window = &window;
        m_mainMenu = std::make_unique<MainMenu>();
        m_highScores = std::make_unique<HighScores>();
        m_lost = std::make_unique<Lost>();

        while(m_window->stackedWidget().count()) {
            m_window->stackedWidget().removeWidget(m_window->stackedWidget().currentWidget());
        }
    }

    static void switchToMainMenu() {
        switchTo(*m_mainMenu);
    }

    static void switchToHighScores() {
        switchTo(*m_highScores);
    }

    static void switchToLost() {
        switchTo(*m_lost);
    }

    static void closeWindow() {
        m_window->close();
    }

    static void set_score(int value) {
        m_score = value;
    }

    static int get_score() {
        return m_score;
    }

    //    static void switchToSettings() {
    //        m_window->setCentralWidget(m_settings.get());
    //    }

private:
    static void switchTo(QWidget &widget) {
        m_window->stackedWidget().removeWidget(m_window->stackedWidget().currentWidget());
        m_window->stackedWidget().addWidget(&widget);
    }

    static std::unique_ptr<MainMenu> m_mainMenu;
    static std::unique_ptr<HighScores> m_highScores;
    static std::unique_ptr<Lost> m_lost;
    // static std::unique_ptr<> m_settings;
    static GameWindow* m_window;
    static int m_score;
};

#endif // VIEWMANAGER
