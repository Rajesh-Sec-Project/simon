#include <chrono>
#include <thread>
#include <cstdlib>
#include "randomsequence.h"

using namespace std::literals;

RandomSequence::RandomSequence() {
    m_clientComThread = std::thread(&RandomSequence::M_thread, this);
}

RandomSequence::~RandomSequence() {
    m_clientComThread.join();
}


void RandomSequence::stop() {
    this->m_alive = false;
}

void RandomSequence::M_thread() {
    this->m_alive = true;
    while(m_alive) {
        this->m_move.addMove();
        std::this_thread::sleep_for(1000ms);
        std::cout << m_move << std::endl;
    }
}
