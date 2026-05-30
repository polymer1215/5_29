#ifndef GRAY_H
#define GRAY_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern volatile GPIO_PinState L1;
    extern volatile GPIO_PinState L2;
    extern volatile GPIO_PinState L3;
    extern volatile GPIO_PinState L4;
    extern volatile GPIO_PinState L5;
    extern volatile GPIO_PinState L6;
    extern volatile GPIO_PinState L7;
    extern volatile GPIO_PinState L8;
    extern volatile uint8_t gray_state;

    void update_gray_state();
#ifdef __cplusplus
}
#endif

#endif