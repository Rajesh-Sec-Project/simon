#ifndef SIMON_TAGCONTROLLER_H
#define SIMON_TAGCONTROLLER_H

#include "gameelement.h"
#include "lcomm/clientsocket.h"
#include <string>

class NavdataController;

class TagController : public GameElement {
public:
    TagController(GameSystem& system);
    ~TagController();

    void gameInit() override;
    void gameLoop() override;

private:
    void M_clearAck();

private:
    NavdataController& m_navctrl;
    lcomm::ClientSocket m_videoSock;
};

#endif // SIMON_TAGCONTROLLER_H
