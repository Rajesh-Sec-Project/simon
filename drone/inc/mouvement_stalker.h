#ifndef MOUVEMENT_STALKER_H
#define MOUVEMENT_STALKER_H

#include "gameelement.h"
#include "pid.h"
#include "navdatacontroller.h"

class GameSystem;

class Mouvement_Stalker : public GameElement {
public:
    Mouvement_Stalker(GameSystem& system);
    ~Mouvement_Stalker();

    void gameInit() override;
    void gameLoop() override;
    void fill_pos_con(Navdata nav, Position_Control& pos_con);
};

#endif // MOUVEMENT_STALKER_H
