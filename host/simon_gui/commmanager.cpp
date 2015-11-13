#include "commmanager.h"
#include <memory>

CommManager::CommManager(QObject* parent) :
    QObject(parent)
{
    m_ep = new lcomm::Endpoint(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001));
}

CommManager::~CommManager()
{
    delete m_ep;
}

void CommManager::notify(lcomm::Endpoint *ep, lcomm::PacketBase *packet)
{

}
