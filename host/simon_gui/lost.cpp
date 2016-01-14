#include "lost.h"
#include "ui_lost.h"
#include "viewmanager.h"
#include <iostream>
#include <fstream>
#include <vector>

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


    ifstream infile("high_scores.txt", ios::app);
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
    string name = m_ui->name->text().toStdString();

    ifstream infile("high_scores.txt");

    if(!infile) {
        cout << "erreur !  \n";
    }

    string current_name, lines;
    int current_score;
    int i;
    // v is a vector that will be used to save actual scores, and add the new one
    vector<structScore> v;
    structScore temp, new_temp;
    new_temp.s_name = name;
    // new_temp.s_score = ViewManager::get_score();
    new_temp.s_score = ViewManager::get_score();

    // infile >>
    bool add = false; // indicates if the new name and score have been added
    if(infile.peek() == std::ifstream::traits_type::eof()) {
        v.push_back(new_temp);
        add = true;
    } else {

        infile >> current_name >> current_score;
        for(i = 0; std::getline(infile, lines); ++i) {

            temp.s_name = current_name;
            temp.s_score = current_score;


            if(current_score < new_temp.s_score and !add) {
                v.push_back(new_temp);
                add = true;
            }

            v.push_back(temp);

            cout << current_name << " " << current_score << endl;
            infile >> current_name >> current_score;
        }
        if(v.size() < 10 and !add) {
            v.push_back(new_temp);
        }
    }


    infile.close();
    ofstream outfile("high_scores.txt");
    if(!outfile) {
        cout << "erreur ecriture fichier! \n";
    }
    for(int i = 0; (unsigned)i < v.size() and i < 10; i++) { // copy all the vector into the file
        // cout << i << v[i].s_name << " "  << '\n';
        outfile << v[i].s_name << " " << v[i].s_score << '\n';
    }
    outfile.close();

    ViewManager::switchToHighScores();
}

Lost::~Lost() {
    delete m_ui;
}
