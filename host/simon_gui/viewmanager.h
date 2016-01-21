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
#include "gameview.h"
#include "soundmanager.h"
#include "loadingview.h"
#include "creditsview.h"

class ViewManager {
public:
    static void init(GameWindow& window) {
        m_window = &window;
        m_mainMenu = std::make_unique<MainMenu>();
        m_highScores = std::make_unique<HighScores>();
        m_lost = std::make_unique<Lost>();
        m_game = std::make_unique<GameView>();
        m_credits = std::make_unique<CreditsView>();
        m_loading = std::make_unique<LoadingView>();

        while(m_window->stackedWidget().count()) {
            m_window->stackedWidget().removeWidget(m_window->stackedWidget().currentWidget());
        }
    }

    static void switchToLoading() {
        switchTo(*m_loading);
    }

    static void switchToMainMenu() {
        switchTo(*m_mainMenu);
        SoundManager::playMusic(SoundManager::menuMusic);
    }

    static void switchToHighScores() {
        switchTo(*m_highScores);
        m_highScores->display_Scores();
    }

    static void switchToLost() {
        switchTo(*m_lost);
        SoundManager::playMusic(SoundManager::lostMusic);
    }

    static void switchToGame() {
        switchTo(*m_game);
        SoundManager::playMusic(SoundManager::gameMusic);
    }

    static void switchToCredits() {
        switchTo(*m_credits);
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
    static void switchTo(QWidget& widget) {
        m_window->stackedWidget().removeWidget(m_window->stackedWidget().currentWidget());
        m_window->stackedWidget().addWidget(&widget);
    }

    static std::unique_ptr<LoadingView> m_loading;
    static std::unique_ptr<MainMenu> m_mainMenu;
    static std::unique_ptr<HighScores> m_highScores;
    static std::unique_ptr<Lost> m_lost;
    static std::unique_ptr<GameView> m_game;
    static std::unique_ptr<CreditsView> m_credits;
    // static std::unique_ptr<> m_settings;
    static GameWindow* m_window;
    static int m_score;
};

#endif // VIEWMANAGER
