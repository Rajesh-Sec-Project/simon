/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LCOMM_SUBSCRIBER_H
#define LCOMM_SUBSCRIBER_H

#include <memory>

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
        virtual void notify(Endpoint& ep, std::shared_ptr<PacketBase> packet) = 0;
    };
}

#endif // LCOMM_SUBSCRIBER_H
