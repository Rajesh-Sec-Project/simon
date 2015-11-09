#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

#include "lcomm/packet.h"
#include "lcomm/subscriber.h"
#include "lcomm/endpoint.h"
#include "lcomm/serversocket.h"
#include "lcomm/clientsocket.h"

class ControlPacket : public lcomm::Packet<ControlPacket>
{
public:
    ControlPacket(lconf::json::Node* node)
    {
        M_setup();

        fromJson(node);
    }

    ControlPacket(std::string const& cmd) :
        m_cmd(cmd), Packet()
    {
        M_setup();
    }

    std::string const& cmd() const {
        return m_cmd;
    }

    void setCmd(std::string const& cmd) {
        m_cmd = cmd;
    }

private:
    void M_setup()
    {
        bind("cmd", m_cmd);
    }

private:
    std::string m_cmd;
};

class ControlSubscriber : public lcomm::Subscriber
{
public:
    ControlSubscriber(bool pong) :
        m_pong(pong)
    {}

    void notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet)
    {
        ControlPacket* ctrl = packet->downcast<ControlPacket>();
        if (ctrl)
        {
            std::cout << ctrl->cmd() << std::endl;
            if (m_pong)
            {
                ControlPacket ctrl("<- pong");
                ep->write(&ctrl);
            }
        }
    }

private:
    bool m_pong;
};

int main()
{
    int port = 48953;

    using namespace lcomm;

    try
    {
        /// Server side ///

        ServerSocket* server = new ServerSocket(port);

        Endpoint* server_ep = new Endpoint();
        server_ep->registerPacketClass<ControlPacket>();
        server_ep->bind(server);

        ControlSubscriber server_sub(true);
        server_ep->registerSubscriber(&server_sub);

        /// Client side ///

        std::this_thread::sleep_for(std::chrono::seconds(1));

        ClientSocket* client = new ClientSocket("127.0.0.1", port);

        Endpoint* client_ep = new Endpoint();
        client_ep->registerPacketClass<ControlPacket>();
        client_ep->bind(client);

        ControlSubscriber client_sub(false);
        client_ep->registerSubscriber(&client_sub);

        for (; !client->opened(); );

        /// Send some data ///

        for (int i = 0; i < 10; ++i)
        {
            ControlPacket ctrl("-> ping");
            client_ep->write(&ctrl);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        delete client_ep;
        delete server_ep;

        delete client;
        delete server;
    }
    catch (std::exception const& exc)
    {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
