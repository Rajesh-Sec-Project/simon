#ifndef SIMON_ROUNDELCONTROLLER_H
#define SIMON_ROUNDELCONTROLLER_H

#include "gameelement.h"
#include "lcomm/clientsocket.h"
#include <string>

class NavdataController;

class RoundelController : public GameElement {
public:
    RoundelController(GameSystem& system);
    ~RoundelController();

    void gameInit() override;

private:
    void M_clearAck();

private:
    NavdataController& m_navctrl;
    lcomm::ClientSocket m_videoSock;
};

#endif // SIMON_ROUNDEL_H
