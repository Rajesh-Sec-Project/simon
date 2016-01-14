#include "highscores.h"
#include "ui_highscores.h"
#include "viewmanager.h"
#include <iostream>
#include <fstream>
#include <QString>
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
    ifstream infile("high_scores.txt");
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
