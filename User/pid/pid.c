#include "pid.h"

/**
 * @brief PID 初始化
 * @param mode 模式选择 (PID_MODE_POSITIONAL 或 PID_MODE_INCREMENTAL)
 * @param p, i, d 浮点 PID 参数
 * @param o_max 输出上限
 * @param o_min 输出下限
 */
void PID_Init(PID_Instance *pid, PID_Mode_t mode, float p, float i, float d, int32_t o_max, int32_t o_min) {
    pid->mode = mode;
    pid->Kp = (int32_t)(p * PID_SCALE_FACTOR);
    pid->Ki = (int32_t)(i * PID_SCALE_FACTOR);
    pid->Kd = (int32_t)(d * PID_SCALE_FACTOR);

    pid->out_max = o_max;
    pid->out_min = o_min;

    // 积分限幅：确保积分项单独作用时不超过输出限幅
    // int_max = out_max * SCALE / Ki (需避免除零)
    if (pid->Ki > 0) {
        int64_t tmp = (int64_t)o_max * PID_SCALE_FACTOR / pid->Ki;
        pid->int_max = (int32_t)(tmp > 0 ? tmp : o_max);
    } else {
        pid->int_max = o_max;
    }

    PID_Reset(pid);
}

/**
 * @brief 重置 PID 状态（切换目标或重启系统时调用）
 */
void PID_Reset(PID_Instance *pid) {
    pid->err_last = 0;
    pid->err_prev = 0;
    pid->integral = 0;
    pid->current_out = 0;
}

/**
 * @brief 通用 PID 计算
 * @param target 目标值 (int32)
 * @param measure 测量值 (int32)
 * @return 控制输出
 */
int32_t PID_Compute(PID_Instance *pid, int32_t target, int32_t measure) {
    int32_t err = target - measure;
    int32_t out = 0;

    if (pid->mode == PID_MODE_POSITIONAL) {
        /* --- 位置式 PID 逻辑（积分钳位抗饱和） --- */

        // 始终累积积分（以 int_max 做硬限幅兜底）
        pid->integral += err;
        if (pid->integral > pid->int_max) pid->integral = pid->int_max;
        else if (pid->integral < -pid->int_max) pid->integral = -pid->int_max;

        // 计算 P / I / D 各项
        int32_t p_term = pid->Kp * err;
        int32_t i_term = pid->Ki * pid->integral;
        int32_t d_term = pid->Kd * (err - pid->err_last);
        out = (p_term + i_term + d_term) >> PID_SHIFT;

        // 输出限幅 + 积分钳位：反算积分上限，从源头防止饱和
        if (pid->Ki > 0) {  // 仅当有积分作用时才做钳位
            if (out > pid->out_max) {
                // out_max = (P + Ki*I_new + D) / SCALE
                // → I_new = (out_max*SCALE - P - D) / Ki
                int32_t i_limit = (int32_t)(((int64_t)pid->out_max << PID_SHIFT)
                                            - p_term - d_term) / pid->Ki;
                if (pid->integral > i_limit && i_limit > 0)
                    pid->integral = i_limit;
                out = pid->out_max;
            }
            else if (out < pid->out_min) {
                int32_t i_limit = (int32_t)(((int64_t)pid->out_min << PID_SHIFT)
                                            - p_term - d_term) / pid->Ki;
                if (pid->integral < i_limit && i_limit < 0)
                    pid->integral = i_limit;
                out = pid->out_min;
            }
        }

        pid->err_last = err;
    }
    else {
        /* --- 增量式 PID 逻辑 --- */
        // delta_out = [Kp(e-e_l) + Ki*e + Kd(e-2e_l+e_p)] >> 10
        int32_t delta_out = (pid->Kp * (err - pid->err_last) +
                             pid->Ki * err +
                             pid->Kd * (err - 2 * pid->err_last + pid->err_prev)) >> PID_SHIFT;

        pid->current_out += delta_out;
        out = pid->current_out;

        // 状态滚动
        pid->err_prev = pid->err_last;
        pid->err_last = err;
    }

    /* 统一最终输出限幅 */
    if (out > pid->out_max) out = pid->out_max;
    else if (out < pid->out_min) out = pid->out_min;

    // 同步增量式的当前输出，防止限幅失效
    if (pid->mode == PID_MODE_INCREMENTAL) pid->current_out = out;

    return out;
}