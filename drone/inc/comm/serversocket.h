#ifndef COMM_SERVERSOCKET_H
#define COMM_SERVERSOCKET_H

#include "lcomm/socket.h"

namespace comm
{
    using namespace lcomm;

    class ServerSocket : public Socket
    {
    public:
        ServerSocket(unsigned int port);
        virtual ~ServerSocket();

        bool opened() const;

        void write(std::string const& data);
        bool read(std::string* data);
    };
}

#endif // COMM_SERVERSOCKET_H
