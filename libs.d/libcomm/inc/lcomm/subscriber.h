#ifndef LCOMM_SUBSCRIBER_H
#define LCOMM_SUBSCRIBER_H

namespace lcomm
{
    class Endpoint;
    class PacketBase;

    //! An abstract subscriber interface used as
    //!   a frontend for the Endpoint class.
    class Subscriber
    {
    public:
        Subscriber();
        virtual ~Subscriber();

        virtual void notify(Endpoint* ep, PacketBase const* packet) = 0;
    };
}

#endif // LCOMM_SUBSCRIBER_H
