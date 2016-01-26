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

#include "ledcontroller.h"
#include "gamesystem.h"

#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

LEDController::LEDController(GameSystem& system)
        : GameElement(system)
        , m_serial(-1)
        , m_flag(0) {
}

LEDController::~LEDController() {
    if(m_serial > 0)
        close(m_serial);
}

void LEDController::gameInit() {
    m_serial = M_serial_init("/dev/ttyACM0");
    if(m_serial < 0)
        M_error("unable to init serial port");
}

void LEDController::gameLoop() {
}

void LEDController::setLed(LEDController::Led led) {
    m_flag |= (0x01 << led);

    char buf[2] = {0xCA, m_flag};
    M_serial_write(m_serial, buf, 2);
}

void LEDController::unsetLed(LEDController::Led led) {
    m_flag &= ~(0x01 << led);

    char buf[2] = {0xCA, m_flag};
    M_serial_write(m_serial, buf, 2);
}

void LEDController::M_serial_config(int fd) {
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD); // Enable local mode
    options.c_cflag &= ~PARENB;          // Disable parity
    options.c_cflag &= ~CSTOPB;          // One stop bit
    options.c_cflag &= ~CSIZE;           // Clear data size bits
    options.c_cflag |= CS8;              // Set 8 bits data size
    // options.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Set raw input (unbuffered, no echo)
    options.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable software flow control
    options.c_oflag &= ~OPOST;                          // Set raw output
    // Set read() to return when 1 or more characters are received
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSAFLUSH, &options);
}


int LEDController::M_serial_init(const char* device) {
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1) {
        M_error("unable to open serial device");
        return -1;
    }

    if(!isatty(fd)) {
        M_error("the device is not a serial port");
        close(fd);
        return -1;
    }

    M_serial_config(fd);

    return fd;
}

int LEDController::M_serial_write(int fd, char* buffer, int size) {
    int n = write(fd, buffer, size);
    if(n < 0) {
        M_error("unable to write to serial port");
        return -errno;
    }
    tcflush(fd, TCIFLUSH);

    return 0;
}
