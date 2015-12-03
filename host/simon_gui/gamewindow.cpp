#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "commmanager.h"

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <sstream>

using namespace std::literals;

GameWindow::GameWindow(QWidget* parent)
        : QMainWindow(parent)
        , m_ui(std::make_unique<Ui::GameWindow>()) {

    m_ui->setupUi(this);

    QObject::connect(m_ui->stopButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->startPauseButton, SIGNAL(clicked()), this, SLOT(M_startPause()));

    M_stop();
}

GameWindow::~GameWindow() {
}

void GameWindow::M_startPause() {
    switch(m_state) {
        case GameState::Stopped:
        case GameState::Paused:
            m_state = GameState::Running;
            break;
        case GameState::Running:
            m_state = GameState::Paused;
            break;
    }

    M_updateState();
}

void GameWindow::M_stop() {
    m_state = GameState::Stopped;
    M_updateState();
}

void GameWindow::M_updateState() {
    switch(m_state) {
        case GameState::Stopped:
            m_ui->statusLabel->setText("No game currently running.");
            m_ui->startPauseButton->setText("Start");
            m_ui->stopButton->setEnabled(false);
            break;
        case GameState::Paused:
            m_ui->statusLabel->setText("The current game is paused.");
            m_ui->startPauseButton->setText("Resume");
            m_ui->stopButton->setEnabled(true);
            break;
        case GameState::Running:
            m_ui->statusLabel->setText("The current game is running.");
            m_ui->startPauseButton->setText("Pause");
            m_ui->stopButton->setEnabled(true);
            break;
    }
}