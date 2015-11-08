#ifndef LCOMM_SOCKET_H
#define LCOMM_SOCKET_H

#include <string>

namespace lcomm
{
    class Socket
    {
    public:
        virtual void write(std::string const& data) = 0;
        virtual bool read(std::string* data) = 0;
    };
}

#endif // LCOMM_SOCKET_H
