//
// Created by Administrator on 2026/5/31.
//

#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

    float Track_err(void);

    int PID_out(float error,int Target);
    void set_speed(int pid_out ,int base_speed);

#ifdef __cplusplus
}
#endif

#endif //CONTROL_H
