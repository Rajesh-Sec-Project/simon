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

#ifndef LCOMM_SERVERSOCKET_H
#define LCOMM_SERVERSOCKET_H

#include "lcomm/socket.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <chrono>
#include "lchrono/chrono.h"

using namespace std::literals;

namespace lcomm {
    //! This class implements the Socket interface for
    //!   TCP connections (with the unix socket API).
    //! It implements a server socket, accepting (a single) connection
    //!   on the given port.
    //! It queues input data that can be read with the read() method.
    class ServerSocket : public Socket {
    public:
        //! Create a server socket, listening for a connection at
        //!   the given port.
        //! \param port The port to listen to
        //! \param latency The polling period for reading (and therefore queuing) input data
        ServerSocket(unsigned int port, lchrono::duration latency = 5ms);

        //! The socket is closed when destroying the instance.
        virtual ~ServerSocket();

        //! Check if the socket has a current connection opened.
        //! \return Returns true if the socket is opened, false otherwise
        bool opened() const override;

        //! \param data The data to write to the socket
        void write(std::string const& data) const override;

        //! Attempt to read some data from the socket.
        //! \param data Output parameter for read data
        //! \return True if all went well, false otherwise (or if there is nothing to read)
        bool read(std::string* data) const override;

        void connect() override;
        void close() override;

    private:
        lchrono::duration m_latency;
        int m_fd, m_cfd;
        mutable std::mutex m_fd_mutex;
        std::atomic_bool m_connected_flag;
        mutable std::vector<char> m_buf;
    };
}

#endif // LCOMM_SERVERSOCKET_H
