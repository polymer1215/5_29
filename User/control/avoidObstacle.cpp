//
// Created by Administrator on 2026/5/31.
//

#include "avoidObstacle.h"
#include "../motor/motor.h"

void turnLeft(void) {
    setLeftMotorDeg(-500);
    setRightMotorDeg(500);
}

void goStraight(void) {
    setLeftMotorDeg(500);
    setRightMotorDeg(500);
}

void turnRight(void) {
    setLeftMotorDeg(500);
    setRightMotorDeg(-500);
}

void stayStill(void) {
    setLeftMotorDeg(0);
    setRightMotorDeg(0);
}