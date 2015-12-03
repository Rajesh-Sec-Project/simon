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
