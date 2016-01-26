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

#ifndef MOUVEMENT_STALKER_H
#define MOUVEMENT_STALKER_H

#include "gameelement.h"
#include "lcomm/lcomm.h"
#include <fstream>
#include <iostream>
#include <memory>
#include "navdatacontroller.h"

struct SetOrigin {
    float org_x;
    float org_y;
    float org_z;
};

struct Position_Control {
    float set_x;
    float set_y;
    float set_z;
    float vx;
    float vy;
    float vz;
    uint32_t altitude;
    float theta;
    float psi;
    float phi;
    float real_x;
    float real_y;
    float real_z;
    float error_x;
    float error_y;
    float error_z;
    float output_x;
    float output_y;
    float output_z;
};


struct SpeedMemory {
    float pre_vx;
    float pre_vy;
};

struct ErrorMemory {
    float pre_error_x;
    float pre_error_y;
    float pre_error_z;
};

struct Gains {
    struct {
        float kp, kd, ki;
    } xy, z;
};

class GameSystem;

class Mouvement_Stalker : public GameElement, public lcomm::Subscriber {
public:
    Mouvement_Stalker(GameSystem& system);
    ~Mouvement_Stalker();

    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;

    void gameInit() override;
    void gameLoop() override;

    void setOrigine(Navdata const& nav);
    void fill_pos_con(Navdata const& nav);
    void SpeedIntegrate();

    void PIDcal();
    void print_Position_Control();
    void speed_command_output();

    Gains& gains();
    Gains const& gains() const;

private:
    Gains m_gains;
    SetOrigin org;
    Position_Control pos_con;
    SpeedMemory speed_mem;
    ErrorMemory err_mem;
    std::ofstream file;
};

#endif // MOUVEMENT_STALKER_H
