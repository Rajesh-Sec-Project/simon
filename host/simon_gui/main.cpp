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
#include "gamewindow.h"
#include <QApplication>
#include "commmanager.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "lcomm/gamecontrol_packet.h"
#include "lcomm/pid_packet.h"
#include "lcomm/sound_packet.h"
#include "viewmanager.h"
#include <QFontDatabase>
#include <QMessageBox>
#include "soundmanager.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    qRegisterMetaType<std::shared_ptr<lcomm::PacketBase>>("std::shared_ptr<lcomm::PacketBase>");

    QCoreApplication::setOrganizationName("RajeshTeam");
    QCoreApplication::setOrganizationDomain("www.insa-toulouse.fr");
    QCoreApplication::setApplicationName("Simon");

    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPositionPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::LogPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::InfoPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::GameControlPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::ScorePacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::PIDPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::SoundPacket>();

    QFontDatabase::addApplicationFont(":/simon/arcade_classic");
    QFontDatabase::addApplicationFont(":/simon/zorque");

    bool noConn = argc > 1 && QString(argv[1]) == "-no_conn";
    if(!noConn) {
        qDebug() << "Waiting for connection...";
        while(!CommManager::self().opened())
            ;
        qDebug() << "Connected !";
    }

    //    DebugWindow dw;
    //    dw.show();

    SoundManager::init();

    GameWindow gw;
    ViewManager::init(gw);

    if(noConn) {
        ViewManager::switchToMainMenu();
    } else {
        ViewManager::switchToLoading();
    }
    gw.show();
    gw.setWindowState(Qt::WindowMaximized);

    return a.exec();
}
