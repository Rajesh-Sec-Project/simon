#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainmenu)
{
    ui->setupUi(this);


    QObject::connect(m_ui->newGameButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->highScoresButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->settingsButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->quitButton, SIGNAL(clicked()), this, SLOT(M_stop()));
}

MainMenu::~MainMenu()
{
    delete ui;
}
