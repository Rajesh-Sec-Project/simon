/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LCOMM_ENDPOINT_H
#define LCOMM_ENDPOINT_H

#include "lcomm/packet.h"
#include "lcomm/packetfactory.h"
#include "lcomm/socket.h"
#include "lcomm/subscriber.h"
#include "lchrono/chrono.h"
#include <string>
#include <set>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <memory>

using namespace std::literals;

namespace lcomm {
    class Socket;

    //! This class manages the serialization and creation of packets from/to
    //!   an abstract socket interface (\see lcomm::Socket).
    class Endpoint {
    public:
        //! Create an endpoint.
        //! \param latency The socket's polling period for reading
        Endpoint(std::unique_ptr<Socket> socket, lchrono::duration latency = 10ms);

        virtual ~Endpoint();

        //! Register a subscriber instance to be notified of packet
        //!   arrivals.
        //! \param subscriber The subscriber instance to register
        void registerSubscriber(Subscriber& subscriber);

        //! Unregister a subscriber.
        //! \param subscriber The subscriber instance to unregister
        void unregisterSubscriber(Subscriber& subscriber);

        //! Send a packet through this endpoint.
        //! \param packet The packet to send through this endpoint
        void write(PacketBase const& packet);

        Socket const& socket() const;

        std::set<Subscriber*> const& subscribers() const {
            return m_subscribers;
        }

    private:
        void M_readThread();
        std::shared_ptr<PacketBase> M_extractPacket(json::Node& node);
        void M_notify(std::shared_ptr<PacketBase> packet);

    private:
        lchrono::duration m_latency;
        std::unique_ptr<Socket> m_socket;
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
