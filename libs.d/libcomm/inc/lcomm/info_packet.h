#ifndef LCOMM_INFO_PACKET_H
#define LCOMM_INFO_PACKET_H

#include "lcomm/packet.h"

namespace lcomm {
    class InfoPacket : public Packet<InfoPacket> {
    public:
        enum State { Landed = 0x01, Flying = 0x02 };

    public:
        InfoPacket(lconf::json::Node* node);
        InfoPacket(State state);

        State state() const;

    private:
        void M_setup();

    private:
        State m_state;
    };
}

#endif // LCOMM_LOG_PACKET_H
