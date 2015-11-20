#include "lcomm/clientsocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <fcntl.h>

#include <chrono>
#include <iostream>
#include <array>

using namespace std::literals;

namespace lcomm {
    ClientSocket::ClientSocket(std::string const& ip, unsigned int port, bool tcp, std::chrono::nanoseconds latency)
            : m_latency(latency)
            , m_connected_flag(false)
            , m_tcp(tcp), m_buf(4096) {
        // Own the socket's descriptor when initializing
        std::lock_guard<std::mutex> guard(m_fd_mutex);

        // Create it
        m_fd = ::socket(AF_INET, tcp ? SOCK_STREAM : SOCK_DGRAM, tcp ? 0 : IPPROTO_UDP);
        if(m_fd < 0)
            throw std::runtime_error("lcomm::ClientSocket::ClientSocket: unable to create socket");

        // Set up the address
        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        m_addr.sin_port = htons(port);
    }

    ClientSocket::~ClientSocket() {
        this->close();
    }

    bool ClientSocket::opened() const {
        return m_connected_flag;
    }

    void ClientSocket::write(std::string const& data) const {
        std::lock_guard<std::mutex> guard(m_fd_mutex);

        try {
            if (m_tcp) {
                // Don't forget to append a new line
                std::string raw = data + '\n';
                int size = raw.size();
                int pos = 0;
                int len = 0;
                do {
                    size -= len;
                    pos += len;
                    len = ::write(m_fd, raw.c_str() + pos, size);
                    if (len < 0)
                        throw std::runtime_error("lcomm::ClientSocket::write: write failed (TCP) "s + std::strerror(errno));
                } while (len != size);
            } else {
                if (sendto(m_fd, data.c_str(), data.size(), 0, (struct sockaddr *) &m_addr, sizeof(m_addr)) == -1) {
                    throw std::runtime_error("lcomm::ClientSocket::write: write failed (UDP) "s + std::strerror(errno));
                }
            }
        }
        catch(...) {
            m_connected_flag = false;
            throw;
        }
    }

    bool ClientSocket::read(std::string* data) const {
        if(!data || !m_connected_flag)
            return false;

        try {
            while(m_connected_flag) {
                ssize_t len;
                if ((len = ::read(m_fd, &m_buf[0], m_buf.size())) < 0) {
                    if (errno != EWOULDBLOCK && errno != EAGAIN)
                        throw std::runtime_error("lcomm::ClientSocket::M_thread: read failed");
                }

                for (ssize_t i = 0; i < len; ++i) {
                    char c = m_buf[i];

                    if (c == '\n') {
                        return true;
                    }

                    *data += c;
                }

                std::this_thread::sleep_for(m_latency);
            }

        } catch(std::exception const &e) {
            std::cerr << "ClientSocket::M_thread: Received exception! " << e.what() << std::endl;
            throw;
        }

        return false;
    }

    void ClientSocket::connect() {
        if(m_tcp) {
            std::lock_guard <std::mutex> guard(m_fd_mutex);

            // Set the socket to be non blocking
            if (fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL, 0) | O_NONBLOCK) < 0)
                throw std::runtime_error("lcomm::ClientSocket::M_thread: fcntl failed");

            while (::connect(m_fd, (struct sockaddr *) &m_addr, sizeof(m_addr)) < 0) {
                std::cerr << "lcomm::ClientSocket::M_thread: connect failed. Retrying..." << std::endl;
                std::this_thread::sleep_for(1s);
            }
        }
        m_connected_flag = true;
    }

    void ClientSocket::close() {
        std::lock_guard<std::mutex> guard(m_fd_mutex);
        if(m_connected_flag) {
            ::close(m_fd);
            m_connected_flag = false;
        }
    }
}
