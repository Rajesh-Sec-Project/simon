/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *                    Original source code from Romain Taprest
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

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "usb_cdc.h"
#include "usbComm.h"

void usbCommInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    // LED indicator on the board (C.12)
    GPIO_InitStructure.GPIO_Pin = PIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Output signals
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = PIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USB init
    Set_System();
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

    // Wait for USB to be OK
    GPIO_WriteBit(GPIOC, GPIO_Pin_12, 1);
    // Wait USB is ready
    while(USB_GetState() != CONFIGURED) {
    }
}

void usbCommLoop(void) {
    static uint8_t buffer[8];
    static uint16_t readSize;

    int ok = USB_GetState() == CONFIGURED;
    GPIO_WriteBit(GPIOC, GPIO_Pin_12, ok ? 0 : 1);

    usbCommReadArray(buffer, sizeof(buffer), &readSize);

    if(readSize == 2) {
        uint8_t magic = buffer[0];
        uint8_t flag = buffer[1];

        if(magic != 0xCA)
            return;

        GPIO_WriteBit(GPIOA, GPIO_Pin_1, (flag & (0x01 << 0)) >> 0);
        GPIO_WriteBit(GPIOA, GPIO_Pin_2, (flag & (0x01 << 1)) >> 1);
        GPIO_WriteBit(GPIOA, GPIO_Pin_3, (flag & (0x01 << 2)) >> 2);
        GPIO_WriteBit(GPIOA, GPIO_Pin_4, (flag & (0x01 << 3)) >> 3);
    }
}

void usbCommLoopBack(void) {
    if(USB_GetTxSize() > 0) {
        USB_Send(USB_Receive());
    }
}

void usbCommSendChar(uint8_t c) {
    USB_Send(c);
}

void usbCommSendData(uint8_t* array, uint16_t size) {
    uint16_t idChar = 0;

    for(idChar = 0; idChar < size; idChar++) {
        // Send one char
        usbCommSendChar(array[idChar]);
    }
}

bool usbCommReadByte(uint8_t* byte) {
    uint8_t TxSize = USB_GetTxSize();
    if(TxSize > 0) {
        *byte = USB_Receive();
        return true;
    } else {
        return false;
    }
}

void usbCommReadArray(uint8_t* array, uint16_t maxSize, uint16_t* readSize) {
    uint8_t TxSize = USB_GetTxSize();
    uint16_t i = 0;

    if(maxSize < TxSize) {
        *readSize = maxSize;
    } else {
        *readSize = TxSize;
    }
    for(i = 0; i < *readSize; i++) {
        array[i] = USB_Receive();
    }
}

uint8_t usbCommWaitInput(void) {
    while(USB_GetTxSize() == 0)
        ;
    return USB_Receive();
}
