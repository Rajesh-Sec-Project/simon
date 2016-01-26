#include "gameview.h"
#include "ui_gameview.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "commmanager.h"
#include "lcomm/sound_packet.h"

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QTimer>
#include <sstream>
#include "viewmanager.h"
#include "soundmanager.h"

using namespace std::literals;

GameView::GameView(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::gameview) {
    m_ui->setupUi(this);

    m_ui->score->setText("0x0");

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, m_ui->tagDisplay->width() * 0.90f, m_ui->tagDisplay->height() * 0.90f);
    m_ui->tagDisplay->setScene(m_scene);

    m_dot = m_scene->addEllipse(0, 0, 5, 5, QPen(QColor("black"), 1.0), QBrush(QColor("yellow")));
    m_dot->setFlags(QGraphicsItem::ItemIsMovable);
    m_dot->hide();

    m_text = m_scene->addText("Tag    ?", QFont("ArcadeClassic", 20));
    m_text->setPos(QPointF(m_scene->width() / 2.0f, m_scene->height() / 2.0f));
    m_text->show();

    QObject::connect(&CommManager::self(),
                     SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this,
                     SLOT(M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    QObject::connect(m_ui->gamepad, SIGNAL(up()), this, SLOT(M_up()));
    QObject::connect(m_ui->gamepad, SIGNAL(down()), this, SLOT(M_down()));
    QObject::connect(m_ui->gamepad, SIGNAL(left()), this, SLOT(M_left()));
    QObject::connect(m_ui->gamepad, SIGNAL(right()), this, SLOT(M_right()));

    QObject::connect(m_ui->stopButton, SIGNAL(clicked()), this, SLOT(M_stop()));
    QObject::connect(m_ui->startPauseButton, SIGNAL(clicked()), this, SLOT(M_startPause()));

    QObject::connect(&CommManager::self(),
                     SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this,
                     SLOT(M_receivedScore(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    m_state = GameState::Stopped;
    M_updateState(false);
}

GameView::~GameView() {
}

void GameView::M_startPause() {
    bool delay = false;

    switch(m_state) {
        case GameState::Stopped:
        case GameState::Paused:
            m_state = GameState::Running;
            delay = true;
            break;
        case GameState::Running:
            m_state = GameState::Paused;
            delay = false;
            break;
    }

    M_updateState(true, delay);
}

void GameView::M_stop() {
    m_state = GameState::Stopped;
    M_updateState();
}

void GameView::M_updateState(bool sendStatus, bool startDelay) {
    m_ui->waitProgress->setValue(0);
    m_ui->waitProgress->show();
    m_ui->getReady->setText("Ready    ?");
    m_ui->getReady->show();

    QTimer::singleShot(startDelay ? 1000 : 0, this, SLOT(M_waitProgress1()));
    QTimer::singleShot(startDelay ? 2000 : 0, this, SLOT(M_waitProgress2()));

    QTimer::singleShot(startDelay ? 3000 : 0, this, SLOT(M_updateUi()));

    if(sendStatus) {
        QTimer::singleShot(startDelay ? 3000 : 0, this, SLOT(M_sendStatusPacket()));
    }
}

void GameView::M_waitProgress1() {
    m_ui->waitProgress->setValue(50);
    m_ui->getReady->setText("Set   !");
}

void GameView::M_waitProgress2() {
    m_ui->waitProgress->setValue(100);
    m_ui->getReady->setText("Go   !");
}

void GameView::M_updateUi() {
    switch(m_state) {
        case GameState::Stopped:
            m_ui->status->setText("Stopped");
            m_ui->startPauseButton->setText("Start");
            m_ui->stopButton->setEnabled(false);
            m_ui->gamepad->hide();
            m_ui->tagDisplay->hide();
            m_ui->score->hide();
            m_ui->waitProgress->hide();
            m_ui->getReady->hide();
            break;
        case GameState::Paused:
            m_ui->status->setText("Paused");
            m_ui->startPauseButton->setText("Resume");
            m_ui->stopButton->setEnabled(true);
            m_ui->gamepad->show();
            m_ui->tagDisplay->show();
            m_ui->score->show();
            m_ui->waitProgress->hide();
            m_ui->getReady->hide();
            break;
        case GameState::Running:
            m_ui->status->setText("Running");
            m_ui->startPauseButton->setText("Pause");
            m_ui->stopButton->setEnabled(true);
            m_ui->gamepad->show();
            m_ui->tagDisplay->show();
            m_ui->score->show();
            m_ui->waitProgress->hide();
            m_ui->getReady->hide();
            break;
    }
}

void GameView::M_sendStatusPacket() {
    lcomm::GameControlPacket pkt(m_state);
    CommManager::self().write(pkt);
}

void GameView::M_receivedScore(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
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

void GameView::M_up() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Up);
    CommManager::self().write(pkt);
}

void GameView::M_down() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Down);
    CommManager::self().write(pkt);
}

void GameView::M_left() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Left);
    CommManager::self().write(pkt);
}

void GameView::M_right() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Right);
    CommManager::self().write(pkt);
}

void GameView::M_lost() {
    ViewManager::switchToLost();
    M_stop();
}

void GameView::M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    using namespace lcomm;

    SoundPacket* sound = packet->downcast<SoundPacket>();
    if(sound) {
        std::cout << "Sound: " << sound->sound() << std::endl;
        switch(sound->sound()) {
            case SoundPacket::Win:
                SoundManager::playSound(SoundManager::winSound);
                break;
            case SoundPacket::Loose:
                SoundManager::playSound(SoundManager::loseSound);
                break;
            case SoundPacket::Good:
                SoundManager::playSound(SoundManager::goodSound);
                break;
        }
        return;
    }

    InfoPacket* info = packet->downcast<InfoPacket>();
    if(info && !(info->state() & InfoPacket::AppReady)) {
        if(info->state() & InfoPacket::Detection) {
            float raw_x = 1.0f - ((float)info->detectX()) / 1000.0f;
            float raw_y = ((float)info->detectY()) / 1000.0f;
            QPointF where = QPointF(m_scene->width() * raw_x, m_scene->height() * raw_y);

            m_text->hide();
            m_dot->show();
            m_dot->setPos(where);
        } else {
            m_text->show();
            m_dot->hide();
        }

        return;
    }
}
