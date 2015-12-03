#ifndef MOUVEMENT_STALKER_H
#define MOUVEMENT_STALKER_H

#include "gameelement.h"
//#include "pid.h"
#include "navdatacontroller.h"

 
typedef struct Position_Control {
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
}Position_Control;

typedef struct speedmemory{
	float pre_vx ; 
	float pre_vy ; 
}SpeedMemory; 

typedef struct errormemory{
	float pre_error_x; 
	float pre_error_y; 
	float pre_error_z; 
}ErrorMemory; 

class GameSystem;

class Mouvement_Stalker : public GameElement {
public:
    Mouvement_Stalker(GameSystem& system);
    ~Mouvement_Stalker();

    void gameInit() override;
    void gameLoop() override;

	void fill_pos_con(Navdata nav) ;
	void SpeedIntegrate() ;
	
	void PIDcal() ;
	void print_Position_Control() ;
	void speed_command_output() ;

private: 
	Position_Control pos_con ;
	SpeedMemory speed_mem ; 
	ErrorMemory err_mem ; 


    void fill_pos_con(Navdata nav, Position_Control& pos_con);

};

#endif // MOUVEMENT_STALKER_H