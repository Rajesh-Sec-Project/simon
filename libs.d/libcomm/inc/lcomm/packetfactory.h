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
