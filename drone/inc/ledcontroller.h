/*
 * Copyright (c) 2016 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
