#ifndef LCOMM_PACKETFACTORY_H
#define LCOMM_PACKETFACTORY_H

#include "lconf/json.h"
#include <memory>

namespace lcomm {
    using namespace lconf;

    class PacketBase;

    //! An abstract interface for packet factories.
    class PacketFactoryBase {
    public:
        //! Create a packet instance with this factory,
        //!   and initialize it with the given Json data.
        //! \param node The lconf::json representation from which
        //!               to initialize the packet
        //! \return The created packet instance
        virtual std::shared_ptr<PacketBase> create(json::Node* node) const = 0;
    };

    //! A generic helper factory for user packet classes.
    //! \tparam Derived The user packet class' name
    template <typename Derived>
    class PacketFactory : public PacketFactoryBase {
    public:
        //! Implements the PacketFactoryBase::create() abstract function.
        //! \param node The json data from which to initialize the newly created packet
        //! \return The newly created packet instance
        std::shared_ptr<PacketBase> create(json::Node* node) const override {
            return std::make_shared<Derived>(node);
        }
    };
}

#endif // LCOMM_PACKETFACTORY_H
