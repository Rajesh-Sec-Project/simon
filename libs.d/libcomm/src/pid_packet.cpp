#include "lcomm/pid_packet.h"
using namespace lcomm;

PIDPacket::PIDPacket(lconf::json::Node* node) {
	M_setup();
	fromJson(node);
}

PIDPacket::PIDPacket(float kp_z, float ki_z, float kd_z, float kp, float ki, float kd)
	: m_kp_z(kp_z)
	, m_ki_z(ki_z)
	, m_kd_z(kd_z)
	, m_kp(kp)
	, m_ki(ki)
	, m_kd(kd)
{
	M_setup();
}

        float PIDPacket::kp_z() const {
        	return m_kp_z;
        }
        float PIDPacket::ki_z() const {
        	return m_ki_z;
        }
        float PIDPacket::kd_z() const {
        	return m_kd_z;
        }

        float PIDPacket::kp() const {
        	return m_kp;
        }
        float PIDPacket::ki() const {
        	return m_ki;
        }
        float PIDPacket::kd() const {
        	return m_kd;
        }

void PIDPacket::M_setup()
{
	bind("kp_z", m_kp_z);
	bind("ki_z", m_ki_z);
	bind("kd_z", m_kd_z);
	bind("kp", m_kp);
	bind("ki", m_ki);
	bind("kd", m_kd);
}
