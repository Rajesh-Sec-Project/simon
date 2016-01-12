#include "gameview.h"
#include "ui_gameview.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "commmanager.h"

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <sstream>
#include "viewmanager.h"

using namespace std::literals;

gameview::gameview(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::gameview)
{
    m_ui->setupUi(this);

    m_ui->score->setText("0");

    QObject::connect(m_ui->gamepad, SIGNAL(up()), this, SLOT(M_up()));
    QObject::connect(m_ui->gamepad, SIGNAL(down()), this, SLOT(M_down()));
    QObject::connect(m_ui->gamepad, SIGNAL(left()), this, SLOT(M_left()));
    QObject::connect(m_ui->gamepad, SIGNAL(right()), this, SLOT(M_right()));

    QObject::connect(m_ui->stopButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->startPauseButton, SIGNAL(clicked()), this, SLOT(M_startPause()));
}

gameview::~gameview()
{
}

void gameview::M_startPause() {
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

void gameview::M_stop() {
    m_state = GameState::Stopped;
    M_updateState();
}

void gameview::M_updateState() {
    switch(m_state) {
        case GameState::Stopped:
            m_ui->status->setText("No game currently running.");
            m_ui->startPauseButton->setText("Start");
            m_ui->stopButton->setEnabled(false);
            break;
        case GameState::Paused:
            m_ui->status->setText("The current game is paused.");
            m_ui->startPauseButton->setText("Resume");
            m_ui->stopButton->setEnabled(true);
            break;
        case GameState::Running:
            m_ui->status->setText("The current game is running.");
            m_ui->startPauseButton->setText("Pause");
            m_ui->stopButton->setEnabled(true);
            break;
    }

    lcomm::GameControlPacket pkt(m_state);
    CommManager::self().write(pkt);
}

void gameview::M_receivedScore(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    using namespace lcomm;

    ScorePacket* score = packet->downcast<ScorePacket>();
    if(!score)
        return;

    if(score->getScore() != -1) {
        ViewManager::set_score(score->getScore());
    }
    m_ui->score->setText(std::to_string(score->getScore()).c_str());
}

void gameview::M_up() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Up);
    CommManager::self().write(pkt);
}

void gameview::M_down() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Down);
    CommManager::self().write(pkt);
}

void gameview::M_left() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Left);
    CommManager::self().write(pkt);
}

void gameview::M_right() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Right);
    CommManager::self().write(pkt);
}

void gameview::M_lost() {
    ViewManager::switchToLost();
}
