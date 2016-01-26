#include "lost.h"
#include "ui_lost.h"
#include "viewmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <QSettings>

using namespace std;
using namespace Ui;

Lost::Lost(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::lost) {
    m_ui->setupUi(this);

    m_ui->tryAgain->setHidden(true);

    m_ui->name->setHidden(true);
    m_ui->ok->setHidden(true);

    QObject::connect(m_ui->ok, SIGNAL(clicked()), this, SLOT(M_saveName()));


    QSettings settings;
    QString str = settings.value("highScores", "").toString();
    std::istringstream infile;
    infile.str(str.toStdString());

    string line;
    string last_name;
    int last_score;
    int i;
    for(i = 0; std::getline(infile, line); ++i) {
        infile >> last_name >> last_score;
    }
    if(i <= 10 or ViewManager::get_score() > last_score) {
        m_ui->tryAgain->setHidden(true);

        m_ui->name->setHidden(false);
        m_ui->ok->setHidden(false);

    } else {
        m_ui->tryAgain->setHidden(false);
    }
}

void Lost::M_saveName() {
    string current_name, lines;
    int current_score;

    vector<StructScore> scores;
    StructScore temp, new_temp;

    // create the new score to be added to high score file
    new_temp.s_name = m_ui->name->text().toStdString();
    new_temp.s_score = ViewManager::get_score();

    QSettings settings;
    QString str = settings.value("highScores", "").toString();
    std::istringstream infile;
    infile.str(str.toStdString());

    /** copy the all file into one vector, and add at the right place the new name and score's player **/
    bool add = false; // indicates if the new name and score have been added
    if(infile.peek() == std::ifstream::traits_type::eof()) {
        scores.push_back(new_temp);
        add = true;
    } else {
        infile >> current_name >> current_score;

        for(int i = 0; std::getline(infile, lines); ++i) {
            temp.s_name = current_name;
            temp.s_score = current_score;

            if(current_score < new_temp.s_score and !add) {
                scores.push_back(new_temp);
                add = true;
            }

            scores.push_back(temp);

            infile >> current_name >> current_score;
        }

        if(scores.size() < 10 and !add) {
            scores.push_back(new_temp);
        }
    }

    std::ostringstream outfile;
    outfile.str(str.toStdString());

    /** Right into the file the new list of high scores **/
    for(size_t i = 0; i < scores.size() && i < 10; ++i) { // copy all the vector into the file
        outfile << scores[i].s_name << " " << scores[i].s_score << '\n';
    }
    settings.setValue("highScores", QString(outfile.str().c_str()));

    ViewManager::switchToHighScores();
}

Lost::~Lost() {
    delete m_ui;
}
