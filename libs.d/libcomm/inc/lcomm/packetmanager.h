#ifndef LCOMM_PACKETMANAGER_H
#define LCOMM_PACKETMANAGER_H

#include "lcomm/packet.h"
#include "lcomm/packetfactory.h"
#include <string>
#include <map>

namespace lcomm
{
    class PacketManager
    {
        PacketManager() = delete;
        PacketManager(PacketManager const&) = delete;
        ~PacketManager() = delete;

    public:
        //! Register a packet class in this endpoint.
        //! Any packet class must inherit lcomm::Packet<DerivedClass>.
        template <typename T>
        static void registerPacketClass()
        { m_packet_factories[T::staticTag()] = T::factory(); }

        static PacketFactoryBase* getFactoryByTag(std::string const& tag);

    private:
        static std::map<std::string, PacketFactoryBase*> m_packet_factories;
    };
}

#endif // LCOMM_PACKETMANAGER_H
