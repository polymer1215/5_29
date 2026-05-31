//
// Created by Administrator on 2026/5/31.
//
#include "control.h"
#include "../gray/gray.h"
#include "../motor/motor.h"

float error;

#define Kp    120
#define Kd    600

uint8_t last_infrared_state = 0;

volatile int last_err = 0;

void reset_PID() {
	last_err = 0;
}
float Track_err(void)
{
	// 优化：新增“连续2次状态一致才更新误差”，既防抖又避免更新滞后
	static uint8_t same_state_cnt = 0; // 新增：连续相同状态计数器

	if (gray_state == last_infrared_state)
	{
	    same_state_cnt++; // 状态一致，计数器加1
	}
	else
	{
	    same_state_cnt = 0; // 状态不一致，计数器清零
	    last_infrared_state = gray_state;
	}

	// 仅当连续2次状态一致时，才更新error，否则返回上一次误差
	if (same_state_cnt < 2)
	{
	    return error;
	}

	switch(gray_state)
	{
		case 0:     //0000 0000
		error= 0 ;	  break;
		case 16:    //0001 0000
		error= 1 ;  break;
		case 8:     //0000 1000
		error= 1 ; break;
		case 24:    //0001 1000
		error= 0 ;	  break;
		case 60:    //0011 1100
		error= 0 ;	  break;
		case 126:   //0111 1110
		error= 0 ;	  break;

		case 48:     //0011 0000   //小车右偏，err为正
		error= 2  ;	  break;
		case 32:     //0010 0000
		error= 2 ;	  break;
		case 64:     //0100 0000
		error= 4 ;	  break;
		case 96:     //0110 0000
		error= 4 ;	  break;
		case 128:    //1000 0000
		error= 6 ;	  break;
		case 192:    //1100 0000
		error= 6 ;	  break;
		case 224:   //1110 0000
		error= 6 ;	  break;
		case 160:   //1010 0000
		error= 6 ;	  break;
		case 254:   //1111 1110
		error= 6 ;	  break;
		case 252:   //1111 1100
		error= 6 ;	  break;
		case 248:   //1111 1000
		error= 6 ;	  break;
		case 240:   //1111 0000
		error= 6 ;	  break;
		case 124:   //0111 1100
		error= 4 ;	  break;
		case 120:   //0111 1000
		error= 4 ;	  break;
		case 56:    //0011 1000
		error= 2 ;	break;




		case 12:   //0000 1100   //小车左偏，err为负
		error= -2 ;	  break;
    case 14:   //0000 1110   //小车左偏，err为负
		error= -4 ;	  break;
    case 30:   //0001 1110   //小车左偏，err为负
		error= -4 ;	  break;
    case 62:   //0011 1110   //小车左偏，err为负
		error= -4 ;	  break;
		case 31:   //0001 1111
		error= -6 ;	  break;
		case 63:   //0011 1111
		error= -6 ;	  break;
		case 125:  //0111 1111
		error= -6 ;	  break;
		case 4:    //0000 0100
		error= -2 ;	  break;
    case 28:   //0001 1100
		error= -2 ;	  break;
		case 2:    //0000 0010
		error= -4 ;	  break;
		case 6:    //0000 0110
		error= -4 ;	  break;
		case 1:    //0000 0001
		error= -6 ;	  break;
		case 3:    //0000 0011
		error= -6 ;	  break;
		case 7:    //0000 0111
		error= -6 ;	  break;
		case 15:   //0000 1111
		error= -6 ;	  break;

		default:
		// 优化点1：默认状态不重置error为0，沿用当前值，避免无效跳变
		break;
	}
	last_infrared_state = gray_state;
	same_state_cnt = 0; // 重置计数器，为下一次防抖做准备
	return error;
}


int PID_out(float error, int Target)      //位置环
{

    int err = (int)error;
    int out;

    // 优化点3：抑制微分项初始跳变（角度环切换后首次循迹，避免输出突变）
    int diff = err - last_err;
    if (last_err == 0 && err != 0) {
        diff = err / 2;  // 微分项减半，减少初始冲击
    }

    // 保留原有PD计算逻辑，仅替换微分项为优化后的diff
    out = Kp * err + Kd * diff;

    // 优化点4：增加极简输出限幅，避免超出电机有效范围
    if (out > 1000) out = 1000;
    if (out < -1000) out = -1000;

    last_err = err;  // 保留原有历史误差更新
    return out;
}


void set_speed(int pid_out ,int base_speed)
{
    int left_speed = base_speed - pid_out;
    int right_speed = base_speed + pid_out;

    // 软过渡：限制电机速度变化幅度，避免爆冲
    #define MIN_SPEED -2000
    #define MAX_SPEED 2000

    // 左电机限幅
    left_speed = left_speed < MIN_SPEED ? MIN_SPEED : left_speed;
    left_speed = left_speed > MAX_SPEED ? MAX_SPEED : left_speed;
    // 右电机限幅
    right_speed = right_speed < MIN_SPEED ? MIN_SPEED : right_speed;
    right_speed = right_speed > MAX_SPEED ? MAX_SPEED : right_speed;

	setLeftMotorPwm(left_speed);
	setRightMotorPwm(right_speed);
}
