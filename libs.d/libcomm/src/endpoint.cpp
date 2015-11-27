#include "lcomm/endpoint.h"
#include "lcomm/packetmanager.h"
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <queue>

namespace lcomm {
    using namespace lconf;

    std::string const Endpoint::m_magic = "simon";
    std::string const Endpoint::m_version = "01";

    Endpoint::Endpoint(std::unique_ptr<Socket> socket, std::chrono::nanoseconds latency)
            : m_latency(latency)
            , m_socket(std::move(socket))
            , m_read_thread_exit(false)
            , m_read_thread_exc(nullptr)
            , m_read_thread(&Endpoint::M_readThread, this) {
    }

    Endpoint::~Endpoint() {
        m_read_thread_exit = true;
        m_socket->close();
        m_read_thread.join();
        m_socket.reset();

        if(m_read_thread_exc)
            std::rethrow_exception(m_read_thread_exc);
    }

    Socket const& Endpoint::socket() const {
        return *m_socket;
    }

    void Endpoint::registerSubscriber(Subscriber& subscriber) {
        m_subscribers.emplace(&subscriber);
    }

    void Endpoint::unregisterSubscriber(Subscriber& subscriber) {
        m_subscribers.erase(&subscriber);
    }

    void Endpoint::write(PacketBase const& packet) {
        json::Node* node = packet.toJson();
        if(!node)
            throw std::logic_error("lcomm::Endpoint::write: packet serialization failed");

        auto obj = std::make_unique<json::ObjectNode>();
        obj->impl()["magic"] = new json::StringNode(m_magic);
        obj->impl()["version"] = new json::StringNode(m_version);
        obj->impl()["tag"] = new json::StringNode(packet.tag());
        obj->impl()["payload"] = node;

        std::ostringstream os;
        json::serialize(obj.get(), os, false);

        m_socket->write(os.str());
    }

    void Endpoint::M_readThread() {
        try {
            m_socket->connect();

            std::queue<std::string> messages;
            std::string incomplete = "";

            while(!m_read_thread_exit) {
                std::string frag;
                bool ok = m_socket->read(&frag);
                if(!ok)
                    continue;

                frag = incomplete + frag;

                // Split messages by new line and enqueue ready packets
                int start = 0, end = 0;
                while((end = (int)frag.find('\n', start)) != (int)std::string::npos) {
                    messages.push(frag.substr(start, end - start));
                    start = end + 1;
                }

                // Handle the partial command (if any, otherwise this
                //   will clear the incomplete command)
                incomplete = frag.substr(start);

                // Empty the message queue
                while(messages.size()) {
                    std::string msg = messages.front();
                    messages.pop();

                    // Parse packet's content
                    json::Node* node = 0;
                    try {
                        std::istringstream ss;
                        ss.str(msg);
                        node = json::parse(ss);
                    } catch(std::exception const& exc) {
                        std::cerr << "lcomm::Endpoint::M_readThread: ill-formed packet (lconf exception) :" << std::endl;
                        std::cout << "    " << msg << std::endl;
                        // throw std::runtime_error("lcomm::Endpoint::M_readThread: ill-formed packet (lconf exception)");
                    }

                    // Extract and notify
                    if(node) {
                        std::shared_ptr<PacketBase> packet = M_extractPacket(*node);
                        M_notify(packet);
                    }
                }
            }
        } catch(std::exception const& e) {
            std::cerr << "Exception received : " << e.what() << std::endl;
            m_read_thread_exc = std::current_exception();
        }
    }

    std::shared_ptr<PacketBase> Endpoint::M_extractPacket(json::Node& node) {
        // Check if it is an object as expected
        json::ObjectNode* obj = node.downcast<json::ObjectNode>();
        if(!obj)
            throw std::runtime_error("lcomm::Endpoint::M_processReceived: ill-formed packet (not an object)");

        // Check magic field
        bool good = true;
        if(obj->exists("magic")) {
            json::StringNode* n = obj->get("magic")->downcast<json::StringNode>();
            if(!n || n->value() != m_magic)
                good = false;
        } else
            good = false;

        if(!good)
            throw std::runtime_error("lcomm::Endpoint::M_processReceived: ill-formed packet (invalid magic field)");

        // Check version field
        good = true;
        if(obj->exists("version")) {
            json::StringNode* n = obj->get("version")->downcast<json::StringNode>();
            if(!n || n->value() != m_version)
                good = false;
        } else
            good = false;

        if(!good)
            throw std::runtime_error(
                "lcomm::Endpoint::M_processReceived: ill-formed packet (invalid or incompatible version)");

        // Retrieve packet tag
        good = true;
        std::string tag;
        if(obj->exists("tag")) {
            json::StringNode* n = obj->get("tag")->downcast<json::StringNode>();
            if(!n)
                good = false;
            else
                tag = n->value();
        } else
            good = false;

        if(!good)
            throw std::runtime_error("lcomm::Endpoint::M_processReceived: ill-formed packet (invalid tag field)");

        // Retrieve packet's payload
        json::Node* payload;
        if(obj->exists("payload"))
            payload = obj->get("payload");
        else
            throw std::runtime_error("lcomm::Endpoint::M_processReceived: ill-formed packet (invalid tag field)");

        // Find the appropriate packet factory
        PacketFactoryBase* factory = PacketManager::getFactoryByTag(tag);
        if(!factory)
            throw std::runtime_error("lcomm:Endpoint::M_processReceived: packet tag '" + tag +
                                     "' has not been registered");

        auto packet = factory->create(payload);
        if(!packet)
            throw std::runtime_error("lcomm::Endpoint::M_processReceived: packet creation failed");

        return packet;
    }

    void Endpoint::M_notify(std::shared_ptr<PacketBase> packet) {
        for(auto subscriber : m_subscribers) {
            subscriber->notify(*this, packet);
        }
    }
}
