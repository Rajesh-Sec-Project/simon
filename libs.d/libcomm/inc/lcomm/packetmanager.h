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

#ifndef LCOMM_PACKETMANAGER_H
#define LCOMM_PACKETMANAGER_H

#include "lcomm/packet.h"
#include "lcomm/packetfactory.h"
#include <string>
#include <map>

namespace lcomm {
    //! This is a static class (not instanciable) that
    //!   holds all registered packet factories.
    //! For being able to use a user packet class in the comm.
    //!   system, one must call the registerPacketClass() method once.
    class PacketManager {
        PacketManager() = delete;
        PacketManager(PacketManager const&) = delete;
        ~PacketManager() = delete;

    public:
        //! Register a packet class in this endpoint.
        //! Any registered packet class must inherit lcomm::Packet<DerivedClass>.
        //! \tparam T The packet class name to register into the system
        template <typename T>
        static void registerPacketClass() {
            m_packet_factories[T::staticTag()] = &T::factory();
        }

        //! Get a packet factory by tag.
        //! \param tag The tag from which to retrieve the factory
        //! \return The packet factory instance if found, 0 otherwise
        static PacketFactoryBase* getFactoryByTag(std::string const& tag);

    private:
        static std::map<std::string, PacketFactoryBase*> m_packet_factories;
    };
}

#endif // LCOMM_PACKETMANAGER_H
