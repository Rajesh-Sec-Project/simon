/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LCOMM_SOCKET_H
#define LCOMM_SOCKET_H

#include <string>

namespace lcomm {
    //! An abstract socket interface, used as a backend
    //!   for the Endpoint class.
    class Socket {
    public:
        virtual ~Socket() = default;

        //! Abstract method for writing some data to the socket.
        //! \param data The data to write to the socket
        virtual void write(std::string const& data) const = 0;

        //! Abstract method for reading data from the socket.
        //! \param data Output parameter for read data.
        //! \return true if succeeded, false otherwise (or if there is no data to be read)
        virtual bool read(std::string* data) const = 0;

        virtual bool opened() const = 0;

        virtual void connect() = 0;

        virtual void close() = 0;
    };
}

#endif // LCOMM_SOCKET_H
