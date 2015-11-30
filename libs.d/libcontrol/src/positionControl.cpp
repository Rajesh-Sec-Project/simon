//
// Created by remi on 26/11/15.
//

#include "lcontrol/positioncontrol.h"

using namespace lcontrol;

Distance PositionControl::m_x;
Distance PositionControl::m_y;
Distance PositionControl::m_z;

void PositionControl::left(Distance d) {
    // return xPos and yPOs and zPos
    m_x = -d;
    m_y = 0;
    m_z = 0;
}
void PositionControl::right(Distance d) {
    // return xPos and yPOs and zPos
    m_x = d;
    m_y = 0;
    m_z = 0;
}
void PositionControl::up(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y = 0;
    m_z = d;
}
void PositionControl::down(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y = 0;
    m_z = -d;
}
void PositionControl::front(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y = -d;
    m_z = 0;
}
void PositionControl::back(Distance d) {
    // return xPos and yPOs and zPos
    m_x = 0;
    m_y = d;
    m_z = 0;
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
