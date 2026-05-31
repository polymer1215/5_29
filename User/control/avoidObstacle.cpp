//
// Created by Administrator on 2026/5/31.
//

#include "avoidObstacle.h"
#include "../motor/motor.h"
#include "../gray/gray.h"
#include "../timer/TimerTask.hpp"

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

void avoidObstacle(void) {
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

}

bool can_resume_track() {
    int sum = 0;
    sum += (int)L1 + (int)L2 + (int)L3 + (int)L4 + (int)L5 + (int)L6 + (int)L7 + (int)L8;
    if (sum != 6 && sum != 7) {
        return false;
    }

    return true;

}