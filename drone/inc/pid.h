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

/*#ifndef PID_H_
#define PID_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Position_Control {
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
} Position_Control;

<<<<<<< HEAD
Position_Control SpeedIntegrate(float vx, float vy, float altitude) ;
Position_Control PIDcal(Position_Control) ;
void print_Position_Control(Position_Control) ;
*/
//#endif /*PID_H_*/
=======
Position_Control SpeedIntegrate(float vx, float vy, float altitude);
Position_Control PIDcal(Position_Control);
void print_Position_Control(Position_Control);

#endif /*PID_H_*/
>>>>>>> 6bfb48668b3afb80063dda76611e9b67797976b5
