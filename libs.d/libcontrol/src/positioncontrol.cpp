//
// Created by remi on 26/11/15.
//

#include "lcontrol/positioncontrol.h"
#include <chrono>
#include <thread>

using namespace lcontrol;

Distance PositionControl::m_x = 0.0f;
Distance PositionControl::m_y = 0.0f;
Distance PositionControl::m_z = 0.0f;
float PositionControl::m_alpha = 20.0f;
int PositionControl::m_move_duration = 500.0f;
int PositionControl::m_counter_move_duration = 250.0f;
std::thread PositionControl::m_motionThread;
std::condition_variable PositionControl::m_motionVariable;
std::mutex PositionControl::m_motionMutex;
std::queue<PositionControl::Motion> PositionControl::m_motionQueue;
std::atomic_bool PositionControl::m_alive;

void PositionControl::left(Distance d) {
    // return xPos and yPOs and zPos
    m_x -= d;
    m_y = 0;
    m_z = 0;
}
void PositionControl::right(Distance d) {
    // return xPos and yPOs and zPos
    m_x += d;
    m_y = 0;
    m_z = 0;
}
void PositionControl::up(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y = 0;
    m_z += d;
}
void PositionControl::down(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y = 0;
    m_z -= d;
}
void PositionControl::front(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y -= d;
    m_z = 0;
}
void PositionControl::back(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y += d;
    m_z = 0;
}

void PositionControl::moveFront() {
    front(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    front(-2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    front(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    front(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    front(2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    front(-m_alpha);
}

void PositionControl::moveBack() {
    back(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    back(-2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    back(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    back(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    back(2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    back(-m_alpha);
}

void PositionControl::moveLeft() {
    left(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    left(-2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    left(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    left(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    left(2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    left(-m_alpha);
}

void PositionControl::moveRight() {
    right(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    right(-2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    right(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    right(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    right(2 * m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    right(-m_alpha);
}

Distance PositionControl::xPos() {
    return m_x;
}

Distance PositionControl::yPos() {
    return m_y;
}
Distance PositionControl::zPos() {
    return m_z;
}

void PositionControl::init() {
    m_alive = true;
    m_motionThread = std::thread(&PositionControl::M_motionThread);
}

void PositionControl::stop() {
    {
        std::lock_guard<std::mutex> lock(m_motionMutex);
        while(m_motionQueue.size()) {
            m_motionQueue.pop();
        }
    }
    m_alive = false;
    m_motionVariable.notify_one();
    m_motionThread.join();
}

void PositionControl::M_motionThread() {
    while(m_alive) {
        Motion motion;
        {
            std::unique_lock<std::mutex> lock(m_motionMutex);
            while(m_motionQueue.empty()) {
                m_motionVariable.wait(lock);
                if(!m_alive) {
                    break;
                }
            }

            motion = m_motionQueue.front();
            m_motionQueue.pop();
        }

        switch(motion) {
            case Motion::Front:
                PositionControl::moveFront();
                break;
            case Motion::Back:
                PositionControl::moveBack();
                break;
            case Motion::Left:
                PositionControl::moveLeft();
                break;
            case Motion::Right:
                PositionControl::moveRight();
                break;
        }
    }
}

void PositionControl::move(Motion m) {
    std::lock_guard<std::mutex> lock(m_motionMutex);
    m_motionQueue.push(m);
    m_motionVariable.notify_one();
}
