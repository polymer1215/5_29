//
// Created by Administrator on 2026/5/31.
//

#include "avoidObstacle.h"
#include "../motor/motor.h"

void turnLeft(void) {
    setLeftMotorPwm(-500);
    setRightMotorPwm(500);
}

void goStraight(void) {
    setLeftMotorPwm(500);
    setRightMotorPwm(500);
}

void turnRight(void) {
    setLeftMotorPwm(500);
    setRightMotorPwm(-500);
}