#include "navdata.h"
#include "lcontrol/control.h"
#include "lcomm/spoofer.h"

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

#include <stdexcept>
#include <thread>

std::string NavdataController::m_fool_ip = "127.0.0.1";
std::string NavdataController::m_sniff_ip = "127.0.0.2";
int NavdataController::m_sniff_port = 5554;

NavdataController::NavdataController()
        : m_pcap(&NavdataController::M_setupPcap, this)
        , m_available(false) {
    M_initNavdata();
}

NavdataController::~NavdataController() {
    if(m_pcap_handle) {
        pcap_close(m_pcap_handle);
        m_pcap.join();
    }
}

bool NavdataController::available() const {
    return m_available;
}

Navdata NavdataController::grab() {
    m_available = false;
    return m_navdata;
}

void NavdataController::M_setupPcap() {
    char errbuf[128];
    pcap_t* m_pcap_handle = pcap_open_live("lo", 65536, 1, 0, errbuf);

    if(!m_pcap_handle) {
        std::cerr << "Unable to open pcap: " << errbuf << std::endl;
        return;
    }

    M_trace("pcap session started");

    pcap_loop(m_pcap_handle, -1, &M_proxy, reinterpret_cast<unsigned char*>(this));

    M_trace("pcap session finished");
}

void NavdataController::M_sniffed(const pcap_pkthdr* header, const unsigned char* buffer) {
    int size = header->len;

    struct iphdr* iph = (iphdr*)(buffer + sizeof(ethhdr));
    unsigned short iphdrlen = iph->ihl * 4;

    // Filter protocol
    if(iph->protocol != 17)
        return;

    // Get the destination IP address and filter
    sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    if(dest.sin_addr.s_addr != inet_addr(m_sniff_ip.c_str()))
        return;

    // Get the UDP header
    udphdr* udph = (udphdr*)(buffer + iphdrlen + sizeof(ethhdr));

    // Filter out ports
    if(ntohs(udph->dest) != m_sniff_port)
        return;

    // Get the data from the sniffed packet
    int header_size = sizeof(ethhdr) + iphdrlen + sizeof(udph);
    const unsigned char* data = buffer + header_size;

    M_decode(data, size - header_size);
}

void NavdataController::M_proxy(unsigned char* self, const pcap_pkthdr* header, const unsigned char* buffer) {
    NavdataController* that = reinterpret_cast<NavdataController*>(self);
    that->M_sniffed(header, buffer);
}

void NavdataController::M_initNavdata() {
    using namespace lcontrol;

    // First, spoof the navdata server
    char data[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    lcomm::Spoofer spoofer(m_fool_ip, m_sniff_port, m_sniff_ip, m_sniff_port, data, sizeof(data));
    for(int i = 0; i < 2; ++i) {
        spoofer.spoof();
        M_trace("spoof packet sent");
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    // Wait for the bootstrap bit
    for(int tm = 0;;) {
        // Get some navdata
        while(!m_available)
            ;
        Navdata nav = grab();

        // Check flag
        if(nav.header.state & navdata::navdata_bootstrap)
            break;

        if(++tm > 30)
            throw std::runtime_error("NavdataController::M_initNavdata: drone not in bootstrap mode");
    }

    M_trace("drone in bootstrap mode");

    // Setup navdata bits, either demo or option_flags
    Control::config("navdata_demo", "TRUE");
    M_trace("navdata_demo set");

    // Wait for the command_ack bit
    for(int tm = 0;;) {
        // Get some navdata
        while(!m_available)
            ;
        Navdata nav = grab();

        // Check flag
        if(nav.header.state & navdata::command_ack)
            break;

        if(++tm > 30)
            throw std::runtime_error("NavdataController::M_initNavdata: ack not sent !");
    }

    M_trace("got command ack");

    // Clear it, and we're OK !
    Control::ackControl();

    M_trace("ack clear sent, we're good to go !");
}

void NavdataController::M_decode(const unsigned char* data, int size) {
    using namespace navdata;

    // Get the navdata header
    m_navdata.header = *reinterpret_cast<const header*>(data);
    if(m_navdata.header.magic != 0x55667788) {
        std::cerr << "NavdataController::M_decode: bad navdata magic" << std::endl;
        return;
    }

    // Loop on all navdata options
    int pos = sizeof(header);
    while(pos <= size) {
        const option_header* header = reinterpret_cast<const option_header*>(data + pos);

        // Handle navdata options here
        if(header->tag == option_demo) {
            std::cout << "NavdataController::M_decode: god demo" << std::endl;

            m_navdata.demo = *reinterpret_cast<const demo*>(header);
        }

        pos += header->size;
    }

    // Signal availability to other threads
    m_available = true;
}

void NavdataController::M_trace(std::string const& msg) const {
    std::cout << "[NavdataController] " << msg << std::endl;
}
