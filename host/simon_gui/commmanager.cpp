#include "commmanager.h"
#include <memory>

CommManager* CommManager::m_self = 0;

CommManager::CommManager()
        : QObject(0) {
    m_ep = new lcomm::Endpoint(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001));
}

CommManager::~CommManager() {
    delete m_ep;
}

CommManager* CommManager::self() {
    if(!m_self)
        m_self = new CommManager();
    return m_self;
}

void CommManager::destroy() {
    if(m_self)
        delete m_self;
    m_self = 0;
}

bool CommManager::opened() {
    return m_ep->socket().opened();
}

void CommManager::notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) {
    emit packetReceived(ep, packet);
}
