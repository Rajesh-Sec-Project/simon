#include "commmanager.h"
#include <memory>
#include <QDebug>

std::unique_ptr<CommManager> CommManager::m_self;

CommManager::CommManager()
        : QObject(0)
        , m_ep(std::make_unique<lcomm::Endpoint>(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001))) {
    m_ep->registerSubscriber(*this);
}

CommManager::~CommManager() {
}

CommManager& CommManager::self() {
    if(!m_self)
        m_self = M_makeCommManager();
    return *m_self;
}

void CommManager::destroy() {
    m_self = nullptr;
}

std::unique_ptr<CommManager> CommManager::M_makeCommManager() {
    return std::unique_ptr<CommManager>{new CommManager};
}

bool CommManager::opened() {
    return m_ep->socket().opened();
}

void CommManager::notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    emit packetReceived(&ep, packet);
}

void CommManager::reconnect() {
    auto sub = m_ep->subscribers();
    m_ep = std::make_unique<lcomm::Endpoint>(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001));
    for(auto s : sub) {
        m_ep->registerSubscriber(*s);
    }
}

void CommManager::write(lcomm::PacketBase const& packet) {
    qDebug() << "write " << &packet;
    while(true) {
        try {
            m_ep->write(packet);
            break;
        } catch(std::exception const& e) {
            std::cerr << "CommManager::write failed: " << e.what() << std::endl;
            this->reconnect();
        }
    }
}
