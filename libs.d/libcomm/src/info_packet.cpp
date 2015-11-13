#include "lcomm/info_packet.h"

namespace lcomm
{
	InfoPacket::InfoPacket(lconf::json::Node* node)
	{
		M_setup();
		fromJson(node);
	}

	InfoPacket::InfoPacket(std::string const& info) :
		m_info(info)
	{
		M_setup();
	}

	InfoPacket::~InfoPacket()
	{ }

	std::string const& InfoPacket::info() const
	{
		return m_info;
	}

	void InfoPacket::setInfo(std::string const& info)
	{
		m_info = info;
	}

	void InfoPacket::M_setup()
	{
		bind("info", m_info);
	}
}
