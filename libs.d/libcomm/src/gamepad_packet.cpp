#include "lcomm/gamepad_packet.h"

namespace lcomm
{
	GamepadPacket::GamepadPacket(lconf::json::Node* node)
	{
		M_setup();
		fromJson(node);
	}

	GamepadPacket::GamepadPacket(GamepadPacket::Keys keys) :
		m_keys(keys)
	{
		M_setup();
	}

	GamepadPacket::~GamepadPacket()
	{ }

	GamepadPacket::Keys GamepadPacket::keys() const
	{
		return m_keys;
	}

	void GamepadPacket::setKeys(GamepadPacket::Keys keys)
	{
		m_keys = keys;
	}

	void GamepadPacket::M_setup()
	{
		bind("keys", (int&) m_keys);
	}
}
