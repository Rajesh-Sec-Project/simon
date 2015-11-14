#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "lcomm/gamepad_packet.h"
#include "commmanager.h"
#include <iostream>

using namespace std::literals;

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , _ui(std::make_unique<Ui::MainWindow>()) {
    _ui->setupUi(this);

    m_gamepad = new GamePad(this);
    m_gamepad->show();

    QObject::connect(m_gamepad, SIGNAL(up()), this, SLOT(M_up()));
    QObject::connect(m_gamepad, SIGNAL(down()), this, SLOT(M_down()));
    QObject::connect(m_gamepad, SIGNAL(left()), this, SLOT(M_left()));
    QObject::connect(m_gamepad, SIGNAL(right()), this, SLOT(M_right()));
    QObject::connect(m_gamepad, SIGNAL(stop()), this, SLOT(M_stop()));
    QObject::connect(m_gamepad, SIGNAL(takeOff()), this, SLOT(M_takeOff()));
    QObject::connect(m_gamepad, SIGNAL(land()), this, SLOT(M_land()));
}

MainWindow::~MainWindow() {
    delete m_gamepad;
}

void MainWindow::M_up() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Up);
    CommManager::self()->write(pkt);
}

void MainWindow::M_down() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Down);
    CommManager::self()->write(pkt);
}

void MainWindow::M_left() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Left);
    CommManager::self()->write(pkt);
}

void MainWindow::M_right() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Right);
    CommManager::self()->write(pkt);
}

void MainWindow::M_stop() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Stop);
    CommManager::self()->write(pkt);
}

void MainWindow::M_takeOff() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::TakeOff);
    CommManager::self()->write(pkt);
}

void MainWindow::M_land() {
    lcomm::GamepadPacket pkt(lcomm::GamepadPacket::Land);
    CommManager::self()->write(pkt);
}
