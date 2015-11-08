#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "lcomm/packet.h"
#include "lcomm/subscriber.h"
#include "lcomm/endpoint.h"

using namespace lconf;

class ControlPacket : public lcomm::Packet<ControlPacket>
{
public:
    ControlPacket(json::Node* node)
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
        std::cout << "Received packet [" << packet << "]" << std::endl;
    }
};

int main()
{
    using namespace lcomm;

    Endpoint* ep = new Endpoint();
    ep->registerPacketClass<ControlPacket>();

    ControlSubscriber* s = new ControlSubscriber();
    ep->registerSubscriber(s);

    ControlPacket ctrl("command");
    ep->write(&ctrl);

    return 0;
}
