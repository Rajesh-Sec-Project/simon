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

void PositionControl::frontMove()
{
    front(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    front(-2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    front(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    front(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    front(2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    front(-m_alpha);
}

void PositionControl::backMove()
{
    back(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    back(-2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    back(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    back(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    back(2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    back(-m_alpha);
}

void PositionControl::leftMove()
{
    left(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    left(-2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    left(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    left(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    left(2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    left(-m_alpha);
}

void PositionControl::rightMove()
{
    right(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    right(-2*m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    right(m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_counter_move_duration));
    right(-m_alpha);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_move_duration));
    right(2*m_alpha);
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
