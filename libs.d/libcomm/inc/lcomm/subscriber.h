#ifndef LCOMM_SUBSCRIBER_H
#define LCOMM_SUBSCRIBER_H

namespace lcomm {
    class Endpoint;
    class PacketBase;

    //! An abstract subscriber interface used as
    //!   a frontend for the Endpoint class.
    class Subscriber {
    public:
        virtual ~Subscriber() = default;

        //! An abstract method to be notified about the arrival
        //!   of a new packet.
        //! If you wish to send back another packet to the sender, send it though ep.
        //! \param ep The endpoint from which the packet comes (as subscribers
        //!   may be registered to several endpoints)
        //! \param packet The received packet. Do **not** free this instance,
        //!   as the endpoint is already taking care of this.
        virtual void notify(Endpoint& ep, PacketBase const& packet) = 0;
    };
}

#endif // LCOMM_SUBSCRIBER_H
