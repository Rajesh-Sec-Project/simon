#include "MainWindow.h"
#include <QApplication>
#include "commmanager.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    qRegisterMetaType<std::shared_ptr<lcomm::PacketBase>>("std::shared_ptr<lcomm::PacketBase>");

    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::LogPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::InfoPacket>();

    qDebug() << "Waiting for connection...";
    //while(!CommManager::self().opened())
    //    ;
    qDebug() << "Connected !";

    MainWindow w;
    w.show();

    std::exit(a.exec());
}
