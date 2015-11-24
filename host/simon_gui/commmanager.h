#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include "lcomm/lcomm.h"
#include <QDebug>
#include <memory>

class CommManager : public QObject, public lcomm::Subscriber {
    Q_OBJECT

private:
    friend std::unique_ptr<CommManager>::deleter_type;

    explicit CommManager();
    virtual ~CommManager();

public:
    static CommManager& self();
    static void destroy();
    bool opened();

    void write(lcomm::PacketBase const& packet);

    void reconnect();

signals:
    void connected();
    void disconnected();
    void packetReceived(lcomm::Endpoint& ep, lcomm::PacketBase const& packet);

private:
    void notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) override;
    static std::unique_ptr<CommManager> M_makeCommManager();

private:
    std::unique_ptr<lcomm::Endpoint> m_ep;
    static std::unique_ptr<CommManager> m_self;
};

#endif // COMMMANAGER_H
