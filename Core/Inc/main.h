/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GIN7_Pin GPIO_PIN_14
#define GIN7_GPIO_Port GPIOC
#define GIN8_Pin GPIO_PIN_15
#define GIN8_GPIO_Port GPIOC
#define E1A_Pin GPIO_PIN_0
#define E1A_GPIO_Port GPIOA
#define E1B_Pin GPIO_PIN_1
#define E1B_GPIO_Port GPIOA
#define GIN1_Pin GPIO_PIN_2
#define GIN1_GPIO_Port GPIOA
#define GIN2_Pin GPIO_PIN_3
#define GIN2_GPIO_Port GPIOA
#define GIN3_Pin GPIO_PIN_4
#define GIN3_GPIO_Port GPIOA
#define GIN4_Pin GPIO_PIN_5
#define GIN4_GPIO_Port GPIOA
#define E2A_Pin GPIO_PIN_6
#define E2A_GPIO_Port GPIOA
#define E2B_Pin GPIO_PIN_7
#define E2B_GPIO_Port GPIOA
#define GIN5_Pin GPIO_PIN_0
#define GIN5_GPIO_Port GPIOB
#define GIN6_Pin GPIO_PIN_1
#define GIN6_GPIO_Port GPIOB
#define PWMA_Pin GPIO_PIN_8
#define PWMA_GPIO_Port GPIOA
#define PWMB_Pin GPIO_PIN_9
#define PWMB_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_11
#define AIN1_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_12
#define AIN2_GPIO_Port GPIOA
#define BIN1_Pin GPIO_PIN_4
#define BIN1_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_5
#define BIN2_GPIO_Port GPIOB
#define ECHO_Pin GPIO_PIN_6
#define ECHO_GPIO_Port GPIOB
#define TRIG_Pin GPIO_PIN_7
#define TRIG_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
