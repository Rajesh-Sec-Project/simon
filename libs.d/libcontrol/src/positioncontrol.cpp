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

//
// Created by remi on 26/11/15.
//

#include "lcontrol/positioncontrol.h"
#include <chrono>
#include <thread>

using namespace lcontrol;

using namespace std::literals;

Distance PositionControl::m_x = 0.0f;
Distance PositionControl::m_y = 0.0f;
Distance PositionControl::m_z = 0.0f;

float PositionControl::m_alpha = 20.0f;
lchrono::duration PositionControl::m_move_duration_lr = 750ms;
lchrono::duration PositionControl::m_counter_move_duration_lr = 150ms;
lchrono::duration PositionControl::m_move_duration_front = 1000ms;
lchrono::duration PositionControl::m_counter_move_duration_front = 400ms;
lchrono::duration PositionControl::m_move_duration_back = 750ms;
lchrono::duration PositionControl::m_counter_move_duration_back = 800ms;

std::thread PositionControl::m_motionThread;
std::condition_variable PositionControl::m_motionVariable;
std::mutex PositionControl::m_motionMutex;
std::queue<PositionControl::Motion> PositionControl::m_motionQueue;
std::atomic_bool PositionControl::m_alive;

void PositionControl::leftRight(Distance d) {
    m_x = d;
    m_y = 0;
    m_z = 0;
}
void PositionControl::upDown(Distance d) {
    m_x = 0;
    m_y = 0;
    m_z = d;
}
void PositionControl::frontBack(Distance d) {
    m_x = 0;
    m_y = d;
    m_z = 0;
}

void PositionControl::moveFront() {
    frontBack(m_alpha);
    std::this_thread::sleep_for(m_move_duration_front);
    frontBack(-m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_front + m_move_duration_back);
    frontBack(m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_back);
    frontBack(0);
}

void PositionControl::moveBack() {
    frontBack(-m_alpha);
    std::this_thread::sleep_for(m_move_duration_back);
    frontBack(m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_back + m_move_duration_front);
    frontBack(-m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_front);
    frontBack(0);
}

void PositionControl::moveLeft() {
    leftRight(-m_alpha);
    std::this_thread::sleep_for(m_move_duration_lr);
    leftRight(m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_lr + m_move_duration_lr);
    leftRight(-m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_lr);
    leftRight(0);
}

void PositionControl::moveRight() {
    leftRight(m_alpha);
    std::this_thread::sleep_for(m_move_duration_lr);
    leftRight(-m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_lr + m_move_duration_lr);
    leftRight(m_alpha);
    std::this_thread::sleep_for(m_counter_move_duration_lr);
    leftRight(0);
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
