//
// Created by Administrator on 2026/5/29.
//

#include "main_entry.h"

#include "tim.h"
#include "../../Lib/oled/oled.h"
#include "../timer/timer.h"

void main_entry(void) {

    HAL_Delay(20);
    OLED_Init();
    HAL_TIM_Base_Start_IT(&htim4);

    for (uint32_t i = 0; i < 256; ++i) {
        OLED_NewFrame();
        OLED_DrawImage((128 - (bilibiliImg.w)) / 2, 0, &bilibiliImg, OLED_COLOR_NORMAL);
        OLED_PrintString(128 - i, 64 - 16, "波特律动hello", &font16x16, OLED_COLOR_NORMAL);
        OLED_ShowFrame();
    }

    for (;;) {
        if (currentMillis - lastMillis > 1000) {
            lastMillis = currentMillis;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }

    }
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        currentMillis += 10;
    }
}