//
// Created by Administrator on 2026/3/18.
//

#include "tim.h"
#include "motor.h"
#include "../pid/pid.h"

const uint32_t RATIO = 20; // motor decceration ratio
const uint32_t FREQUENCY = 100; // tim4 interrupt frequency
const uint32_t LINE = 500;
const uint32_t REV = LINE * 4 * RATIO;
const int32_t MAX_pwm = 3599;


int32_t leftMotorDegTarget = 0;
int32_t rightMotorDegTarget = 0;


volatile int32_t rightMotorDeg = 0;
volatile int32_t leftMotorDeg = 0;
volatile int32_t leftMotorPwm = 0;
volatile int32_t rightMotorPwm = 0;

const float KP = 5.0;
const float KI = 1.55;
const float KD = 0.2;

PID_Instance pid_left;
PID_Instance pid_right;

void allMotorInit() {
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    PID_Init(&pid_left, PID_MODE_POSITIONAL, KP, KI, KD, 3599, -3599);
    PID_Init(&pid_right, PID_MODE_POSITIONAL, KP, KI, KD, 3599, -3599);
}


void setLeftMotorDeg(int32_t deg) {
    leftMotorDegTarget = deg;
}

void setRightMotorDeg(int32_t deg) {
    rightMotorDegTarget = deg;
}

void setRightMotorPwm(int32_t pwm) {
    uint32_t pwm_val = 0;
    if (pwm >= 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
        pwm_val = (uint32_t)pwm;
    } else if (pwm < 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
        pwm_val = (uint32_t)(-pwm);
    }
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_val);
    rightMotorPwm = pwm;
}

void setLeftMotorPwm(int32_t pwm) {
    uint32_t pwm_val = 0;
    if (pwm >= 0) {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
        pwm_val = (uint32_t)pwm;
    } else if (pwm < 0) {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
        pwm_val = (uint32_t)(-pwm);
    }
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_val);
    leftMotorPwm = pwm;
}


void updateAllMotor() {
    updateRightMotorSpeed();
    updateLeftMotorSpeed();
}

void updateRightMotorSpeed() {
    int16_t count = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    rightMotorDeg = (int32_t)count * FREQUENCY * 360 / REV;
}

void updateLeftMotorSpeed() {
    int16_t count = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 0);

    leftMotorDeg = (int32_t)count * FREQUENCY * 360 / REV;
}

void rightMotorPid() {
    int32_t pwm_output = PID_Compute(&pid_right, rightMotorDegTarget, rightMotorDeg);
    setRightMotorPwm(pwm_output);
}

void leftMotorPid() {
    int32_t pwm_output = PID_Compute(&pid_left, leftMotorDegTarget, leftMotorDeg);
    setLeftMotorPwm(pwm_output);
}