#ifndef LCOMM_INFO_PACKET_H
#define LCOMM_INFO_PACKET_H

#include "lcomm/packet.h"
#include <string>

namespace lcomm
{
	class InfoPacket : public Packet<InfoPacket>
	{
	public:
	public:
		InfoPacket(lconf::json::Node* node);
		InfoPacket(std::string const& info);
		~InfoPacket();

		std::string const& info() const;
		void setInfo(std::string const& info);

	private:
		void M_setup();

	private:
		std::string m_info;
	};
}

#endif // LCOMM_INFO_PACKET_H
