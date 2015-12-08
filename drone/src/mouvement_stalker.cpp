#include "mouvement_stalker.h"
#include <iomanip>
//#include "pid.h"!
#include "gamesystem.h"
#include "navdatacontroller.h"
#include "lcomm/info_packet.h"
#include "lcontrol/positioncontrol.h"
#include "lcontrol/control.h"
#include <math.h>
   
using namespace lcontrol;

    //Define parameter
    #define epsilon 0.01 //max error acceptable
    #define dt 0.01//100ms loop time
//For Saturation
    #define MAX 0.98
    #define MIN -0.98
//PID
    #define Kp 0.1 
    #define Kd 0.01
    #define Ki 0.005


Mouvement_Stalker::Mouvement_Stalker(GameSystem& system)
        : GameElement(system) 
        , file("samples.txt"){

}

Mouvement_Stalker::~Mouvement_Stalker() {
}

void Mouvement_Stalker::gameInit() {

    org.org_x =0.0 ; 
    org.org_y = 0.0 ;
    org.org_z = 0.0 ;

    pos_con.set_x = 0.0; 
    pos_con.set_y= 0.0;
    pos_con.set_z= 0.0; 
    pos_con.vx=0.0  ;
    pos_con.vy=0.0 ;
    pos_con.vz= 0.0;
    pos_con.altitude= 0 ;
    pos_con.theta = 0.0; ; 
    pos_con.psi = 0.0; ;
    pos_con.phi= 0.0; ;
    pos_con.real_x = 0.0; ; 
    pos_con.real_y = 0.0; ;
    pos_con.real_z = 0.0; ; 
    pos_con.error_x = 0.0; ; 
    pos_con.error_y = 0.0; ; 
    pos_con.error_z = 0.0; ; 
    pos_con.output_x= 0.0;  ; 
    pos_con.output_y = 0.0; ; 
    pos_con.output_z = 0.0; ; 

    speed_mem.pre_vx=0.0 ; 
    speed_mem.pre_vy = 0.0; 

    err_mem.pre_error_x=0.0; 
    err_mem.pre_error_y=0.0; 
    err_mem.pre_error_z=0.0; 

}

void Mouvement_Stalker::gameLoop() {

    
        Navdata nav = m_system.navdataController().grab() ;
        if(nav.header.state & navdata::fly) {
           
            fill_pos_con(nav);
            SpeedIntegrate() ;

            PIDcal() ;
            /*float leftRightTilt;
            float  frontBackTilt;
            float verticalSpeed ;
            float angularSpeed= 0.0;
            frontBackTilt = pos_con.output_y ; 
            leftRightTilt = pos_con.output_x ; 
            verticalSpeed = pos_con.output_z ; 
           // print_Position_Control() ; 
            Control::movement(1,frontBackTilt,leftRightTilt,verticalSpeed,angularSpeed) ; */
        }
        

}
void Mouvement_Stalker::setOrigine(Navdata const &nav){
/*
    float x_org = 0 ; 
    float x_org_now=0;

    x_org_now = nav.demo.vx - speed_mem.pre_vx ; 
    x_org = x_org + x_org_now; 
    speed_mem.pre_vx = nav.demo.vx ; 

    //float pre_vy= 0 ; 
    float pos_y = 0 ; 
    float pos_y_actuelle=0; 
    pos_y_actuelle = pos_con.vy - speed_mem.pre_vy ; 
    pos_y = pos_y + pos_y_actuelle ; 
    speed_mem.pre_vy= pos_con.vy; 

    pos_con.real_x = pos_x ; 
    pos_con.real_y = pos_y ;
    pos_con.real_z = pos_con.altitude ;*/
}
 
void Mouvement_Stalker::fill_pos_con(Navdata const &nav) {
        /*if(!(file >> pos_con.vx >> pos_con.vy >> pos_con.vz >>pos_con.altitude >> pos_con.theta >> pos_con.phi >> pos_con.psi)) {
            std::cout<<"plus de donnee "<<std::endl ; 

        }*/

        pos_con.vx = nav.demo.vx;
        pos_con.vy = nav.demo.vy;
        pos_con.vz = nav.demo.vz;
        pos_con.altitude = nav.demo.altitude;
        pos_con.psi = nav.demo.psi;
        pos_con.phi = nav.demo.phi;
        pos_con.theta = nav.demo.theta;
        
        pos_con.set_x = lcontrol::PositionControl::xPos();
        pos_con.set_y = lcontrol::PositionControl::yPos();
        pos_con.set_z = lcontrol::PositionControl::zPos();
        
    }


void Mouvement_Stalker::SpeedIntegrate()
{  
       /* float Vxd ; 
        float Vyd ;

    Vxd = (pos_con.vx*cos(pos_con.phi)) + (pos_con.vy*sin(pos_con.phi)) ; ;
    Vyd = (pos_con.vy*cos(pos_con.phi)) - (pos_con.vx*sin(pos_con.phi)); ;*/
    
    //float pre_vx = 0 ; 
    float pos_x = 0 ; 
    float pos_x_actuelle=0;

    pos_x_actuelle = pos_con.vx - speed_mem.pre_vx ; 
    pos_x = pos_x + pos_x_actuelle ; 
    speed_mem.pre_vx = pos_con.vx ; 

    //float pre_vy= 0 ; 
    float pos_y = 0 ; 
    float pos_y_actuelle=0; 
    pos_y_actuelle = pos_con.vy - speed_mem.pre_vy ; 
    pos_y = pos_y + pos_y_actuelle ; 
    speed_mem.pre_vy= pos_con.vy; 

    pos_con.real_x = pos_x ; 
    pos_con.real_y = pos_y ;
    pos_con.real_z = pos_con.altitude ;
    std::cout<<"vitesse selon x,y,z" <<pos_con.vx<<" " << pos_con.vy<< " " << pos_con.vz<<std::endl ; 
    std::cout<<"real position d'apres integrateur  " <<pos_con.real_x<<" " << pos_con.real_y<< " " << pos_con.real_z<<std::endl ; 
    //std::cout<<"Euler angles theta phi psi  " <<pos_con.theta<<" " << pos_con.phi<< " " << pos_con.psi<<std::endl ; 

}


 
void Mouvement_Stalker::PIDcal()
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
    if (pos_con.output_x>MAX) {
        pos_con.output_x = MAX ;
    }
    if (pos_con.output_x<MIN) {
        pos_con.output_x = MIN ;
    }

    err_mem.pre_error_x = pos_con.error_x;

    pos_con.error_y = pos_con.set_y-pos_con.real_y ;

    if(fabs(pos_con.error_y > epsilon)) //In case of error too small then stop integration
        {
            integral_y = integral_y + pos_con.error_y*dt;
        }

    derivative_y= (pos_con.error_y - pre_error_y)/dt;
    pos_con.output_y= Kp*pos_con.error_y + Ki*integral_y + Kd*derivative_y;

    if (pos_con.output_y>MAX) {
        pos_con.output_y = MAX ;
    }
    if (pos_con.output_y<MIN) {
        pos_con.output_y = MIN ;
    }

    err_mem.pre_error_y = pos_con.error_y;

    pos_con.error_z = pos_con.set_z-pos_con.real_z ;

    if(fabs(pos_con.error_z > epsilon)) //In case of error too small then stop integration
        {
            integral_z = integral_z + pos_con.error_z*dt;
        }

    derivative_z= (pos_con.error_z - pre_error_z)/dt;
    pos_con.output_z= Kp*pos_con.error_z + Ki*integral_z + Kd*derivative_z;

    if (pos_con.output_z>MAX) {
        pos_con.output_z = MAX ;
    }
    if (pos_con.output_z<MIN) {
        pos_con.output_z = MIN ;
    }

    err_mem.pre_error_z = pos_con.error_z;  

    std::cout<<"pos commande" <<pos_con.output_x<<" " << pos_con.output_y<< " " << pos_con.output_z<<std::endl ; 

}


void Mouvement_Stalker::print_Position_Control(){

    std::string clr = "                      ";

        /*std::cout << "set_x:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.set_x / 100.0f << clr << std::endl;
        std::cout << "set_y:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.set_y / 100.0f << clr << std::endl;*/
        std::cout << "set_z:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.set_z / 100.0f << clr << std::endl;

        std::cout << "output z:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.output_z / 100.0f << clr << std::endl;
        std::cout << "output x:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.output_x / 100.0f << clr << std::endl;
        std::cout << "output y:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.output_y / 100.0f << clr << std::endl;

        /*std::cout << "real_x:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.real_x / 100.0f << clr << std::endl;
        std::cout << "real_y:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.real_y / 100.0f << clr << std::endl;*/
        std::cout << "real_z:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << pos_con.real_z / 100.0f << clr << std::endl;

        std::cout << "error z:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << err_mem.pre_error_z / 100.0f << clr << std::endl;
        
        std::cout << "error y:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << err_mem.pre_error_y / 100.0f << clr << std::endl;
        std::cout << "error x:    " << std::fixed << std::setw(4) << std::setprecision(1) << std::setfill('0')
        << err_mem.pre_error_x / 100.0f << clr << std::endl;

        std::cout << "\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A";
        
}
/*
void Mouvement_Stalker::speed_command_output(){
    //Hover mode 
    //Control::movement(0,frontBackTilt,leftRightTilt,verticalSpeed,angularSpeed) ; 
    //left 50cm 
    //output x , y z à appliquer sur mon systeme en 
    float leftRightTilt = -0.05; 
    float  frontBackTilt = 0 ;
    float verticalSpeed =0.001 ;
    float angularSpeed = 0 ; 
    Control::takeoff() ; 
    //Control::movement(frontBackTilt,leftRightTilt,verticalSpeed,angularSpeed) ; 
}*/

/*void Mouvement_Stalker::fill_pos_con(Navdata nav, Position_Control& pos_con) {

    pos_con.set_x = lcontrol::PositionControl::xPos();
    pos_con.set_y = lcontrol::PositionControl::yPos();
    pos_con.set_z = lcontrol::PositionControl::zPos();
    pos_con.vx = nav.demo.vx;
    pos_con.vy = nav.demo.vy;
    pos_con.vz = nav.demo.vz;
    pos_con.altitude = nav.demo.altitude;
    pos_con.psi = nav.demo.psi;
    pos_con.phi = nav.demo.phi;
    pos_con.theta = nav.demo.theta;
}*/
