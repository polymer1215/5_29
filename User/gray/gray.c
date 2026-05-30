#include "gray.h"

#include "stm32f1xx_hal_gpio.h"
#include "main.h"

volatile GPIO_PinState L1 = 0;
volatile GPIO_PinState L2 = 0;
volatile GPIO_PinState L3 = 0;
volatile GPIO_PinState L4 = 0;
volatile GPIO_PinState L5 = 0;
volatile GPIO_PinState L6 = 0;
volatile GPIO_PinState L7 = 0;
volatile GPIO_PinState L8 = 0;
volatile uint8_t gray_state = 0;

void update_gray_state() {

    L1 = HAL_GPIO_ReadPin(GIN1_GPIO_Port, GIN1_Pin);
    L2 = HAL_GPIO_ReadPin(GIN2_GPIO_Port, GIN2_Pin);
    L3 = HAL_GPIO_ReadPin(GIN3_GPIO_Port, GIN3_Pin);
    L4 = HAL_GPIO_ReadPin(GIN4_GPIO_Port, GIN4_Pin);
    L5 = HAL_GPIO_ReadPin(GIN5_GPIO_Port, GIN5_Pin);
    L6 = HAL_GPIO_ReadPin(GIN6_GPIO_Port, GIN6_Pin);
    L7 = HAL_GPIO_ReadPin(GIN7_GPIO_Port, GIN7_Pin);
    L8 = HAL_GPIO_ReadPin(GIN8_GPIO_Port, GIN8_Pin);

    gray_state = (uint8_t)(~((L1 << 7) | (L2 << 6) | (L3 << 5) | (L4 << 4) | (L5 << 3) | (L6 << 2) | (L7 << 1) | (L8 << 0)));
}