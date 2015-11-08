#include "comm/serversocket.h"

namespace comm
{
    using namespace lcomm;

    ServerSocket::ServerSocket(unsigned int port)
    { }

    ServerSocket::~ServerSocket()
    { }

    bool ServerSocket::opened() const
    { }

    void ServerSocket::write(std::string const& data)
    { }

    bool ServerSocket::read(std::string* data)
    { return false; }
}
