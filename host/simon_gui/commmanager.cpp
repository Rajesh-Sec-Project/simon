#include "commmanager.h"
#include <memory>

std::unique_ptr<CommManager> CommManager::m_self;

CommManager::CommManager()
        : QObject(0)
        , m_ep(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001)) {
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
    return m_ep.socket().opened();
}

void CommManager::notify(lcomm::Endpoint& ep, lcomm::PacketBase const& packet) {
    emit packetReceived(ep, packet);
}
