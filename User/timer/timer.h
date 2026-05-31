//
// Created by Administrator on 2026/5/29.
//

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
    extern volatile uint32_t currentMillis;
    extern volatile uint32_t lastMillis;
    extern volatile uint32_t lastMillis2;
    extern volatile uint32_t obstacleStartMillis;

#ifdef __cplusplus
}
#endif

#endif //TIMER_H
