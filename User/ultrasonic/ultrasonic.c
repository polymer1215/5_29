//
// Created by Administrator on 2026/5/29.
//

#include "ultrasonic.h"
#include <stdint.h>
#include "main.h"
#include "cmsis_compiler.h"

volatile uint32_t ic_val1 = 0;
volatile uint32_t ic_val2 = 0;
volatile uint32_t overflow_cnt = 0;
volatile uint8_t  capture_flag = 0;
volatile float    distance_cm = 0;

void delay_us(uint32_t us) {
    uint32_t delay = us * 10; // F103 72MHz 粗略估算
    while(delay--) __asm("nop");
}

void ultrasonic_startMeasure(void) {
    if (capture_flag == 0 || capture_flag == 2) {
        capture_flag = 0; // 重置状态

        HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
        delay_us(12); // 略大于10us确保可靠
        HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
    }
}