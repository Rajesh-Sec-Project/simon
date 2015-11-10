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

namespace lcomm {
    ClientSocket::ClientSocket(std::string const& ip, unsigned int port, unsigned int latency)
            : m_latency(latency)
            , m_init_flag(false)
            , m_exit_flag(false)
            , m_connected_flag(false)
            , m_thread_exc(nullptr)
            , m_thread(&ClientSocket::M_thread, this) {
        // Own the socket's descriptor when initializing
        std::lock_guard<std::mutex> guard(m_fd_mutex);

        // Create it
        m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
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

    void ClientSocket::write(std::string const& data) {
        std::lock_guard<std::mutex> guard(m_fd_mutex);

        // Don't forget to append a new line
        std::string raw = data + '\n';
        int size = raw.size();
        int pos = 0;
        int len = 0;
        do {
            size -= len;
            pos += len;
            len = ::write(m_fd, raw.c_str() + pos, size);
            if(len < 0)
                throw std::runtime_error("lcomm::ClientSocket::write: write failed");
        } while(len != size);
    }

    bool ClientSocket::read(std::string* data) {
        if(!data || !m_connected_flag)
            return false;

        std::lock_guard<std::mutex> guard(m_rcv_queue_mutex);
        if(!m_rcv_queue.size())
            return false;

        *data = m_rcv_queue.front();
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
                if(connect(m_fd, (struct sockaddr*)&m_addr, sizeof(m_addr)) < 0)
                    throw std::runtime_error("lcomm::ClientSocket::M_thread: connect failed");
                m_connected_flag = true;
            }

            // Set the socket to be non blocking
            if(fcntl(m_fd, F_SETFL, O_NONBLOCK) < 0)
                throw std::runtime_error("lcomm::ClientSocket::M_thread: fcntl failed");

            // Allocate input chunk buffer
            int const chunk_size = 4096;
            char* chunk = new char[chunk_size];

            std::string data = "";

            for(;;) {
                if(m_exit_flag)
                    break;

                int len;
                if((len = ::read(m_fd, chunk, chunk_size)) < 0) {
                    if(errno != EWOULDBLOCK && errno != EAGAIN)
                        throw std::runtime_error("lcomm::ClientSocket::M_thread: read failed");
                }

                for(int i = 0; i < len; ++i) {
                    char c = chunk[i];

                    if(c == '\n') {
                        std::lock_guard<std::mutex> guard(m_rcv_queue_mutex);
                        m_rcv_queue.push(data);
                        data = "";
                    }

                    data += c;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(m_latency));
            }

            delete[] chunk;
            ::close(m_fd);
            m_connected_flag = false;
        } catch(...) {
            m_thread_exc = std::current_exception();
        }
    }
}
