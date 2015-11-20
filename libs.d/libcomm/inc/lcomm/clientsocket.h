#ifndef LCOMM_CLIENTSOCKET_H
#define LCOMM_CLIENTSOCKET_H

#include "lcomm/socket.h"

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
        ClientSocket(std::string const& ip, unsigned int port, bool tcp = true, std::chrono::nanoseconds latency = 5ms);

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
        std::chrono::nanoseconds m_latency;
        int m_fd;
        sockaddr_in m_addr;
        mutable std::mutex m_fd_mutex;
        mutable std::atomic_bool m_connected_flag;
        bool const m_tcp;
        mutable std::vector<char> m_buf;
    };
}

#endif // LCOMM_CLIENTSOCKET_H
