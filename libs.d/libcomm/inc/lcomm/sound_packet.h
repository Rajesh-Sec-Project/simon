#ifndef LCOMM_SOUND_PACKET_H
#define LCOMM_SOUND_PACKET_H

#include "lcomm/packet.h"
#include <cstdint>

namespace lcomm {
    class SoundPacket : public Packet<SoundPacket> {
    public:
        enum Sound : int { Good, Win, Loose };

    public:
        SoundPacket(lconf::json::Node* node);
        SoundPacket(Sound sound);

        Sound sound() const;

    private:
        void M_setup();

    private:
        Sound m_sound;
    };
}

#endif // LCOMM_SOUND_PACKET_H
