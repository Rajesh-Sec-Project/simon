#ifndef LCOMM_PID_PACKET_H
#define LCOMM_PID_PACKET_H

#include "lcomm/packet.h"
#include <cstdint>

namespace lcomm {
    class PIDPacket : public Packet<PIDPacket> {
    public:
        PIDPacket(lconf::json::Node* node);
        PIDPacket(float kp_z, float ki_z, float kd_z,
                  float kp, float ki, float kd);

        float kp_z() const;
        float ki_z() const;
        float kd_z() const;

        float kp() const;
        float ki() const;
        float kd() const;

    private:
        void M_setup();

    private:
        float m_kp_z, m_ki_z, m_kd_z;
        float m_kp, m_ki, m_kd;
    };
}

#endif // LCOMM_PID_PACKET_H
