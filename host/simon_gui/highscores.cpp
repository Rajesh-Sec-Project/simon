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

#include "highscores.h"
#include "ui_highscores.h"
#include "viewmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QString>
#include <QSettings>
#include <string>

using namespace std;
HighScores::HighScores(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::highscores) {
    m_ui->setupUi(this);

    QObject::connect(m_ui->backButton, SIGNAL(clicked()), this, SLOT(M_back()));

    display_Scores();
}

HighScores::~HighScores() {
    delete m_ui;
}


void HighScores::M_back() {
    ViewManager::switchToMainMenu();
}

void HighScores::display_Scores() {
    QSettings settings;
    QString str = settings.value("highScores", "").toString();
    std::istringstream infile;
    infile.str(str.toStdString());

    int i = 0;
    string line, current_name;
    int current_score;
    string temp = "";
    infile >> current_name >> current_score;
    for(i = 0; getline(infile, line); ++i) {
        temp = temp + "<b>" + current_name + "</b>          " + to_string(current_score) + "<br />";
        infile >> current_name >> current_score;
    }
    QString temp2 = QString::fromStdString(temp);
    m_ui->scoreList->setText(temp2);
}
