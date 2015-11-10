#ifndef LCOMM_CLIENTSOCKET_H
#define LCOMM_CLIENTSOCKET_H

#include "lcomm/socket.h"

#include <netinet/in.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <stdexcept>

namespace lcomm
{
    //! This class implements the lcomm::Socket interface
    //!   for a client TCP socket (using Unix sockets).
    class ClientSocket : public Socket
    {
    public:
        //! Create a client socket, and attempt a connection to
        //!   the specified server.
        //! The latency is the polling period in ms for queuing input data (in ms).
        ClientSocket(std::string const& ip, unsigned int port, unsigned int latency = 5);

        //! Connection is closed and ressources freed at destruction of the socket.
        virtual ~ClientSocket();

        //! Check if socket is still opened.
        bool opened() const;

        //! Write some data in the socket.
        void write(std::string const& data);

        //! Attempt to read data from the socket.
        //! Returns false if there is no data to be read.
        bool read(std::string* data);

    private:
        void M_thread();

    private:
        unsigned int m_latency;
        int m_fd;
        sockaddr_in m_addr;
        std::mutex m_fd_mutex;
        std::atomic<bool> m_init_flag, m_exit_flag, m_connected_flag;
        std::exception_ptr m_thread_exc;
        std::thread m_thread;
        std::queue<std::string> m_rcv_queue;
        std::mutex m_rcv_queue_mutex;
    };
}

#endif // LCOMM_CLIENTSOCKET_H
