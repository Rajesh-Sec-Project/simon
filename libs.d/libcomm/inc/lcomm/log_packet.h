#ifndef LCOMM_LOG_PACKET_H
#define LCOMM_LOG_PACKET_H

#include "lcomm/packet.h"

namespace lcomm {
    class LogPacket : public Packet<LogPacket> {
    public:
        enum Level { Trace = 0x01, Message = 0x02, Warning = 0x03, Error = 0x04 };

    public:
        LogPacket(lconf::json::Node* node);
        LogPacket(Level level, std::string const& message);

        Level level() const;
        std::string const& message() const;

    private:
        void M_setup();

    private:
        Level m_level;
        std::string m_message;
    };
}

#endif // LCOMM_LOG_PACKET_H
