#ifndef LCOMM_PACKET_H
#define LCOMM_PACKET_H

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

        template <typename T>
        void bind(std::string const& name, T& value)
        { m_tpl.bind(name, value); }

        template <typename Derived>
        Derived* downcast() const
        {
            if (Derived::staticTag() == tag())
                return (Derived*) this;
            else
                return 0;
        }

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

#endif // LCOMM_PACKET_H
