#ifndef LCOMM_SOCKET_H
#define LCOMM_SOCKET_H

#include <string>

namespace lcomm
{
    //! An abstract socket interface, used as a backend
    //!   for the Endpoint class.
    class Socket
    {
    public:
        Socket();
        virtual ~Socket();

        virtual void write(std::string const& data) = 0;
        virtual bool read(std::string* data) = 0;
    };
}

#endif // LCOMM_SOCKET_H
