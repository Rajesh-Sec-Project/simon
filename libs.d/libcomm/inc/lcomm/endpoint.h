#ifndef COMM_ENDPOINT_H
#define COMM_ENDPOINT_H

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

namespace lcomm
{
    class Socket;

    //! The Endpoint class managed reading and writing of packets.
    //! It communicates w/ the outside world via the Socket interface,
    //!   of which an instance must be bound to the endpoint prior to any read
    //!   or write operation.
    //! It sends and receives serialized packets (in the json format). In order to
    //!   be correctly allocated upon receive, any packet class must be registered to
    //!   the endpoint (using the auto-generated factory from comm::Packet<Derived>) with
    //!   the registerPacketClass<T>() function.
    //! It keeps track of several Subscriber instances which are notified of the new packet arrival.
    //! When a new packet is received, for each subscriber in the list their notify() function
    //!   is invoked (in separate threads).
    class Endpoint
    {
    public:
        Endpoint();
        virtual ~Endpoint();

        template <typename T>
        void registerPacketClass()
        { m_packet_factories[T::staticTag()] = T::factory(); }

        void registerSubscriber(Subscriber* subscriber);
        void unregisterSubscriber(Subscriber* subscriber);

        void bind(Socket*);
        void write(PacketBase* packet);

    private:
        void M_readThread();
        PacketBase* M_extractPacket(json::Node* node);
        void M_notify(PacketBase* packet);

    private:
        Socket* m_socket;
        std::mutex m_socket_mutex;
        std::atomic<bool> m_read_thread_exit;
        std::thread m_read_thread;
        std::map<std::string, PacketFactoryBase*> m_packet_factories;
        std::set<Subscriber*> m_subscribers;

    private:
        static std::string const m_magic;
        static std::string const m_version;
    };
};

#endif // COMM_ENDPOINT_H
