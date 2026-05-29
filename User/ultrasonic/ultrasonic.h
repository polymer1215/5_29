//
// Created by Administrator on 2026/5/29.
//

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
    extern volatile uint32_t ic_val1;       // 上升沿计数器值
    extern volatile uint32_t ic_val2;       // 下降沿计数器值
    extern volatile uint32_t overflow_cnt;  // 捕获期间的溢出次数
    extern volatile uint8_t  capture_flag;  // 0:等待上升沿, 1:等待下降沿, 2:捕获完成
    extern volatile float distance_cm;   // 计算出的距离

    void delay_us(uint32_t us);
    void ultrasonic_startMeasure(void);

#ifdef __cplusplus
}
#endif

#endif //ULTRASONIC_H
