#include "MainWindow.h"
#include <QApplication>
#include "commmanager.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    qDebug() << "Waiting for connection...";
    while(!CommManager::self()->opened())
        ;
    qDebug() << "Connected !";

    MainWindow w;
    w.show();

    return a.exec();
}
