/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
