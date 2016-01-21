#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <string>
#include <unistd.h>

#include "moves.h"

std::ostream& operator<<(std::ostream& out, lmoves::tmove value) {
    using namespace lmoves;
    static std::map<tmove, std::string> strings;
    if(strings.size() == 0) {
#define INSERT_ELEMENT(p) strings[tmove::p] = #p
        INSERT_ELEMENT(DOWN);
        INSERT_ELEMENT(UP);
        INSERT_ELEMENT(RIGHT);
        INSERT_ELEMENT(LEFT);
#undef INSERT_ELEMENT
    }

    return out << strings[value];
}


std::ostream& operator<<(std::ostream& out, lmoves::Moves const& seq) {
    using namespace lmoves;
    for(auto m : seq.getSequence()) {
        out << m << ' ';
    }

    return out;
}

namespace lmoves {
    // constructor
    Moves::Moves(size_t seqLen) : m_distrib(0, 3), m_generator(std::random_device{}()) {
        while(seqLen-- != 0) {
            this->addRandomMove();
        }
    }

    // getter
    std::vector<tmove> const& Moves::getSequence() const {
        return this->m_sequence;
    }

    void Moves::clearSequence() {
        this->m_sequence.clear();
    }

    size_t Moves::size() const {
        return m_sequence.size();
    }

    tmove Moves::M_randomMove() {
        return static_cast<tmove>(m_distrib(m_generator));
    }
    void Moves::addMove(tmove m) {
        m_sequence.push_back(m);
    }
    void Moves::addRandomMove() {
        tmove new_move = this->M_randomMove();
        m_sequence.push_back(new_move);
    }
}
