#ifndef S_SERIALCOMM_H
#define S_SERIALCOMM_H

#include "typesAndConstants.h"

void usbCommInit(void);
void usbCommLoop(void);
void usbCommSendChar(uint8_t c);
void usbCommSendData(uint8_t* array, uint16_t size);
void usbCommLoopBack(void);
bool usbCommReadByte(uint8_t* byte);
void usbCommReadArray(uint8_t* array, uint16_t maxSize, uint16_t* readSize);
uint8_t usbCommWaitInput(void);

#endif
