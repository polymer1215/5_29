//
// Created by Administrator on 2026/3/18.
//

#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../pid/pid.h"

extern volatile int32_t leftMotorDeg;
extern volatile int32_t rightMotorDeg;

extern volatile int32_t leftMotorPwm;
extern volatile int32_t rightMotorPwm;

extern PID_Instance pid_left;
extern PID_Instance pid_right;

void allMotorInit();

void setLeftMotorDeg(int32_t deg);
void setRightMotorDeg(int32_t deg);

void updateAllMotor();
void updateRightMotorSpeed();
void updateLeftMotorSpeed();

void setLeftMotorPwm(int32_t pwm);
void setRightMotorPwm(int32_t pwm);

    void rightMotorPid();
    void leftMotorPid();

#ifdef __cplusplus
}
#endif

#endif //MOTOR_H
