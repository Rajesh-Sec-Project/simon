#ifndef LCOMM_SUBSCRIBER_H
#define LCOMM_SUBSCRIBER_H

#include "lcomm/packet.h"

namespace lcomm
{
    class Subscriber
    {
    public:
        Subscriber();
        virtual ~Subscriber();

        virtual void notify(PacketBase const* packet) = 0;
    };
}

#endif // LCOMM_SUBSCRIBER_H
