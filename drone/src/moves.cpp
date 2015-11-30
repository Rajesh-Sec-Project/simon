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
    Moves::Moves(size_t seqLen) {
        while(seqLen-- != 0) {
            this->addRandomMove();
        }
    }

    // getter
    std::vector<tmove> const& Moves::getSequence() const {
        return this->sequence;
    }


    void Moves::clearSequence() {
        this->sequence.clear();
    }

    unsigned int Moves::size() const {
        return sequence.size();
    }

    tmove Moves::M_randomMove() {
        return static_cast<tmove>(rand() % static_cast<int>(tmove::NUM_MOVES));
    }
    void Moves::addMove(tmove m) {
        sequence.push_back(m);
    }
    void Moves::addRandomMove() {
        tmove new_move = this->M_randomMove();
        sequence.push_back(new_move);
    }
}
