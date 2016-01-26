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

#include "lcomm/serversocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>

#include <stdexcept>
#include <chrono>
#include <iostream>
#include <array>
#include "lchrono/chrono.h"

namespace lcomm {
    ServerSocket::ServerSocket(unsigned int port, lchrono::duration latency)
            : m_latency(latency)
            , m_connected_flag(false)
            , m_buf(4096) {
        std::lock_guard<std::mutex> guard(m_fd_mutex);

        // Create the socket descriptor
        m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if(m_fd < 0)
            throw std::runtime_error("lcomm::ServerSocket::ServerSocket: unable to create socket");

        // Listen on localhost
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);

        // Bind the socket
        if(::bind(m_fd, (struct sockaddr*)&addr, sizeof(sockaddr_in)) < 0)
            throw std::runtime_error("lcomm::ServerSocket::ServerSocket: bind failed");

        // 3 is the backlog, i.e. the size of the connection queue
        if(::listen(m_fd, 3) < 0)
            throw std::runtime_error("lcomm::ServerSocket::ServerSocket: listen failed");
    }

    ServerSocket::~ServerSocket() {
        this->close();
    }

    bool ServerSocket::opened() const {
        return m_connected_flag;
    }

    void ServerSocket::write(std::string const& data) const {
        std::lock_guard<std::mutex> guard(m_fd_mutex);

        // Don't forget to append a new line
        std::string raw = data + '\n';
        int size = raw.size();
        int pos = 0;
        int len = 0;
        do {
            size -= len;
            pos += len;
            len = ::write(m_cfd, raw.c_str() + pos, size);
            if(len < 0)
                throw std::runtime_error("lcomm::ServerSocket::write: write failed");
        } while(len != size);
    }

    bool ServerSocket::read(std::string* data) const {
        if(!data || !m_connected_flag)
            return false;

        ssize_t len;
        if((len = ::read(m_cfd, &m_buf[0], m_buf.size())) < 0) {
            if(errno != EWOULDBLOCK && errno != EAGAIN)
                throw std::runtime_error("lcomm::ServerSocket::read: read failed");
        }

        std::string tmp = "";
        for(int i = 0; i < len; ++i)
            tmp += m_buf[i];

        if(tmp.size()) {
            *data = tmp;
            return true;
        }

        return false;
    }

    void ServerSocket::connect() {
        // Listen for incoming connections
        {
            std::lock_guard<std::mutex> guard(m_fd_mutex);
            m_cfd = ::accept(m_fd, 0, 0);
            if(m_cfd < 0)
                throw std::runtime_error("lcomm::ServerSocket::connect: accept failed");
        }

        // Set the socket to be non blocking
        int flags;
        if((flags = fcntl(m_cfd, F_GETFL, 0)) < 0)
            flags = 0;
        if(fcntl(m_cfd, F_SETFL, flags | O_NONBLOCK) < 0)
            throw std::runtime_error("lcomm::ServerSocket::connect: fcntl failed");

        m_connected_flag = true;
    }

    void ServerSocket::close() {
        std::lock_guard<std::mutex> guard(m_fd_mutex);
        if(m_connected_flag) {
            ::close(m_fd);
            m_connected_flag = false;
        }
    }
}
