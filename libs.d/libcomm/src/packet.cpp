#include "lcomm/packet.h"

namespace lcomm
{
    using namespace lconf;

    PacketBase::PacketBase()
    { }

    PacketBase::~PacketBase()
    { }

    json::Node* PacketBase::toJson() const
    {
        return m_tpl.synthetize();
    }

    void PacketBase::fromJson(json::Node* node)
    {
        m_tpl.extract(node);
    }
}
