#include "pid.h"
#define epsilon 0.01
int main()
{   
    while(1){


        position_control pos_con ;
        pos_con.error_x = 0.03 ; //a initialiser de la manière 
        print_position_control(pos_con) ;
        while (fabs(pos_con.error_x) >  epsilon)
        {
            pos_con= PIDcal(pos_con) ;
            print_position_control(pos_con) ;
        }

    }
}
