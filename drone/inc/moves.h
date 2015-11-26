#ifndef SIMON_MOVES_H
#define SIMON_MOVES_H

#include <list>
#include <iostream>

// An enum representing all the different moves that the drone is able to produce
enum class tmove { DOWN, UP, RIGHT, LEFT, NUM_MOVES };

// The Moves class contains a sequence of moves (a list)
class Moves {
public:
    // constructs a random sequence with the specified length
    Moves(size_t seqLen = 0);

    // getter of the sequence
    std::list<tmove> const& getSequence() const;

    // add a random move to the sequence
    void addMove();

private:
    // return a random move among the moves available in the enum tmove.
    tmove M_randomMove();

    std::list<tmove> sequence;
};

std::ostream& operator<<(std::ostream& out, tmove value);
std::ostream& operator<<(std::ostream& out, Moves const& value);


#endif // SIMON_MOVES_H
