#ifndef LCOMM_SOCKET_H
#define LCOMM_SOCKET_H

#include <string>

namespace lcomm {
    //! An abstract socket interface, used as a backend
    //!   for the Endpoint class.
    class Socket {
    public:
        Socket();
        virtual ~Socket();

        //! Abstract method for writing some data to the socket.
        //! \param data The data to write to the socket
        virtual void write(std::string const& data) = 0;

        //! Abstract method for reading data from the socket.
        //! \param data Output parameter for read data.
        //! \return true if succeeded, false otherwise (or if there is no data to be read)
        virtual bool read(std::string* data) = 0;

        virtual bool opened() const = 0;

    };
}

#endif // LCOMM_SOCKET_H
