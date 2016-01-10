#ifndef SIMON_LEDCONTROLLER_H
#define SIMON_LEDCONTROLLER_H

#include "gameelement.h"

class NavdataController;

class LEDController : public GameElement {
public:
    enum Led {
        Blue = 0x00,
        Yellow,
        Green,
        Red
    };

public:
    LEDController(GameSystem& system);
    ~LEDController();

    void gameInit() override;
    void gameLoop() override;

    void setLed(int led);
    void unsetLed(int led);

private:
    void M_serial_config(int fd);
    int M_serial_init(const char * device);
    int M_serial_write(int fd, char* buffer, int size);

private:
    int m_serial;
    char m_flag;
};

#endif // SIMON_LEDCONTROLLER_H
