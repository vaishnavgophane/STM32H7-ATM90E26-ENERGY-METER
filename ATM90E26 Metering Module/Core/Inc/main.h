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


/* ====== Metering ======= */
#define PLconstH 0x0015
#define PLconstL 0xD174
#define Lgain 0x0000		// 0x23 = L Line Gain

/* ====== Mesurement ======= */

#define IgainL 0x3DBC 	//3F4C    // L Line Current rms Gain(0x32)
#define IoffsetL 0x0000 // L Line Current Offset(0x35)

//
//#define SoftReset 0x00 // Software Reset
//#define SysStatus 0x01 // System Status
//#define FuncEn 0x02    // Function Enable
//#define SagTh 0x03     // Voltage Sag Threshold
//#define SmallPMod 0x04 // Small-Power Mode
//#define LastData 0x06  // Last Read/Write SPI/UART Value
//#define LSB 0x08       // RMS/Power 16-bit LSB
//#define CalStart 0x20  // Calibration Start Command
//#define PLconstH 0x21  // High Word of PL_Constant
//#define PLconstH_Default 0x0015
//#define PLconstL 0x22 // Low Word of PL_Constant
//#define PLconstL_Default 0xD174
//#define Lgain 0x23 // L Line Calibration Gain
//#define Lgain_Default 0x0000
//#define Lphi 0x24 // L Line Calibration Angle
//#define Lphi_Default 0x0000
//#define Ngain 0x25 // N Line Calibration Gain
//#define Ngain_Default 0x0000
//#define Nphi 0x26 // N Line Calibration Angle
//#define Nphi_Default 0x0000
//#define PStartTh 0x27 // Active Startup Power Threshold
//#define PStartTh_Default 0x08BD
//#define PNolTh 0x28 // Active No-Load Power Threshold
//#define PNolTh_Default 0x0000
//#define QStartTh 0x29 // Reactive Startup Power Threshold
//#define QStartTh_Default 0x0AEC
//#define QNolTh 0x2A // Reactive No-Load Power Threshold
//#define QNolTh_Default 0x0000
//#define MMode 0x2B // Metering Mode Configuration
//#define MMode_Default 0x9422
//#define CSOne 0x2C    // Checksum 1
//#define AdjStart 0x30 // Measurement Calibration Start Command
//#define Ugain 0x31    // Voltage rms Gain
//#define Ugain_Default 0x6720
//#define IgainL 0x32 // L Line Current rms Gain
//#define IgainL_Default 0x7A13
//#define IgainN 0x33 // N Line Current rms Gain
//#define IgainN_Default 0x7530
//#define Uoffset 0x34 // Voltage Offset
//#define Uoffset_Default 0x0000
//#define IoffsetL 0x35 // L Line Current Offset
//#define IoffsetL_Default 0x0000
//#define IoffsetN 0x36 // N Line Current Offse
//#define IoffsetN_Default 0x0000
//#define PoffsetL 0x37 // L Line Active Power Offset
//#define PoffsetL_Default 0x0000
//#define QoffsetL 0x38 // L Line Reactive Power Offset
//#define QoffsetL_Default 0x0000
//#define PoffsetN 0x39 // N Line Active Power Offset
//#define PoffsetN_Default 0x0000
//#define QoffsetN 0x3A // N Line Reactive Power Offset
//#define QoffsetN_Default 0x0000
//#define CSTwo 0x3B     // Checksum 2
//#define APenergy 0x40  // Forward Active Energy
//#define ANenergy 0x41  // Reverse Active Energy
//#define ATenergy 0x42  // Absolute Active Energy
//#define RPenergy 0x43  // Forward (Inductive) Reactive Energy
//#define Rnenerg 0x44   // Reverse (Capacitive) Reactive Energy
//#define Rtenergy 0x45  // Absolute Reactive Energy
//#define EnStatus 0x46  // Metering Status
//#define Irms 0x48      // L Line Current rms
//#define Urms 0x49      // Voltage rms
//#define Pmean 0x4A     // L Line Mean Active Power
//#define Qmean 0x4B     // L Line Mean Reactive Power
//#define Frequency 0x4C      // Voltage Frequency
//#define PowerF 0x4D    // L Line Power Factor
//#define Pangle 0x4E    // Phase Angle between Voltage and L Line Current
//#define Smean 0x4F     // L Line Mean Apparent Power
//#define IrmsTwo 0x68   // N Line Current rms
//#define PmeanTwo 0x6A  // N Line Mean Active Power
//#define QmeanTwo 0x6B  // N Line Mean Reactive Power
//#define PowerFTwo 0x6D // N Line Power Factor
//#define PangleTwo 0x6E // Phase Angle between Voltage and N Line Current
//#define SmeanTwo 0x6F  // N Line Mean Apparent Power


/* USER CODE BEGIN Private defines */
#define CS_LOW()   HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH()  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
