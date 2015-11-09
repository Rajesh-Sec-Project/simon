#ifndef LCOMM_SERVERSOCKET_H
#define LCOMM_SERVERSOCKET_H

#include "lcomm/socket.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <stdexcept>

namespace lcomm
{
    //! This class implements the Socket interface for
    //!   TCP connections (with the unix socket API).
    //! It implements a server socket, accepting (a single) connection
    //!   on the given port.
    class ServerSocket : public Socket
    {
    public:
        //! Create a server socket, listening for a connection at
        //!   the given port.
        ServerSocket(unsigned int port);

        //! The socket is closed when destroying the instance.
        virtual ~ServerSocket();

        //! Check if the socket has a current connection opened.
        bool opened() const;

        //! Write some data to the socket.
        void write(std::string const& data);

        //! Attempt to read some data from the socket.
        //! Returns false if there is no data to be read.
        bool read(std::string* data);

    private:
        void M_thread();

    private:
        int m_fd, m_cfd;
        std::mutex m_fd_mutex;
        std::atomic<bool> m_init_flag, m_exit_flag, m_connected_flag;
        std::exception_ptr m_thread_exc;
        std::thread m_thread;
        std::queue<std::string> m_rcv_queue;
        std::mutex m_rcv_queue_mutex;
    };
}

#endif // LCOMM_SERVERSOCKET_H
