#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "viewmanager.h"

MainMenu::MainMenu(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::mainmenu) {
    m_ui->setupUi(this);


    // QObject::connect(m_ui->newGameButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->highScoresButton, SIGNAL(clicked()), this, SLOT(M_highScores()));
    QObject::connect(m_ui->settingsButton, SIGNAL(clicked()), this, SLOT(M_settings()));
    QObject::connect(m_ui->quitButton, SIGNAL(clicked()), this, SLOT(M_quit()));
}

MainMenu::~MainMenu() {
    delete m_ui;
}

void MainMenu::M_highScores() {
    ViewManager::switchToHighScores();
}

void MainMenu::M_settings() {
    // ViewManager::switchToSettings();
}

void MainMenu::M_quit() {
    ViewManager::closeWindow();
}
