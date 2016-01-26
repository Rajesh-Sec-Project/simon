/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "debugwindow.h"
#include "ui_debugwindow.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "lcomm/pid_packet.h"
#include "commmanager.h"
#include <iostream>

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <sstream>

using namespace std::literals;

DebugWindow::DebugWindow(QWidget* parent)
        : QMainWindow(parent)
        , m_ui(std::make_unique<Ui::DebugWindow>()) {

    m_ui->setupUi(this);

    m_scene = std::make_unique<QGraphicsScene>(this);
    m_scene->setSceneRect(0, 0, 243, 138);
    m_ui->detections->setScene(m_scene.get());

    m_dot = m_scene->addEllipse(0, 0, 5, 5, QPen(QColor("black"), 1.0), QBrush(QColor("yellow")));
    m_dot->setFlags(QGraphicsItem::ItemIsMovable);
    m_dot->hide();

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    m_ui->log->setCurrentFont(font);

    QObject::connect(m_ui->gamepad, SIGNAL(up()), this, SLOT(M_up()));
    QObject::connect(m_ui->gamepad, SIGNAL(down()), this, SLOT(M_down()));
    QObject::connect(m_ui->gamepad, SIGNAL(left()), this, SLOT(M_left()));
    QObject::connect(m_ui->gamepad, SIGNAL(right()), this, SLOT(M_right()));

    QObject::connect(m_ui->positionControl, SIGNAL(up()), this, SLOT(M_positionUp()));
    QObject::connect(m_ui->positionControl, SIGNAL(down()), this, SLOT(M_positionDown()));
    QObject::connect(m_ui->positionControl, SIGNAL(left()), this, SLOT(M_positionLeft()));
    QObject::connect(m_ui->positionControl, SIGNAL(right()), this, SLOT(M_positionRight()));

    QObject::connect(m_ui->button_takeOff, SIGNAL(clicked()), this, SLOT(M_takeOff()));
    QObject::connect(m_ui->button_land, SIGNAL(clicked()), this, SLOT(M_land()));

    QObject::connect(m_ui->minLogLevel, SIGNAL(currentIndexChanged(int)), this, SLOT(M_logLevelChanged(int)));

    QObject::connect(&CommManager::self(), SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this, SLOT(M_receivedLog(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    QObject::connect(&CommManager::self(), SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this, SLOT(M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    QObject::connect(&CommManager::self(), SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this, SLOT(M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));

    QObject::connect(m_ui->kp_z, SIGNAL(valueChanged(double)), this, SLOT(M_pidUpdated(double)));
    QObject::connect(m_ui->ki_z, SIGNAL(valueChanged(double)), this, SLOT(M_pidUpdated(double)));
    QObject::connect(m_ui->kd_z, SIGNAL(valueChanged(double)), this, SLOT(M_pidUpdated(double)));
    QObject::connect(m_ui->kp, SIGNAL(valueChanged(double)), this, SLOT(M_pidUpdated(double)));
    QObject::connect(m_ui->ki, SIGNAL(valueChanged(double)), this, SLOT(M_pidUpdated(double)));
    QObject::connect(m_ui->kd, SIGNAL(valueChanged(double)), this, SLOT(M_pidUpdated(double)));
}

DebugWindow::~DebugWindow() {
}

void DebugWindow::M_up() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Up);
    CommManager::self().write(pkt);
}

void DebugWindow::M_down() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Down);
    CommManager::self().write(pkt);
}

void DebugWindow::M_left() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Left);
    CommManager::self().write(pkt);
}

void DebugWindow::M_right() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Right);
    CommManager::self().write(pkt);
}

void DebugWindow::M_takeOff() {
    lcomm::GamepadPositionPacket pkt(lcomm::GamepadPositionPacket::TakeOff);
    CommManager::self().write(pkt);
}

void DebugWindow::M_land() {
    lcomm::GamepadPositionPacket pkt(lcomm::GamepadPositionPacket::Land);
    CommManager::self().write(pkt);
}

void DebugWindow::M_positionUp() {
    lcomm::GamepadPositionPacket pkt(lcomm::GamepadPositionPacket::Up);
    CommManager::self().write(pkt);
}

void DebugWindow::M_positionDown() {
    lcomm::GamepadPositionPacket pkt(lcomm::GamepadPositionPacket::Down);
    CommManager::self().write(pkt);
}

void DebugWindow::M_positionLeft() {
    lcomm::GamepadPositionPacket pkt(lcomm::GamepadPositionPacket::Left);
    CommManager::self().write(pkt);
}

void DebugWindow::M_positionRight() {
    lcomm::GamepadPositionPacket pkt(lcomm::GamepadPositionPacket::Right);
    CommManager::self().write(pkt);
}

void DebugWindow::M_pidUpdated(double) {
    using namespace lcomm;
    PIDPacket pkt(m_ui->kp_z->value(), m_ui->ki_z->value(), m_ui->kd_z->value(), m_ui->kp->value(), m_ui->ki->value(),
                  m_ui->kd->value());
    CommManager::self().write(pkt);
}

void DebugWindow::M_logLevelChanged(int newIndex) {
    m_logLevel = static_cast<lcomm::LogPacket::Level>(newIndex);
}

void DebugWindow::M_receivedLog(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    using namespace lcomm;

    LogPacket* log = packet->downcast<LogPacket>();
    if(!log /* || log->level() < m_logLevel*/)
        return;

    QColor text_c;
    int text_fw;
    QString prefix = "";

    switch(log->level()) {
        case LogPacket::Trace:
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

void DebugWindow::M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    using namespace lcomm;

    InfoPacket* info = packet->downcast<InfoPacket>();
    if(!info)
        return;

    if(info->state() & InfoPacket::Landed)
        m_ui->droneStateLabel->setText("Landed");
    else
        m_ui->droneStateLabel->setText("Flying");

    if(info->state() & InfoPacket::Detection) {
        QPointF where = QPointF((243.0f / 2.0f) - info->detectX(), (138.0f / 2.0f) - info->detectY());

        std::ostringstream ss;
        ss << "Yes : " << info->detectX() << ", " << info->detectY();

        m_dot->show();
        m_dot->setPos(where);
        m_ui->detections->update();
        m_ui->detectionsLabel->setText(QString(ss.str().c_str()));

        qreal speed_x = std::max(-1.0f, std::min(1.0f, info->speedX() / 100.0f));
        qreal speed_y = std::max(-1.0f, std::min(1.0f, info->speedY() / 100.0f));

        m_ui->speed_x_pos->setValue(fabs(speed_x) * 100);
        m_ui->speed_y_pos->setValue(fabs(speed_y) * 100);
    } else {
        m_dot->hide();
        m_ui->detectionsLabel->setText("No");
    }
}
