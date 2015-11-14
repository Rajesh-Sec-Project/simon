#include <iostream>

#include "lcomm/lcomm.h"
#include <memory>

class PingPacket : public lcomm::Packet<PingPacket> {
public:
    PingPacket(lconf::json::Node* node) {
        M_setup();

        fromJson(node);
    }

    PingPacket(std::string const& message)
            : m_message(message) {
        M_setup();
    }

    std::string const& message() const {
        return m_message;
    }

private:
    void M_setup() {
        bind("message", m_message);
    }

private:
    std::string m_message;
};

static std::atomic<bool> quit;

class Pinger : public lcomm::Subscriber {
public:
    void notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet) {
        PingPacket* pong = packet->downcast<PingPacket>();
        if(pong) {
            std::cout << pong->message() << std::endl;
        }
    }
};

int main() {
    using namespace lcomm;

    try {
        quit = false;

        PacketManager::registerPacketClass<PingPacket>();

        Endpoint ep(std::make_unique<ClientSocket>("192.168.1.1", 50001));

        Pinger pinger;
        ep.registerSubscriber(pinger);

        // Wait for the client to be opened, otherwise
        //   write() will throw
        for(; !ep.socket().opened();)
            ;

        /*** Send some data ***/

        for(int i = 0; i < 10; ++i) {
            PingPacket ping("ping");
            ep.write(ping);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        PingPacket quit("stop");
        ep.write(quit);

    } catch(std::exception const& exc) {
        std::cerr << "exception: " << exc.what() << std::endl;
        return -1;
    }

    return 0;
}
