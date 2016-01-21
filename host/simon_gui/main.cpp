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
    }
    else {
        ViewManager::switchToLoading();
    }
    gw.show();

    return a.exec();
}
