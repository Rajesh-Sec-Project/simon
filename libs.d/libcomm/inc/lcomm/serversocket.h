#ifndef LCOMM_SERVERSOCKET_H
#define LCOMM_SERVERSOCKET_H

#include "lcomm/socket.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <chrono>

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
        ServerSocket(unsigned int port, std::chrono::nanoseconds latency = 5ms);

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

    private:
        void M_thread();

    private:
        std::chrono::nanoseconds m_latency;
        int m_fd, m_cfd;
        mutable std::mutex m_fd_mutex;
        std::atomic<bool> m_init_flag, m_exit_flag, m_connected_flag;
        std::exception_ptr m_thread_exc;
        std::thread m_thread;
        mutable std::queue<std::string> m_rcv_queue;
        mutable std::mutex m_rcv_queue_mutex;
    };
}

#endif // LCOMM_SERVERSOCKET_H
