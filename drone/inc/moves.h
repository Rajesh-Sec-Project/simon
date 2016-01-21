#ifndef SIMON_MOVES_H
#define SIMON_MOVES_H

#include <vector>
#include <iostream>
#include <random>

namespace lmoves {
    // An enum representing all the different moves that the drone is able to produce
    enum class tmove { DOWN = 0, UP = 1, RIGHT = 2, LEFT = 3, NUM_MOVES };

    // The Moves class contains a sequence of moves (a list)
    class Moves {
    public:
        // constructs a random sequence with the specified length
        Moves(size_t seqLen = 0);

        // getter of the sequence
        std::vector<tmove> const& getSequence() const;

        // add a random move to the sequence
        void addRandomMove();

        void addMove(tmove m);

        void clearSequence();

        size_t size() const;

    private:
        // return a random move among the moves available in the enum tmove.
        tmove M_randomMove();

        std::uniform_int_distribution<int> m_distrib;
        std::minstd_rand m_generator;

        std::vector<tmove> m_sequence;
    };
}


std::ostream& operator<<(std::ostream& out, lmoves::tmove value);
std::ostream& operator<<(std::ostream& out, lmoves::Moves const& value);

#endif // SIMON_MOVES_H
