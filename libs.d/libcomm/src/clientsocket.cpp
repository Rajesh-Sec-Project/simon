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
            , m_init_flag(false)
            , m_exit_flag(false)
            , m_connected_flag(false)
            , m_thread_exc(nullptr)
            , m_thread(&ClientSocket::M_thread, this)
            , m_tcp(tcp) {
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

        // Signal our thread that we're inited
        m_init_flag = true;
    }

    ClientSocket::~ClientSocket() {
        // Signal our thread that we must exit and join
        m_exit_flag = true;
        m_thread.join();

        if(m_thread_exc)
            std::rethrow_exception(m_thread_exc);
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

        std::lock_guard<std::mutex> guard(m_rcv_queue_mutex);
        if(!m_rcv_queue.size())
            return false;

        *data = std::move(m_rcv_queue.front());
        m_rcv_queue.pop();

        return true;
    }

    void ClientSocket::M_thread() {
        try {
            while(!m_init_flag)
                if(m_exit_flag)
                    return;

            // Try to connect to the server
            {
                std::lock_guard<std::mutex> guard(m_fd_mutex);

                // Set the socket to be non blocking
                if(fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL, 0) | O_NONBLOCK) < 0)
                    throw std::runtime_error("lcomm::ClientSocket::M_thread: fcntl failed");

                while(connect(m_fd, (struct sockaddr*)&m_addr, sizeof(m_addr)) < 0) {
                    std::cerr << "lcomm::ClientSocket::M_thread: connect failed. Retrying..." << std::endl;
                    std::this_thread::sleep_for(1s);
                }

                m_connected_flag = true;
            }

            // Allocate input chunk buffer
            constexpr int const chunk_size = 4096;
            std::array<char, chunk_size> chunk;

            std::string data;

            while(!m_exit_flag) {
                ssize_t len;
                if((len = ::read(m_fd, &chunk[0], chunk_size)) < 0) {
                    if(errno != EWOULDBLOCK && errno != EAGAIN)
                        throw std::runtime_error("lcomm::ClientSocket::M_thread: read failed");
                }

                for(ssize_t i = 0; i < len; ++i) {
                    char c = chunk[i];

                    if(c == '\n') {
                        std::lock_guard<std::mutex> guard(m_rcv_queue_mutex);
                        m_rcv_queue.push(std::move(data));
                        data.clear();
                    }

                    data += c;
                }

                std::this_thread::sleep_for(m_latency);
            }

            ::close(m_fd);
            m_connected_flag = false;
        } catch(std::exception const &e) {
            std::cerr << "ClientSocket::M_thread: Received exception! " << e.what() << std::endl;
            m_thread_exc = std::current_exception();
        }
    }
}
