#ifndef SIMON_JOURNALIST_H
#define SIMON_JOURNALIST_H

#include "gameelement.h"

class GameSystem;

class Journalist : public GameElement {
public:
    Journalist(GameSystem& system);
    ~Journalist();

    void gameInit() override;
    void gameLoop() override;

private:
    void M_sendInfoPacket();

private:
    bool m_landed;
};

#endif // SIMON_JOURNALIST_H
