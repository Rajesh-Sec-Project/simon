#ifndef LCOMM_SCORE_PACKET_H
#define LCOMM_SCORE_PACKET_H

#include "lcomm/packet.h"
namespace lcomm {
    class ScorePacket : public Packet<ScorePacket> {
    public:
        ScorePacket(lconf::json::Node* node);
        ScorePacket(int scoreValue);
        int getScore();

    private:
        void M_setup();

    private:
        int m_score;
    };
}
#endif // LCOMM_SCORE_PACKET_H