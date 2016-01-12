#include "lcomm/score_packet.h"

namespace lcomm {
    ScorePacket::ScorePacket(lconf::json::Node* node) {
        M_setup();
        fromJson(node);
    }

    ScorePacket::ScorePacket(int scoreValue)
            : m_score(scoreValue) {
        M_setup();
    }

    int ScorePacket::getScore() {
        return m_score;
    }

    void ScorePacket::M_setup() {
        bind("score", m_score);
    }
}