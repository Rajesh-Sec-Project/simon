#ifndef SIMON_RANQOMSEQUENCE_H
#define SIMON_RANQOMSEQUENCE_H

#include <thread>
#include <atomic>
#include "moves.h"


/* This class will produce a random sequence of movements
The only call of the constructor will generate this sequence,
by adding every seconds a new moves to the sequence.
*/
class RandomSequence {
public:
    // constructor
    RandomSequence();

    // destructor
    ~RandomSequence();

    // allows to stop the incrementation of the sequence.
    void stop();

private:
    void M_thread();

private:
    std::atomic_bool m_alive = {false};
    std::thread m_clientComThread;
    lmoves::Moves m_move;
};

#endif // SIMON_RANQOMSEQUENCE_H
