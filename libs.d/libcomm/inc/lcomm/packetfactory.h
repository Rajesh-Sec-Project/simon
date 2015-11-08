#ifndef LCOMM_PACKETFACTORY_H
#define LCOMM_PACKETFACTORY_H

#include "lconf/json.h"

namespace lcomm
{
    using namespace lconf;

    class PacketBase;

    class PacketFactoryBase
    {
    public:
        virtual PacketBase* create(json::Node* node) const = 0;
    };

    template <typename Derived>
    class PacketFactory : public PacketFactoryBase
    {
    public:
        PacketBase* create(json::Node* node) const {
            return new Derived(node);
        }
    };
}

#endif // LCOMM_PACKETFACTORY_H
