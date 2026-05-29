//
// Created by Administrator on 2026/5/29.
//

#include "main_entry.h"

#include "tim.h"
#include "../../Lib/oled/OLED.h"
#include "../timer/timer.h"

void main_entry(void) {
    HAL_Delay(50);  // wait for OLED power-up
    OLED_Init();

    HAL_TIM_Base_Start_IT(&htim4);


    /*在(0, 0)位置显示字符'A'，字体大小为8*16点阵*/
    OLED_ShowChar(0, 0, 'A', OLED_8X16);

    /*在(16, 0)位置显示字符串"Hello World!"，字体大小为8*16点阵*/
    OLED_ShowString(16, 0, "Hello World!", OLED_8X16);

    /*在(0, 18)位置显示字符'A'，字体大小为6*8点阵*/
    OLED_ShowChar(0, 18, 'A', OLED_6X8);

    /*在(16, 18)位置显示字符串"Hello World!"，字体大小为6*8点阵*/
    OLED_ShowString(16, 18, "Hello World!", OLED_6X8);

    /*在(0, 28)位置显示数字12345，长度为5，字体大小为6*8点阵*/
    OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);

    /*在(40, 28)位置显示有符号数字-66，长度为2，字体大小为6*8点阵*/
    OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);

    /*在(70, 28)位置显示十六进制数字0xA5A5，长度为4，字体大小为6*8点阵*/
    OLED_ShowHexNum(70, 28, 0xA5A5, 4, OLED_6X8);

    /*在(0, 38)位置显示二进制数字0xA5，长度为8，字体大小为6*8点阵*/
    OLED_ShowBinNum(0, 38, 0xA5, 8, OLED_6X8);

    /*在(60, 38)位置显示浮点数字123.45，整数部分长度为3，小数部分长度为2，字体大小为6*8点阵*/
    OLED_ShowFloatNum(60, 38, 123.45, 3, 2, OLED_6X8);

    /*在(0, 48)位置显示英文和汉字串"Hello,世界。"，支持中英文混写*/
    OLED_ShowString(0, 48, "Hello,世界。", OLED_8X16);

    /*在(96, 48)位置显示图像，宽16像素，高16像素，图像数据为Diode数组*/
    OLED_ShowImage(96, 48, 16, 16, Diode);

    /*在(96, 18)位置打印格式化字符串，字体大小为6*8点阵，格式化字符串为"[%02d]"*/
    OLED_Printf(96, 18, OLED_6X8, "[%02d]", 6);

    /*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
    OLED_Update();
    for (;;) {
        if (currentMillis - lastMillis > 1000) {
            lastMillis = currentMillis;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
    }
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        currentMillis += 10;
    }
}