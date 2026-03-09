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
#include "stm32h7xx_hal.h"

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
#define RLY1_Pin GPIO_PIN_0
#define RLY1_GPIO_Port GPIOC
#define RLY2_Pin GPIO_PIN_1
#define RLY2_GPIO_Port GPIOC
#define RLY3_Pin GPIO_PIN_2
#define RLY3_GPIO_Port GPIOC
#define RLY4_Pin GPIO_PIN_3
#define RLY4_GPIO_Port GPIOC
#define RLY5_Pin GPIO_PIN_0
#define RLY5_GPIO_Port GPIOA
#define RLY6_Pin GPIO_PIN_1
#define RLY6_GPIO_Port GPIOA
#define RLY7_Pin GPIO_PIN_2
#define RLY7_GPIO_Port GPIOA
#define RLY8_Pin GPIO_PIN_3
#define RLY8_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOA
#define RLY9_Pin GPIO_PIN_4
#define RLY9_GPIO_Port GPIOC
#define RLY10_Pin GPIO_PIN_5
#define RLY10_GPIO_Port GPIOC
#define RLY11_Pin GPIO_PIN_0
#define RLY11_GPIO_Port GPIOB
#define RLY12_Pin GPIO_PIN_1
#define RLY12_GPIO_Port GPIOB
#define RLY13_Pin GPIO_PIN_2
#define RLY13_GPIO_Port GPIOB
#define RLY14_Pin GPIO_PIN_10
#define RLY14_GPIO_Port GPIOB
#define RLY15_Pin GPIO_PIN_12
#define RLY15_GPIO_Port GPIOB
#define RLY16_Pin GPIO_PIN_13
#define RLY16_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_6
#define D7_GPIO_Port GPIOC
#define D4_Pin GPIO_PIN_7
#define D4_GPIO_Port GPIOC
#define D5_Pin GPIO_PIN_9
#define D5_GPIO_Port GPIOC
#define D6_Pin GPIO_PIN_8
#define D6_GPIO_Port GPIOA
#define EN_Pin GPIO_PIN_11
#define EN_GPIO_Port GPIOA
#define RS_Pin GPIO_PIN_12
#define RS_GPIO_Port GPIOA
#define DOWN_Pin GPIO_PIN_10
#define DOWN_GPIO_Port GPIOC
#define ENTER_Pin GPIO_PIN_11
#define ENTER_GPIO_Port GPIOC
#define ESC_Pin GPIO_PIN_12
#define ESC_GPIO_Port GPIOC
#define UP_Pin GPIO_PIN_2
#define UP_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
/* ====== Metering ======= */
#define PLconstH 0x0015
#define PLconstL 0xD174
#define Lgain 0x0000		// 0x23 = L Line Gain

/* ====== Mesurement ======= */

#define IgainL 0x3F4C    // L Line Current rms Gain(0x32)
#define IoffsetL 0x0000 // L Line Current Offset(0x35)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
