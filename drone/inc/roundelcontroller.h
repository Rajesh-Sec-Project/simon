#ifndef SIMON_ROUNDELCONTROLLER_H
#define SIMON_ROUNDELCONTROLLER_H

#include "gameelement.h"
#include <string>

class NavdataController;

class RoundelController : public GameElement {
public:
    RoundelController(GameSystem& system);
    ~RoundelController();

    void init();

private:
    void M_clearAck();

private:
    NavdataController& m_navctrl;
};

#endif // SIMON_ROUNDEL_H
