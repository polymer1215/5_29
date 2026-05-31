//
// Created by Administrator on 2026/5/29.
//

#include "main_entry.h"

#include "tim.h"
#include "../../Lib/oled/OLED.h"
#include "../control/avoidObstacle.h"
#include "../control/control.h"
#include "../timer/timer.h"
#include "../ultrasonic/ultrasonic.h"
#include "../motor/motor.h"
#include "../gray/gray.h"
#include "../timer/TimerTask.hpp"


enum class State {
    TRACK,
    OBSTACLE,
    TEST,
};

volatile State state = State::TRACK;

volatile bool enable_track_pid = 1;
volatile bool enable_speed_pid = 1;

volatile bool ENTER_OBSTACLE = 1;
volatile bool WAIT_FOR_RESUME_TRACK = 1;
volatile bool READY_FOR_ADJUST_POS = 1;

volatile bool ENTER_TEST = 1;

volatile uint32_t tim4_irq_ticks = 0;
uint32_t main_tim4_ticks = 0;

volatile float current_error = 0;

void main_entry(void) {
    HAL_Delay(100);  // wait for OLED power-up
    OLED_Init();

    distance_cm = 100;
    allMotorInit();
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
    HAL_TIM_Base_Start_IT(&htim4);





    for (;;) {

        if (tim4_irq_ticks != main_tim4_ticks)
        {
            main_tim4_ticks++;

            update_gray_state();
            switch (state) {
                case State::TEST:
                    if (ENTER_TEST) {
                        ENTER_TEST = 0;
                        setLeftMotorDeg(1000);
                        setRightMotorDeg(1000);
                        enable_track_pid = 0;
                    }
                    break;

                case State::TRACK:

                    if (distance_cm < 12) {
                        enable_track_pid = 0;
                        setLeftMotorPwm(0);
                        setRightMotorPwm(0);
                        state = State::OBSTACLE;
                    }

                    break;

                case State::OBSTACLE:
                    if (ENTER_OBSTACLE) {
                        TimerTask::ClearTasks();
                        TimerTask::AddTask(turnLeft, 500);
                        TimerTask::AddTask(stayStill, 50);
                        TimerTask::AddTask(goStraight, 800);
                        TimerTask::AddTask(stayStill, 50);
                        TimerTask::AddTask(turnRight, 500);
                        TimerTask::AddTask(stayStill, 50);
                        TimerTask::AddTask(goStraight, 1500);
                        TimerTask::AddTask(stayStill, 50);
                        TimerTask::AddTask(turnRight, 500);
                        TimerTask::AddTask(stayStill, 50);
                        TimerTask::AddTask(goStraight, 800);
                        TimerTask::AddTask(stayStill, 50);
                        ENTER_OBSTACLE = 0;
                    }


                    if (TimerTask::IsFinished()) {

                        if (WAIT_FOR_RESUME_TRACK) {
                            setLeftMotorDeg(-500);
                            setRightMotorDeg(500);
                            WAIT_FOR_RESUME_TRACK = 0;
                        }

                        if (can_resume_track()) {
                            state = State::TRACK;
                            track_PID_reset();
                            PID_Reset(&pid_left);
                            PID_Reset(&pid_right);
                            ENTER_OBSTACLE = 1;
                            WAIT_FOR_RESUME_TRACK = 1;
                            enable_track_pid = 1;
                            TimerTask::ClearTasks();
                        }

                    }
                    break;
            }
            // if ((L1 && L2 && L3 && L4 && L5 && L6 && L7 && L8) || (!L1 && !L2 && !L3 && !L4 && !L5 && !L6 && !L7 && !L8)) {
            //     enable_pid = 0;
            //     setLeftMotorPwm(0);
            //     setRightMotorPwm(0);
            // }
            if (enable_track_pid) {
                current_error = Track_err();
                int pid_val = PID_out(current_error, 0);
                setLeftMotorDeg(500 - pid_val);
                setRightMotorDeg(500 + pid_val);
            }

            if (enable_speed_pid) {
                leftMotorPid();
                rightMotorPid();
            }

            TimerTask::Update();

        }

        if (currentMillis - lastMillis > 100) {
            lastMillis += 100;
            ultrasonic_startMeasure();
        }

        if (currentMillis - lastMillis2 > 1000) {
            lastMillis2 += 1000;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            OLED_ShowNum(0, 0, distance_cm, 10, OLED_8X16);
            OLED_ShowNum(0, 20, leftMotorDeg, 10, OLED_8X16);
            OLED_ShowNum(0, 40, rightMotorDeg, 10, OLED_8X16);
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
        tim4_irq_ticks++;

        updateAllMotor();

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