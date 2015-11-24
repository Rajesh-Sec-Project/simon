#include "MainWindow.h"
#include <QApplication>
#include "commmanager.h"
#include "lcomm/lcomm.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/log_packet.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    lcomm::PacketManager::registerPacketClass<lcomm::GamepadPacket>();
    lcomm::PacketManager::registerPacketClass<lcomm::LogPacket>();

    qDebug() << "Waiting for connection...";
    /*while(!CommManager::self().opened())
        ;*/
    qDebug() << "Connected !";

    MainWindow w;
    w.show();

    return a.exec();
}
