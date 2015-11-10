#ifndef LCOMM_PACKETMANAGER_H
#define LCOMM_PACKETMANAGER_H

#include "lcomm/packet.h"
#include "lcomm/packetfactory.h"
#include <string>
#include <map>

namespace lcomm
{
    //! This is a static class (not instanciable) that
    //!   holds all registered packet factories.
    //! For being able to use a user packet class in the comm.
    //!   system, one must call the registerPacketClass() method once.
    class PacketManager
    {
        PacketManager() = delete;
        PacketManager(PacketManager const&) = delete;
        ~PacketManager() = delete;

    public:
        //! Register a packet class in this endpoint.
        //! Any registered packet class must inherit lcomm::Packet<DerivedClass>.
        //! \tparam T The packet class name to register into the system
        template <typename T>
        static void registerPacketClass()
        { m_packet_factories[T::staticTag()] = T::factory(); }

        //! Get a packet factory by tag.
        //! \param tag The tag from which to retrieve the factory
        //! \return The packet factory instance if found, 0 otherwise
        static PacketFactoryBase* getFactoryByTag(std::string const& tag);

    private:
        static std::map<std::string, PacketFactoryBase*> m_packet_factories;
    };
}

#endif // LCOMM_PACKETMANAGER_H
