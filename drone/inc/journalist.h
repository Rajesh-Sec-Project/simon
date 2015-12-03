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
    bool m_landed, m_detect;
    int m_detect_x, m_detect_y;
    float m_speed_x, m_speed_y;
};

#endif // SIMON_JOURNALIST_H
