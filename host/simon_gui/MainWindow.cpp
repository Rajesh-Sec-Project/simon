#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "commmanager.h"

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <sstream>

using namespace std::literals;

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , m_ui(std::make_unique<Ui::MainWindow>()) {

    m_ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 400, 250);
    m_ui->detections->setScene(m_scene);
    m_dot = m_scene->addEllipse(0, 0, 5, 5, QPen(QColor("yellow")), QBrush(QColor("yellow")));
    m_dot->setFlags(QGraphicsItem::ItemIsMovable);
    m_dot->hide();

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    m_ui->log->setCurrentFont(font);

    QObject::connect(m_ui->gamepad, SIGNAL(up()), this, SLOT(M_up()));
    QObject::connect(m_ui->gamepad, SIGNAL(down()), this, SLOT(M_down()));
    QObject::connect(m_ui->gamepad, SIGNAL(left()), this, SLOT(M_left()));
    QObject::connect(m_ui->gamepad, SIGNAL(right()), this, SLOT(M_right()));
    QObject::connect(m_ui->gamepad, SIGNAL(stop()), this, SLOT(M_stop()));
    QObject::connect(m_ui->gamepad, SIGNAL(takeOff()), this, SLOT(M_takeOff()));
    QObject::connect(m_ui->gamepad, SIGNAL(land()), this, SLOT(M_land()));

    QObject::connect(&CommManager::self(),
                     SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this,
                     SLOT(M_receivedLog(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    QObject::connect(&CommManager::self(),
                     SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this,
                     SLOT(M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));
}

MainWindow::~MainWindow() {
}

void MainWindow::M_up() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Up);
    CommManager::self().write(pkt);
}

void MainWindow::M_down() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Down);
    CommManager::self().write(pkt);
}

void MainWindow::M_left() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Left);
    CommManager::self().write(pkt);
}

void MainWindow::M_right() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Right);
    CommManager::self().write(pkt);
}

void MainWindow::M_stop() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Stop);
    CommManager::self().write(pkt);
    std::this_thread::sleep_for(200ms);
    CommManager::self().reconnect();
}

void MainWindow::M_takeOff() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::TakeOff);
    CommManager::self().write(pkt);
}

void MainWindow::M_land() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Land);
    CommManager::self().write(pkt);
}

void MainWindow::M_receivedLog(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    using namespace lcomm;

    LogPacket* log = packet->downcast<LogPacket>();
    if(!log)
        return;

    QColor text_c;
    int text_fw;
    QString prefix = "";

    switch(log->level()) {
        case LogPacket::Trace:
            /* */ return;
            prefix = "[TRACE] ";
            text_c = QColor("gray");
            text_fw = QFont::Normal;
            break;

        case LogPacket::Message:
            prefix = "[INFO]  ";
            text_c = QColor("green");
            text_fw = QFont::Normal;
            break;

        case LogPacket::Warning:
            prefix = "[WARN]  ";
            text_c = QColor("orange");
            text_fw = QFont::DemiBold;
            break;

        case LogPacket::Error:
            prefix = "[ERROR] ";
            text_c = QColor("red");
            text_fw = QFont::Bold;
            break;

        default:
            prefix = "[?????] ";
            text_c = QColor("violet");
            text_fw = QFont::Bold;
    }

    // Save
    int fw = m_ui->log->fontWeight();
    QColor tc = m_ui->log->textColor();
    // Append
    m_ui->log->setFontWeight(text_fw);
    m_ui->log->setTextColor(text_c);
    m_ui->log->append(prefix + QString(log->message().c_str()));
    // Restore
    m_ui->log->setFontWeight(fw);
    m_ui->log->setTextColor(tc);
}

void MainWindow::M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    using namespace lcomm;

    InfoPacket* info = packet->downcast<InfoPacket>();
    if(!info)
        return;

    if(info->state() & InfoPacket::Landed)
        m_ui->droneStateLabel->setText("Landed");
    else
        m_ui->droneStateLabel->setText("Flying");

    if (info->state() & InfoPacket::Detection)
    {
        QPointF where = QPointF(m_scene->sceneRect().width() - ((qreal) info->detectX() * m_scene->sceneRect().width())  / 1000.0f,
                               ((qreal) info->detectY() * m_scene->sceneRect().height()) / 1000.0f);

        std::ostringstream ss;
        ss << "Yes : " << info->detectX() << ", " << info->detectY();

        m_dot->show();
        m_dot->setPos(where);
        m_ui->detections->update();
        m_ui->detectionsLabel->setText(QString(ss.str().c_str()));
    }
    else
    {
        m_dot->hide();
        m_ui->detectionsLabel->setText("No");
    }
}
