#include "mouvement_stalker.h"
#include "gamesystem.h"
#include "navdatacontroller.h"
#include "lcomm/info_packet.h"

Mouvement_Stalker::Mouvement_Stalker(GameSystem& system)
        : GameElement(system)
        {}

Mouvement_Stalker::~Mouvement_Stalker() {
}

void Mouvement_Stalker::gameInit() {

}

void Mouvement_Stalker::gameLoop() {
    Navdata nav = m_system.navdataController().grab();
    PositionControl pos_con ; 
    fill_pos_con(nav, pos_con);
}

void Mouvement_Stalker::fill_pos_con(Navdata nav, PositionControl &pos_con) {
       /* pos_con.set_x = 
        pos_con.set_y =
        pos_con.set_z =*/
        pos_con.vx = nav.demo.vx ; 
        pos_con.vy = nav.demo.vy ;
        pos_con.vz = nav.demo.vz ;
        pos_con.altitude = nav.demo.altitude ;
        pos_con.psi = nav.demo.psi ; 
        pos_con.phi = nav.demo.phi ;
        pos_con.theta = nav.demo.theta;
    }

