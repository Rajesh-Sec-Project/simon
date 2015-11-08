#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "lcomm/packet.h"
#include "lcomm/subscriber.h"
#include "lcomm/endpoint.h"
#include "comm/serversocket.h"

class ControlPacket : public lcomm::Packet<ControlPacket>
{
public:
    ControlPacket(lconf::json::Node* node)
    {
        fromJson(node);
    }

    ControlPacket(std::string const& cmd) :
        m_cmd(cmd), Packet()
    {
        m_tpl.bind("cmd", m_cmd);
    }

    std::string const& cmd() const {
        return m_cmd;
    }

    void setCmd(std::string const& cmd) {
        m_cmd = cmd;
    }

private:
    std::string m_cmd;
};

class ControlSubscriber : public lcomm::Subscriber
{
public:
    void notify(lcomm::PacketBase const* packet)
    {
        std::cout << "Received packet [" << packet->tag() << "]" << std::endl;
    }
};

int main()
{
    using namespace lcomm;
    using namespace comm;

    ServerSocket sock(3333);
    for (; !sock.opened(); );

    Endpoint ep;
    ep.bind(&sock);
    ep.registerPacketClass<ControlPacket>();

    ControlSubscriber sub;
    ep.registerSubscriber(&sub);

    ControlPacket ctrl("command");
    ep.write(&ctrl);

    return 0;
}
