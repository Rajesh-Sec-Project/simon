#include "lcomm/log_packet.h"

namespace lcomm {
    LogPacket::LogPacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    LogPacket::LogPacket(LogPacket::Level level, std::string const& message)
            : m_level(level)
            , m_message(message) {
        M_setup();
    }

    LogPacket::Level LogPacket::level() const {
        return m_level;
    }

    std::string const& LogPacket::message() const {
        return m_message;
    }

    void LogPacket::M_setup() {
        bind("level", (int&)m_level);
        bind("message", m_message);
    }
}
