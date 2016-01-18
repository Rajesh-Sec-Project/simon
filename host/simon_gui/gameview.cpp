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

gameview::gameview(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::gameview) {
    m_ui->setupUi(this);

    m_ui->score->setText("0x0");

    QObject::connect(m_ui->gamepad, SIGNAL(up()), this, SLOT(M_up()));
    QObject::connect(m_ui->gamepad, SIGNAL(down()), this, SLOT(M_down()));
    QObject::connect(m_ui->gamepad, SIGNAL(left()), this, SLOT(M_left()));
    QObject::connect(m_ui->gamepad, SIGNAL(right()), this, SLOT(M_right()));

    QObject::connect(m_ui->stopButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->startPauseButton, SIGNAL(clicked()), this, SLOT(M_startPause()));

    QObject::connect(&CommManager::self(), SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this, SLOT(M_receivedScore(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    m_state = GameState::Stopped;
    M_updateState();
}

gameview::~gameview() {
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
            m_ui->status->setText("Stopped");
            m_ui->startPauseButton->setText("Start");
            m_ui->stopButton->setEnabled(false);
            m_ui->gamepad->hide();
            m_ui->tagDisplay->hide();
            m_ui->score->hide();
            break;
        case GameState::Paused:
            m_ui->status->setText("Paused");
            m_ui->startPauseButton->setText("Resume");
            m_ui->stopButton->setEnabled(true);
            m_ui->gamepad->show();
            m_ui->tagDisplay->show();
            m_ui->score->show();
            break;
        case GameState::Running:
            m_ui->status->setText("Running");
            m_ui->startPauseButton->setText("Pause");
            m_ui->stopButton->setEnabled(true);
            m_ui->gamepad->show();
            m_ui->tagDisplay->show();
            m_ui->score->show();
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

    if(score->getScore() > 0) {
        ViewManager::set_score(score->getScore());
    } else if(score->getScore() == -1) {
        M_lost();
    }
    m_ui->score->setText("0x" + QString::number(score->getScore(), 16));
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
    M_stop();
}
