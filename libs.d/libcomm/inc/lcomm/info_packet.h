#ifndef LCOMM_INFO_PACKET_H
#define LCOMM_INFO_PACKET_H

#include "lcomm/packet.h"

namespace lcomm {
    class InfoPacket : public Packet<InfoPacket> {
    public:
        enum State { Landed = 0x01, Flying = 0x02, Detection = 0x04 };

    public:
        InfoPacket(lconf::json::Node* node);
        InfoPacket(State state, int detect_x, int detect_y);

        State state() const;
        int detectX() const;
        int detectY() const;

    private:
        void M_setup();

    private:
        State m_state;
        int m_detect_x, m_detect_y;
    };
}

#endif // LCOMM_LOG_PACKET_H
