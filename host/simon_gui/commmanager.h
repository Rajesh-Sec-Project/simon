#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include "lcomm/lcomm.h"
#include <QDebug>

class CommManager : public QObject, public lcomm::Subscriber {
    Q_OBJECT

private:
    explicit CommManager();
    virtual ~CommManager();

public:
    static CommManager* self();
    static void destroy();
    bool opened();

    template <typename T>
    void write(T* packet) {
        qDebug() << "write " << packet;
        m_ep->write(packet);
    }

signals:
    void packetReceived(lcomm::Endpoint* ep, lcomm::PacketBase const* packet);

private:
    void notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet);

private:
    lcomm::Endpoint* m_ep;
    static CommManager* m_self;
};

#endif // COMMMANAGER_H
