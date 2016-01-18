#include "lcomm/sound_packet.h"

namespace lcomm {
    SoundPacket::SoundPacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    SoundPacket::SoundPacket(SoundPacket::Sound sound)
            : m_sound(sound) {
        M_setup();
    }

    SoundPacket::Sound SoundPacket::sound() const {
        return m_sound;
    }

    void SoundPacket::M_setup() {
        bind("sound", (int&)m_sound);
    }
}
