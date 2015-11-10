#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

#include "lcomm/lcomm.h"

//! A simple packet structure.
class MyPacket : public lcomm::Packet<MyPacket> {
public:
    //! This constructor must be defined, and must call
    //!   fromJson(node) after setting up bindings.
    MyPacket(lconf::json::Node* node) {
        M_setup();

        fromJson(node);
    }

    //! This is a custom constructor (that sets up bindings too).
    MyPacket(std::string const& cmd)
            : m_cmd(cmd) {
        M_setup();
    }

    //! Just a getter.
    std::string const& cmd() const {
        return m_cmd;
    }

    //! Just a setter.
    void setCmd(std::string const& cmd) {
        m_cmd = cmd;
    }

private:
    //! This function sets up bindings.
    void M_setup() {
        //! This call to bind(name, ref) exposes the member 'm_cmd'
        //!   to the serializing system (by reference), meaning
        //!   that it will be preserved during transmission of this packet
        //!   instance.
        bind("cmd", m_cmd);
    }

private:
    std::string m_cmd;
};

//! A simple ping-pong subscriber.
//! Create with pong == false for the pinger and pong == true
//!   for the ponger.
class PingPonger : public lcomm::Subscriber {
public:
    PingPonger(bool pong)
            : m_pong(pong) {
    }

    //! The endpoint calls this function when a packet is received.
    void notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet) {
        // Don't cast directly as it is really unsafe, call the downcast<T>()
        //   function, that checks the tag of the packet.
        MyPacket* ctrl = packet->downcast<MyPacket>();
        if(ctrl) {
            std::cout << ctrl->cmd() << std::endl;

            // If we're a ponger, reply to the packet.
            if(m_pong) {
                MyPacket ctrl("<- pong");
                ep->write(&ctrl);
            }
        }
    }

private:
    bool m_pong;
};

int main() {
    // Just a random port
    int port = 48953;

    using namespace lcomm;

    try {
        // Remember to register our packet to the system
        PacketManager::registerPacketClass<MyPacket>();

        /*** Server side ***/

        // Create the server socket
        ServerSocket* server = new ServerSocket(port);

        // Create and attach the endpoint
        Endpoint* server_ep = new Endpoint();
        server_ep->bind(server);

        // Create the ponger (that receives pings and send pongs)
        PingPonger server_sub(true);
        server_ep->registerSubscriber(&server_sub);

        /*** Client side ***/

        // Wait for the server to be properly init'ed
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Create the client socket
        ClientSocket* client = new ClientSocket("127.0.0.1", port);

        // Create and attach the client endpoint
        Endpoint* client_ep = new Endpoint();
        client_ep->bind(client);

        // Create the pinger subscriber (that receives pongs)
        PingPonger client_sub(false);
        client_ep->registerSubscriber(&client_sub);

        // Wait for the client to be opened, otherwise
        //   write() will throw
        for(; !client->opened();)
            ;

        /*** Send some data ***/

        for(int i = 0; i < 10; ++i) {
            // Create a ping packet and send it through the endpoint
            MyPacket ctrl("-> ping");
            client_ep->write(&ctrl);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Remember to delete endpoints **before** their sockets !
        delete client_ep;
        delete server_ep;

        // Deleting sockets closes all connections
        delete client;
        delete server;
    } catch(std::exception const& exc) {
        std::cerr << "Exception: " << exc.what() << std::endl;
        return -1;
    }

    return 0;
}
