/*
 * Copyright (c) 2016 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
#include "settingsview.h"

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
        m_settings = std::make_unique<SettingsView>();

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

    static void switchToSettings() {
        switchTo(*m_settings);
    }

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
    static std::unique_ptr<SettingsView> m_settings;
    static GameWindow* m_window;
    static int m_score;
};

#endif // VIEWMANAGER
