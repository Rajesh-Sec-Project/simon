#include "lcomm/spoofer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include <stdexcept>

//using namespace lcomm;

    lcomm::Spoofer::Spoofer(std::string const& to, int to_port, std::string const& fake, int fake_port, const char* data, int data_len)
{
    // Create raw IP socket
    m_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    if(m_sock < 0)
    {
        throw std::runtime_error("lcomm::Spoofer::Spoofer: socket() failed: " + std::string(strerror(errno)));
    }

    // Zero out the packet buffer
    memset(m_datagram, 0, 4096);

    // IP header
    m_iph = (iphdr*) m_datagram;

    // UDP header
    m_udph = (udphdr*) (m_datagram + sizeof(ip));

    // Data part
    m_data = m_datagram + sizeof(iphdr) + sizeof(udphdr);
    memcpy(m_data, data, data_len);

    m_sin.sin_family = AF_INET;
    m_sin.sin_port = to_port;
    m_sin.sin_addr.s_addr = inet_addr(to.c_str());

    // Fill in the IP Header
    m_iph->ihl = 5;
    m_iph->version = 4;
    m_iph->tos = 0;
    m_iph->tot_len = sizeof(iphdr) + sizeof(udphdr) + data_len;
    m_iph->id = htonl(54321); // Id of this packet
    m_iph->frag_off = 0;
    m_iph->ttl = 255;
    m_iph->protocol = IPPROTO_UDP;
    m_iph->check = 0; // Set to 0 before calculating checksum
    m_iph->saddr = inet_addr(fake.c_str()); // Spoof the source ip address
    m_iph->daddr = m_sin.sin_addr.s_addr;

    // Ip checksum
    m_iph->check = M_csum((unsigned short*) m_datagram, m_iph->tot_len);

    // UDP header
    m_udph->source = htons(fake_port);
    m_udph->dest = htons(to_port);
    m_udph->len = htons(8 + data_len); // tcp header size
    m_udph->check = 0; // leave checksum 0 now, filled later by pseudo header

    struct pseudo_header {
        u_int32_t source_address;
        u_int32_t dest_address;
        u_int8_t placeholder;
        u_int8_t protocol;
        u_int16_t udp_length;
    } psh;

    // Now the UDP checksum using the pseudo header
    psh.source_address = inet_addr(fake.c_str());
    psh.dest_address = m_sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = htons(sizeof(udphdr) + data_len);

    int psize = sizeof(pseudo_header) + sizeof(udphdr) + data_len;
    m_pseudogram = new char[psize];

    memcpy(m_pseudogram, (char*) &psh, sizeof(pseudo_header));
    memcpy(m_pseudogram + sizeof(pseudo_header), m_udph, sizeof(udphdr) + data_len);

    m_udph->check = M_csum((unsigned short*) m_pseudogram, psize);
}

void lcomm::Spoofer::spoof()
{
    if(sendto(m_sock, m_datagram, m_iph->tot_len, 0, (sockaddr*) &m_sin, sizeof(m_sin)) < 0)
    {
        throw std::runtime_error("lcomm::Spoofer::spoof: sendto() failed: " + std::string(strerror(errno)));
    }
}

unsigned short lcomm::Spoofer::M_csum(unsigned short* ptr, int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum = 0;
    while(nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if(nbytes == 1) {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;

    return (answer);
}
