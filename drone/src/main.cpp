#include <iostream>

#include "lcomm/lcomm.h"

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

class Ponger : public lcomm::Subscriber {
public:
    void notify(lcomm::Endpoint* ep, lcomm::PacketBase const* packet) {
        PingPacket* ping = packet->downcast<PingPacket>();
        if(ping) {
            std::cout << ping->message() << std::endl;
            PingPacket pong("pong");
            ep->write(&pong);

            if(ping->message() == "stop")
                quit = true;
        }
    }
};

int main() {
    using namespace lcomm;

    try {
        quit = false;

        PacketManager::registerPacketClass<PingPacket>();

        Endpoint ep(std::make_unique<ServerSocket>(50001));

        Ponger ponger;
        ep.registerSubscriber(&ponger);

        while(!quit)
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

    } catch(std::exception const& exc) {
        std::cerr << "exception: " << exc.what() << std::endl;
        return -1;
    }

    return 0;
}
