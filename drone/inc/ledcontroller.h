#ifndef SIMON_LEDCONTROLLER_H
#define SIMON_LEDCONTROLLER_H

#include "gameelement.h"

class NavdataController;

class LEDController : public GameElement {
public:
    enum Led { Green = 0x00, Red, Yellow, Blue };

public:
    LEDController(GameSystem& system);
    ~LEDController();

    void gameInit() override;
    void gameLoop() override;

    //! Use this function to light up a led on the board
    //! \param led The led to light
    void setLed(Led led);

    //! Use this function to turn off a led on the board
    //! \param led The led to turn off
    void unsetLed(Led led);

private:
    void M_serial_config(int fd);
    int M_serial_init(const char* device);
    int M_serial_write(int fd, char* buffer, int size);

private:
    int m_serial;
    char m_flag;
};

#endif // SIMON_LEDCONTROLLER_H
