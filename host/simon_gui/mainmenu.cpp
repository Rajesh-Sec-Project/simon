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

#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "viewmanager.h"

MainMenu::MainMenu(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::mainmenu) {
    m_ui->setupUi(this);


    // QObject::connect(m_ui->newGameButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->newGameButton, SIGNAL(clicked()), this, SLOT(M_newGame()));
    QObject::connect(m_ui->highScoresButton, SIGNAL(clicked()), this, SLOT(M_highScores()));
    QObject::connect(m_ui->settingsButton, SIGNAL(clicked()), this, SLOT(M_settings()));
    QObject::connect(m_ui->creditsButton, SIGNAL(clicked()), this, SLOT(M_credits()));
    QObject::connect(m_ui->quitButton, SIGNAL(clicked()), this, SLOT(M_quit()));
}

MainMenu::~MainMenu() {
    delete m_ui;
}

void MainMenu::M_newGame() {
    ViewManager::switchToGame();
}

void MainMenu::M_highScores() {
    ViewManager::switchToHighScores();
}

void MainMenu::M_settings() {
    ViewManager::switchToSettings();
}

void MainMenu::M_credits() {
    ViewManager::switchToCredits();
}

void MainMenu::M_quit() {
    ViewManager::closeWindow();
}
