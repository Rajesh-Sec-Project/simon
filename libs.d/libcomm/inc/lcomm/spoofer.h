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

#ifndef SIMON_SPOOFER_H
#define SIMON_SPOOFER_H

#include <string>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

namespace lcomm {
    class Spoofer {
    public:
        //! The spoofer will forge UDP packets and send them
        //!   to to:to_port, faking the source address to
        //!   fake:fake_port.
        Spoofer(std::string const& to, int to_port, std::string const& fake, int fake_port, const char* data, int data_len);

        //! Send a spoof packet to the target.
        void spoof();

    private:
        unsigned short M_csum(unsigned short* ptr, int nbytes);

    private:
        std::string m_to;
        int m_to_port;
        std::string m_fake;
        int m_fake_port;

        char m_datagram[4096];
        char* m_data;
        char* m_pseudogram;
        iphdr* m_iph;
        udphdr* m_udph;
        int m_sock;
        sockaddr_in m_sin;
    };
}

#endif // SIMON_SPOOF_H
