#ifndef PID_H_
#define PID_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
 
typedef struct PositionControl {
	float set_x ; 
	float set_y ;
	float set_z ; 
	float vx ;
    float vy ;
    float vz ;
    uint32_t altitude ;
    float theta ; 
    float psi ;
    float phi;
	float real_x ; 
	float real_y ;
	float real_z ; 
	float error_x ; 
	float error_y ; 
	float error_z ; 
	float output_x ; 
	float output_y ; 
	float output_z ; 
}PositionControl;

PositionControl SpeedIntegrate(float vx, float vy, float altitude) ; 
PositionControl PIDcal(PositionControl) ;
void print_PositionControl(PositionControl) ;

#endif /*PID_H_*/