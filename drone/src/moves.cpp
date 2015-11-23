#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <string>
#include <unistd.h>

#include "moves.h"

using namespace lmoves;

// Overloaded operator <<
std::ostream& operator<<(std::ostream& out, const tmove value) {
    static std::map<tmove, std::string> strings;
    if(strings.size() == 0) {
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(DOWN);
        INSERT_ELEMENT(UP);
        INSERT_ELEMENT(RIGHT);
        INSERT_ELEMENT(LEFT);
#undef INSERT_ELEMENT
    }

    return out << strings[value];
}

// constructor
Moves::Moves() {
}

// getter
std::list<tmove> const& Moves::getSequence() const {
    return this->sequence;
}

void Moves::print() const {
    std::list<tmove>::const_iterator i;
    for(i = this->sequence.begin(); i != this->sequence.end(); ++i)
        std::cout << static_cast<tmove>(*i) << " ";
    std::cout << '\n';
}

tmove Moves::random_move() {
    srand(time(NULL));
    return static_cast<tmove>(rand() % NUM_MOVES);
}

void Moves::add_move() {
    tmove new_move = this->random_move();
    sequence.push_back(new_move);
}
