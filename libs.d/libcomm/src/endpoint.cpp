#include "lcomm/endpoint.h"
#include "lcomm/packetmanager.h"
#include <stdexcept>
#include <sstream>
#include <chrono>

namespace lcomm {
    using namespace lconf;

    std::string const Endpoint::m_magic = "simon";
    std::string const Endpoint::m_version = "01";

    Endpoint::Endpoint(std::unique_ptr<Socket> socket, unsigned int latency)
            : m_latency(latency)
            , m_socket(std::move(socket))
            , m_read_thread_exit(false)
            , m_read_thread_exc(nullptr)
            , m_read_thread(&Endpoint::M_readThread, this) {
    }

    Endpoint::~Endpoint() {
        m_read_thread_exit = true;
        m_read_thread.join();

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

    void Endpoint::write(PacketBase const & packet) {
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

        std::lock_guard<std::mutex> guard(m_socket_mutex);
        m_socket->write(os.str());
    }

    void Endpoint::M_readThread() {
        try {
            // Wait for the endpoint to be bound
            for(; !m_socket && !m_read_thread_exit;)
                ;

            // Loop until asked to exit
            while(m_socket && !m_read_thread_exit) {
                std::string data;
                bool received = false;

                // Try to read some input data
                {
                    std::lock_guard<std::mutex> guard(m_socket_mutex);
                    received = m_socket->read(&data);
                }

                if(received) {
                    std::string magic, tag;
                    json::Node* node;

                    // Parse packet's content
                    try {
                        std::istringstream ss;
                        ss.str(data);
                        node = json::parse(ss);
                    } catch(json::Exception const& exc) {
                        throw std::runtime_error("lcomm::Endpoint::M_readThread: ill-formed packet (lconf exception)");
                    }

                    PacketBase* packet = M_extractPacket(node);
                    delete node;
                    M_notify(packet);
                } else
                    std::this_thread::sleep_for(std::chrono::milliseconds(m_latency));
            }
        } catch(std::exception const& e) {
            std::cerr << "Exception received : " << e.what() << std::endl;
            m_read_thread_exc = std::current_exception();
        }
    }

    PacketBase* Endpoint::M_extractPacket(json::Node* node) {
        // Check if it is an object as expected
        json::ObjectNode* obj = node->downcast<json::ObjectNode>();
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

        PacketBase* packet = factory->create(payload);
        if(!packet)
            throw std::runtime_error("lcomm::Endpoint::M_processReceived: packet creation failed");

        return packet;
    }

    void Endpoint::M_notify(PacketBase* packet) {
        std::vector<std::thread> threads;

        for(auto subscriber : m_subscribers)
            threads.push_back(std::thread(&Subscriber::notify, subscriber, this, packet));

        for(auto& th : threads)
            th.join();

        delete packet;
    }
}
