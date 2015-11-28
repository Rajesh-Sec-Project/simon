#ifndef PID_H_
#define PID_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
 
typedef struct position_control {
	float set_x ; 
	float set_y ;
	float set_z ; 
	float real_x ; 
	float real_y ;
	float real_z ; 
	float error_x ; 
	float error_y ; 
	float error_z ; 
	float output_x ; 
	float output_y ; 
	float output_z ; 
}position_control;

position_control SpeedIntegrate(float vx, float vy, float altitude) ; 
position_control PIDcal(position_control) ;
void print_position_control(position_control) ;

#endif /*PID_H_*/