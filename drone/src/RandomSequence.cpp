
#include "../inc/RandomSequence.h"
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std::literals;

RandomSequence::RandomSequence() {
    m_clientComThread = std::thread(&RandomSequence::M_clientComThread, this);
}

RandomSequence::~RandomSequence() {
    m_clientComThread.join();
}


void RandomSequence::stop() {
    this->m_alive = false;
}

void RandomSequence::M_clientComThread() {
    this->m_alive = true;
    while(m_alive) {
        this->m_move.add_move();
        std::this_thread::sleep_for(1000ms);
        this->m_move.print();
    }
}
/*
int main(){
    std::system("ulimit -s");

    {
        RandomSequence seq;

    }
    return 0 ;
}*/
