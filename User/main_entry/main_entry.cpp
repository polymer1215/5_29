//
// Created by Administrator on 2026/5/29.
//

#include "main_entry.h"

#include "tim.h"
#include "../../Lib/oled/OLED.h"
#include "../control/control.h"
#include "../timer/timer.h"
#include "../ultrasonic/ultrasonic.h"
#include "../motor/motor.h"
#include "../gray/gray.h"


volatile bool enable_pid = 1;
void main_entry(void) {
    HAL_Delay(100);  // wait for OLED power-up
    OLED_Init();

    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);

    allMotorInit();



    for (;;) {
        if (currentMillis - lastMillis > 100) {
            lastMillis += 100;
            ultrasonic_startMeasure();
        }

        if (currentMillis - lastMillis2 > 1000) {
            lastMillis2 += 1000;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            OLED_ShowNum(0, 0, currentMillis, 10, OLED_8X16);
            OLED_ShowNum(0, 20, distance_cm, 10, OLED_8X16);
            OLED_Update();
        }
    }
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        if (capture_flag == 1)
        {
            overflow_cnt++;
        }

        currentMillis += 10;


        update_gray_state();
        if ((L1 && L2 && L3 && L4 && L5 && L6 && L7 && L8) || (!L1 && !L2 && !L3 && !L4 && !L5 && !L6 && !L7 && !L8)) {
            enable_pid = 0;
            setLeftMotorPwm(0);
            setRightMotorPwm(0);
        }
        if (enable_pid) {
            float current_error = Track_err();
            int pid_val = PID_out(current_error, 0);
            set_speed(pid_val, 500);
        }

    }
}

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        if (capture_flag == 0) // 捕获到上升沿
        {
            ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            overflow_cnt = 0; // 清空溢出计数
            capture_flag = 1; // 切换到等待下降沿状态

            // 设置为下降沿捕获
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else if (capture_flag == 1) // 捕获到下降沿
        {
            ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            capture_flag = 2; // 标记捕获完成

            // 计算总时间 (us)
            // 总计数值 = (溢出次数 * (Period + 1)) + 下降沿值 - 上升沿值
            uint32_t total_ticks = (overflow_cnt * 10000) + ic_val2 - ic_val1;

            distance_cm = total_ticks * 0.017f; // 相当于 total_ticks / 58.8

            // 恢复为上升沿捕获，为下次测量做准备
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        }

    }
}