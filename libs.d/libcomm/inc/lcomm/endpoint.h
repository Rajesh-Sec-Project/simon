#ifndef LCOMM_ENDPOINT_H
#define LCOMM_ENDPOINT_H

#include "lcomm/packet.h"
#include "lcomm/packetfactory.h"
#include "lcomm/socket.h"
#include "lcomm/subscriber.h"
#include <string>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>

namespace lcomm
{
    class Socket;

    //! The Endpoint class managed reading and writing of packets.
    //! It communicates w/ the outside world via the Socket interface,
    //!   of which an instance must be bound to the endpoint prior to any read
    //!   or write operation.
    //! It sends and receives serialized packets (in the json format). In order to
    //!   be correctly crafted upon receive, any packet class must be registered to
    //!   the endpoint (using the auto-generated factory from comm::Packet<Derived>) with
    //!   the registerPacketClass<T>() function.
    //! It keeps track of several Subscriber instances which are notified of new packet arrivals.
    //! When a new packet is received, for each subscriber in the list their notify() function
    //!   is invoked (in separate threads).
    class Endpoint
    {
    public:
        //! Create an endpoint.
        Endpoint();
        virtual ~Endpoint();

        //! Register a packet class in this endpoint.
        //! Any packet class must inherit lcomm::Packet<DerivedClass>.
        template <typename T>
        void registerPacketClass()
        { m_packet_factories[T::staticTag()] = T::factory(); }

        //! Register a subscriber instance to be notified of packet
        //!   arrivals.
        void registerSubscriber(Subscriber* subscriber);
        //! Unregister a subscriber.
        void unregisterSubscriber(Subscriber* subscriber);

        //! Bind this endpoint to a socket interface.
        void bind(Socket* socket);

        //! Send a packet through this endpoint.
        void write(PacketBase* packet);

    private:
        void M_readThread();
        PacketBase* M_extractPacket(json::Node* node);
        void M_notify(PacketBase* packet);

    private:
        Socket* m_socket;
        std::mutex m_socket_mutex;
        std::atomic<bool> m_read_thread_exit;
        std::exception_ptr m_read_thread_exc;
        std::thread m_read_thread;
        std::map<std::string, PacketFactoryBase*> m_packet_factories;
        std::set<Subscriber*> m_subscribers;

    private:
        static std::string const m_magic;
        static std::string const m_version;
    };
};

#endif // LCOMM_ENDPOINT_H
