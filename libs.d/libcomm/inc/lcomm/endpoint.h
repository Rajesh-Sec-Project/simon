#ifndef LCOMM_ENDPOINT_H
#define LCOMM_ENDPOINT_H

#include "lcomm/packet.h"
#include "lcomm/packetfactory.h"
#include "lcomm/socket.h"
#include "lcomm/subscriber.h"
#include <string>
#include <set>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>

namespace lcomm
{
    class Socket;

    //! This class manages the serialization and creation of packets from/to
    //!   an abstract socket interface (\see lcomm::Socket).
    class Endpoint
    {
    public:
        //! Create an endpoint.
        //! \param latency The socket's polling period for reading
        Endpoint(unsigned int latency = 10);

        virtual ~Endpoint();

        //! Register a subscriber instance to be notified of packet
        //!   arrivals.
        //! \param subscriber The subscriber instance to register
        void registerSubscriber(Subscriber* subscriber);

        //! Unregister a subscriber.
        //! \param subscriber The subscriber instance to unregister
        void unregisterSubscriber(Subscriber* subscriber);

        //! Bind this endpoint to a socket interface.
        //! \param socket The socket to bind to
        void bind(Socket* socket);

        //! Send a packet through this endpoint.
        //! \param packet The packet to send through this endpoint
        void write(PacketBase* packet);

    private:
        void M_readThread();
        PacketBase* M_extractPacket(json::Node* node);
        void M_notify(PacketBase* packet);

    private:
        unsigned int m_latency;
        Socket* m_socket;
        std::mutex m_socket_mutex;
        std::atomic<bool> m_read_thread_exit;
        std::exception_ptr m_read_thread_exc;
        std::thread m_read_thread;
        std::set<Subscriber*> m_subscribers;

    private:
        static std::string const m_magic;
        static std::string const m_version;
    };
};

#endif // LCOMM_ENDPOINT_H
