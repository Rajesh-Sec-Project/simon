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

#ifndef LCOMM_CLIENTSOCKET_H
#define LCOMM_CLIENTSOCKET_H

#include "lcomm/socket.h"
#include "lchrono/chrono.h"

#include <netinet/in.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <stdexcept>

#include <chrono>
#include <vector>

using namespace std::literals;

namespace lcomm {
    //! This class implements the lcomm::Socket interface
    //!   for a client TCP socket (using Unix sockets).
    //! It queues input data that can be read with the read() method.
    class ClientSocket : public Socket {
    public:
        //! Create a client socket, and attempt a connection to
        //!   the specified server.
        //! \param ip The IP address of the remote server to connect to
        //! \param port The port to connect to
        //! \param latency The polling period for reading (and therefore queuing) input data
        ClientSocket(std::string const& ip, unsigned int port, bool tcp = true, lchrono::duration latency = 5ms);

        //! Connection is closed and ressources freed at destruction of the socket.
        virtual ~ClientSocket();

        //! Check if socket is still opened.
        //! \return Returns true if the socket is opened, false otherwise
        bool opened() const override;

        //! Write some data in the socket.
        //! \param data The data to write to the socket
        void write(std::string const& data) const override;

        //! Attempt to read data from the socket.
        //! \param data Output parameter for read data
        //! \return True if all went well, false otherwise (or if there is nothing to read)
        bool read(std::string* data) const override;

        void connect() override;
        void close() override;

    private:
        lchrono::duration m_latency;
        int m_fd;
        sockaddr_in m_addr;
        mutable std::mutex m_fd_mutex;
        mutable std::atomic_bool m_connected_flag;
        bool const m_tcp;
        mutable std::vector<char> m_buf;
    };
}

#endif // LCOMM_CLIENTSOCKET_H
