#include "lcomm/packetmanager.h"

namespace lcomm
{
    std::map<std::string, PacketFactoryBase*> PacketManager::m_packet_factories;

    PacketFactoryBase* PacketManager::getFactoryByTag(std::string const& tag)
    {
        auto it = m_packet_factories.find(tag);
        if (it == m_packet_factories.end())
            return 0;
        else
            return it->second;
    }
}
