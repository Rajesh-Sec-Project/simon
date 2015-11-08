#ifndef COMM_SUBSCRIBER_H
#define COMM_SUBSCRIBER_H

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

#endif // COMM_SUBSCRIBER_H
