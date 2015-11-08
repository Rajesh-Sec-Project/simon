#ifndef COMM_PACKET_H
#define COMM_PACKET_H

#include "lcomm/packetfactory.h"
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "lconf/json.h"

namespace lcomm
{
    using namespace lconf;

    class PacketBase
    {
    public:
        PacketBase();
        virtual ~PacketBase();

        json::Node* toJson() const;
        void fromJson(json::Node* node);

        virtual std::string tag() const = 0;

    protected:
        json::Template m_tpl;
    };

    template <typename Derived>
    class Packet : public PacketBase
    {
    public:
        using PacketBase::PacketBase;
        virtual ~Packet()
        { }

        static PacketFactoryBase* factory()
        { return &m_factory; }

        static std::string staticTag()
        {
            char* demangled = abi::__cxa_demangle(typeid(Derived).name(), 0, 0, 0);
            std::string tag = demangled;
            free(demangled);
            return tag;
        }

        std::string tag() const
        { return staticTag(); }

    private:
        static PacketFactory<Derived> m_factory;
    };

    template <typename Derived>
    PacketFactory<Derived> Packet<Derived>::m_factory;
}

#endif // COMM_PACKET_H
