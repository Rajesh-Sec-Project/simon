#include "lcomm/serversocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include <stdexcept>
#include <chrono>
#include <iostream>
#include <array>

namespace lcomm {
    ServerSocket::ServerSocket(unsigned int port, std::chrono::nanoseconds latency)
            : m_latency(latency)
            , m_init_flag(false)
            , m_exit_flag(false)
            , m_connected_flag(false)
            , m_thread_exc(nullptr)
            , m_thread(&ServerSocket::M_thread, this) {
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

        m_init_flag = true;
    }

    ServerSocket::~ServerSocket() {
        m_exit_flag = true;
        m_thread.join();

        if(m_thread_exc)
            std::rethrow_exception(m_thread_exc);
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

        std::lock_guard<std::mutex> guard(m_rcv_queue_mutex);
        if(!m_rcv_queue.size())
            return false;

        *data = std::move(m_rcv_queue.front());
        m_rcv_queue.pop();

        return true;
    }

    void ServerSocket::M_thread() {
        try {
            while(!m_init_flag)
                if(m_exit_flag)
                    return;

            while(!m_exit_flag) {
                // Listen for incoming connections
                {
                    std::lock_guard<std::mutex> guard(m_fd_mutex);
                    m_cfd = ::accept(m_fd, 0, 0);
                    if(m_cfd < 0)
                        throw std::runtime_error("lcomm::ServerSocket::M_thread: accept failed");
                    m_connected_flag = true;
                }

                // Set the socket to be non blocking
                int flags;
                if((flags = fcntl(m_cfd, F_GETFL, 0)) < 0)
                    flags = 0;
                if(fcntl(m_cfd, F_SETFL, flags | O_NONBLOCK) < 0)
                    throw std::runtime_error("lcomm::ServerSocket::M_thread: fcntl failed");

                // Get the socket's buffer size
                constexpr int const chunk_size = 4096;

                // Allocate input buffer
                std::array<char, chunk_size> chunk;

                std::string data;

                while(!m_exit_flag) {
                    int len;
                    if((len = ::read(m_cfd, &chunk[0], chunk_size)) < 0) {
                        if(errno != EWOULDBLOCK && errno != EAGAIN)
                            throw std::runtime_error("lcomm::ServerSocket::M_thread: read failed");
                    }

                    for(int i = 0; i < len; ++i) {
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

                ::close(m_cfd);
                m_connected_flag = false;
            }
        } catch(...) {
            m_thread_exc = std::current_exception();
        }
    }
}
