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
