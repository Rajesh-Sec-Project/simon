	#include "pid.h"
	
	//Define parameter
	#define epsilon 0.01 //max error acceptable
	#define dt 0.01//100ms loop time
//For Saturation
	//#define MAX 100
//#define MIN -100
//PID
	#define Kp 0.1 
	#define Kd 0.01
	#define Ki 0.005

struct position_control SpeedIntegrate(float vx, float vy, float altitude)
{	
	struct position_control pos_con ;
	//on event or every 80ms FOR X 
	float pre_vx = 0 ; 
	float pos_x = 0 ; 
	float pos_x_actuelle=0; 
	pos_x_actuelle = vx - pre_vx ; 
	pos_x = pos_x + pos_x_actuelle ; 
	pre_vx = vx ; 

	//on event or every 80ms FOR Y 
	float pre_vy= 0 ; 
	float pos_y = 0 ; 
	float pos_y_actuelle=0; 
	pos_y_actuelle = vy - pre_vy ; 
	pos_y = pos_y + pos_y_actuelle ; 
	pre_vy= vy ; 

	pos_con.real_x = pos_x ; 
	pos_con.real_y = pos_y ;
	pos_con.real_z = altitude ;

	return pos_con ;
/*float actual_position ;
//take 3 dim speed and make it a 3 dim position- then make 3 dim into 
//1 dim position with crude left right, down above 
return actual_position ; 
*/
}
//user has to give a 1 dim set point with up down left right
//UI puts info in structure POSITION_CONTROL under set_x, set_y, set_z 
//SpeedIntegrate transforms vx, vy and altitude into real_x, real_y, real_z 
 
struct position_control PIDcal(struct position_control pos_con)
{

	float pre_error_x= 0 ;float pre_error_y= 0 ;float pre_error_z= 0 ;
	float integral_x= 0 ; float integral_y= 0 ; float integral_z= 0 ;

	float derivative_x ;float derivative_y ;float derivative_z;
	

	//Caculate P,I,D
	pos_con.error_x = pos_con.set_x-pos_con.real_x ;
	if(fabs(pos_con.error_x > epsilon))//In case of error too small then stop integration
		{
			integral_x = integral_x + pos_con.error_x*dt;
		}

	derivative_x = (pos_con.error_x - pre_error_x)/dt;
	pos_con.output_x= Kp*pos_con.error_x + Ki*integral_x + Kd*derivative_x;
	pre_error_x = pos_con.error_x;

	pos_con.error_y = pos_con.set_y-pos_con.real_y ;

	if(fabs(pos_con.error_y > epsilon)) //In case of error too small then stop integration
		{
			integral_y = integral_y + pos_con.error_y*dt;
		}

	derivative_y= (pos_con.error_y - pre_error_y)/dt;
	pos_con.output_y= Kp*pos_con.error_y + Ki*integral_y + Kd*derivative_y;
	pre_error_y = pos_con.error_y;

	pos_con.error_z = pos_con.set_z-pos_con.real_z ;

	if(fabs(pos_con.error_z > epsilon)) //In case of error too small then stop integration
		{
			integral_z = integral_z + pos_con.error_z*dt;
		}

	derivative_z= (pos_con.error_z - pre_error_z)/dt;
	pos_con.output_z= Kp*pos_con.error_z + Ki*integral_z + Kd*derivative_z;
	pre_error_z = pos_con.error_z;	

	return pos_con ; 
}

void print_position_control(struct position_control pos_con){

	printf("set_x : %f   ", pos_con.set_x);
	printf("real_x: %f \n",pos_con.real_x);

	printf("set_y: %f   ", pos_con.set_y);
	printf("real_y: %f \n",pos_con.real_y);

	printf("set_z : %f   ", pos_con.set_z);
	printf("real_z: %f \n",pos_con.real_z);

	printf("error_x: %f \n",pos_con.error_x);
	printf("error_y: %f \n",pos_con.error_y);
	printf("error_z: %f \n",pos_con.error_z);
}
