#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include "lcomm/lcomm.h"

class CommManager : public QObject, public lcomm::Subscriber
{
public:
    explicit CommManager(QObject* parent = 0);
    virtual ~CommManager();

private:
    void notify(lcomm::Endpoint* ep, lcomm::PacketBase* packet);

private:
    lcomm::Endpoint* m_ep;
};

#endif // COMMMANAGER_H
