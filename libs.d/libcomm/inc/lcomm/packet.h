#ifndef LCOMM_PACKET_H
#define LCOMM_PACKET_H

#include "lcomm/packetfactory.h"
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "lconf/json.h"

namespace lcomm {
    using namespace lconf;

    //! This class defined the abstract interface for all packets.
    //! It owns a lconf::json::Template instance meant to be
    //!   setup by implementation classes, that bind their members.
    //! It then defines from/to Json primitives using this template.
    class PacketBase {
    public:
        //! Create a packet instance.
        PacketBase() = default;
        virtual ~PacketBase() = default;

        //! Serialize this packet to lconf::json representation.
        //! \return The serialized packet
        json::Node* toJson() const;

        //! Initialize this packet from lconf::json representation.
        //! \param node The Json tree's root node from which to initialize
        void fromJson(json::Node* node);

        //! Attempt to downcast this instance to a derived class.
        //! This automatically checks for tag mismatch.
        //! \tparam Derived The class name to downcast to
        //! \return 0 if tag mismatch (i.e. wrong cast), (Derived*) this otherwise
        template <typename Derived>
        Derived* downcast() const {
            if(Derived::staticTag() == tag())
                return (Derived*)this;
            else
                return nullptr;
        }

        //! An abstract function returning a string tag
        //!   characteristic of the implementation class
        //!   (must be unique).
        virtual std::string tag() const = 0;

    protected:
        //! Bind a variable to the internal lconf::json::Template instance.
        //! \param name The name of the variable to be encoded
        //! \param ref A reference to the variable to be encoded
        template <typename T>
        void bind(std::string const& name, T& ref) {
            m_tpl.bind(name, ref);
        }

    private:
        json::Template m_tpl;
    };

    //! A helper class for implementing user packet classes.
    //! It cares about generating the unique tag, and also about
    //!   creating the associated packet factory.
    //! \tparam Derived Set this to the implementing class name
    template <typename Derived>
    class Packet : public PacketBase {
    public:
        using PacketBase::PacketBase;

        virtual ~Packet() = default;

        //! Return the packet factory associated with this class.
        //! \return The packet factory instance (unique for each derived class)
        static PacketFactoryBase& factory() {
            return m_factory;
        }

        //! This defines an automatic tag (set to the derived class name).
        //! \return The computed unique tag for the derived class
        static std::string staticTag() {
            char* demangled = abi::__cxa_demangle(typeid(Derived).name(), 0, 0, 0);
            std::string tag = demangled;
            free(demangled);
            return tag;
        }

        //! This implements the PacketBase::tag() virtual function.
        //! \return The computed unique tag for the derived class
        std::string tag() const override {
            return staticTag();
        }

    private:
        static PacketFactory<Derived> m_factory;
    };

    template <typename Derived>
    PacketFactory<Derived> Packet<Derived>::m_factory;
}

#endif // LCOMM_PACKET_H
