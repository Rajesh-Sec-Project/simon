#include "typesAndconstants.h"
#include "stm32f10x.h"
#include "Clock.h"
#include "usbComm.h"

int main(void) {
    // Clock
    CLOCK_Configure();

    // USB communication
    usbCommInit();

    while(1) {
        usbCommLoop();
    }

    return 0;
}
