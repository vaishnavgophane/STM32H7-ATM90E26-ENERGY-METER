/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "20x4_LCD.h"
#include "ATM90E26_Metering.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Calculate how many items are in the menu automatically
#define MENU_SIZE (sizeof(menuItems) / sizeof(menuItems[0]))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

							/*======= System Error Indicating Variable =======*/

uint16_t System_Status;

							/*======= Register Read Variables =======*/
float vRMS;
float iRMS;
float Freq;
float Apparent_Power;
float Aprt_Pwr;
uint16_t rawPF;
uint16_t  lPF;
float Reactive_Power;
float Ract_Pwr;
float Active_Power;
float Act_Pwr;
int16_t Phase_Angle;
float signedPF;
char line[32];
float scaling_factor;

									/*======= Checksum Variables =======*/

uint16_t cs1 = 0;		// Explicite CheckSum1 Calculation
uint16_t cs2 = 0;		// Explicite CheckSum1 Calculation
uint16_t CS1 = 0;		// Implicite CheckSum1 Calculation
uint16_t CS2 = 0;		// Implicite CheckSum2 Calculation

								/*======= Function Variables for Debug =======*/
uint8_t rx_buf[3] = {0};
volatile char line2[30] = {0};
volatile char line1[30] = {0};

								/*======= Checksum Calculation Arrays =======*/

uint16_t meteringBlock[11] = {PLconstH, PLconstL, Lgain, 0x0000, 0x0000, 0x0000, 0x08BD, 0x0000, 0x0AEC, 0x0000, 0x9422};
uint16_t measurementBlock[10] = {0x6720, IgainL, 0x7530, 0x0000, IoffsetL, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

								/* ===== Keypad Variables ===== */
typedef enum {
    STATE_WELCOME,
    STATE_MENU,
    STATE_LIVE_DATA
} SystemState_t;
uint8_t x = 0;           // Bitmask (0000)
uint8_t key = 0;		// Maseked Reciever
SystemState_t currentState = STATE_WELCOME;
int cursorLine = 0; // 0 to 3 for our 4 menu items
const char *menuItems[] = {
    "RMS Data",
    "PF & Frequency",
    "Ang & Apparent",
    "Act/React Power"
};

uint8_t updateDisplay = 1; // Start with 1 to draw the first time
static uint32_t lastUpdate = 0;


typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} LED_t;

LED_t leds[16] =
{
    {RLY1_GPIO_Port, RLY1_Pin},
    {RLY2_GPIO_Port, RLY2_Pin},
    {RLY3_GPIO_Port, RLY3_Pin},
    {RLY4_GPIO_Port, RLY4_Pin},

    {RLY5_GPIO_Port, RLY5_Pin},
    {RLY6_GPIO_Port, RLY6_Pin},
    {RLY7_GPIO_Port, RLY7_Pin},
    {RLY8_GPIO_Port, RLY8_Pin},

    {RLY9_GPIO_Port, RLY9_Pin},
    {RLY10_GPIO_Port, RLY10_Pin},
    {RLY11_GPIO_Port, RLY11_Pin},
    {RLY12_GPIO_Port, RLY12_Pin},

    {RLY13_GPIO_Port, RLY13_Pin},
    {RLY14_GPIO_Port, RLY14_Pin},
    {RLY16_GPIO_Port, RLY16_Pin},
    {RLY15_GPIO_Port, RLY15_Pin}
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void   ATM90E26_Init();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(100);
  LCD_Init();

//  LED_All_On();
//  HAL_Delay(1000);
//  LED_All_Off();
//
//  LCD_SetCursor(0,5);
//  LCD_Print("Welcome,");
//  LCD_SetCursor(1,2);
//  LCD_Print("System Online!");
//  LCD_SetCursor(2, 4);
//  LCD_Print("~ ShineArc Tech!");
//  HAL_Delay(2000);
  LCD_Clear();
  currentState = STATE_MENU;

  ATM90E26_Init();

  ATM90E26_Metering_Enable();

  ATM90E26_Mesurement_Enable();

  x = 0x00; // Start with the first bit set (Line 0)
  updateDisplay = 1;
  // 1. Read registers

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_Delay(1000);
	  ATM90_Get_Values();

	  if(currentState == STATE_MENU) {

		          if (updateDisplay == 1) {
		        	  LCD_SendCommand(0x0F);		// Command: Display ON, Cursor ON, Blinking ON
		              Display_20x4_Menu();

		              LCD_SetCursor(cursorLine, 0);
		              updateDisplay = 0;
		          }
		          key = Keypad_Read();

		          switch(key)
		          {
		              case 0x0E:   // UP
		            	  cursorLine = (cursorLine == 0) ? 3 : cursorLine - 1;
		              	  updateDisplay = 1;
		                  break;

		              case 0x0D:   // DOWN
		            	  cursorLine = (cursorLine + 1) % 4;
		              	  updateDisplay = 1;
		                  break;

		              case 0x0B:   // ENTER
		              	currentState = STATE_LIVE_DATA;
		              	LCD_Clear();
		                  break;

		              case 0x07:   // ESC
		                  currentState = STATE_MENU;
		                  updateDisplay = 1;
		                  LCD_Clear();
		                  break;
		          }
	  }
	  else if(currentState == STATE_LIVE_DATA) {
		    key = Keypad_Read();

		    if(key == 0x07)        // ESC pressed
		    {
		        currentState = STATE_MENU;
		        updateDisplay = 1;
		        LCD_Clear();
		    }

	        /* ======= 3. DISPLAY LIVE VALUES ======= */

	        	  LCD_SendCommand(0x0C); // Blinking OFF in live Data
		          if (HAL_GetTick() - lastUpdate > 500) {
		              Display_Live_Values(cursorLine);
		              lastUpdate = HAL_GetTick();
		          }
	    	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /*AXI clock gating */
  RCC->CKGAENR = 0xE003FFFF;

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV64;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RLY1_Pin|RLY2_Pin|RLY3_Pin|RLY4_Pin
                          |RLY9_Pin|RLY10_Pin|D7_Pin|D4_Pin
                          |D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RLY5_Pin|RLY6_Pin|RLY7_Pin|RLY8_Pin
                          |SPI_CS_Pin|D6_Pin|EN_Pin|RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RLY11_Pin|RLY12_Pin|RLY13_Pin|RLY14_Pin
                          |RLY15_Pin|RLY16_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RLY1_Pin RLY2_Pin RLY3_Pin RLY4_Pin
                           RLY9_Pin RLY10_Pin D7_Pin D4_Pin
                           D5_Pin */
  GPIO_InitStruct.Pin = RLY1_Pin|RLY2_Pin|RLY3_Pin|RLY4_Pin
                          |RLY9_Pin|RLY10_Pin|D7_Pin|D4_Pin
                          |D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RLY5_Pin RLY6_Pin RLY7_Pin RLY8_Pin
                           D6_Pin EN_Pin RS_Pin */
  GPIO_InitStruct.Pin = RLY5_Pin|RLY6_Pin|RLY7_Pin|RLY8_Pin
                          |D6_Pin|EN_Pin|RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_CS_Pin */
  GPIO_InitStruct.Pin = SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RLY11_Pin RLY12_Pin RLY13_Pin RLY14_Pin
                           RLY15_Pin RLY16_Pin */
  GPIO_InitStruct.Pin = RLY11_Pin|RLY12_Pin|RLY13_Pin|RLY14_Pin
                          |RLY15_Pin|RLY16_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DOWN_Pin ENTER_Pin ESC_Pin */
  GPIO_InitStruct.Pin = DOWN_Pin|ENTER_Pin|ESC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : UP_Pin */
  GPIO_InitStruct.Pin = UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(UP_GPIO_Port, &GPIO_InitStruct);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_CLOSE);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3, SYSCFG_SWITCH_PC3_CLOSE);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_CLOSE);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void LED_All_On(void)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_SET);
    }
}
void LED_All_Off(void)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        HAL_GPIO_WritePin(leds[i].port, leds[i].pin, GPIO_PIN_RESET);
    }
}
void LED_All_Toggle(void)
{
    for (uint8_t i = 1; i <= 16; i++)
    {
    	HAL_GPIO_WritePin(leds[i].port, leds[i].pin,SET);
    	HAL_Delay(100);
        HAL_GPIO_WritePin(leds[i-1].port, leds[i-1].pin,RESET);
        HAL_Delay(100);
    }
}
void LED_All_Toggle_Rev(void)
{
    for (int8_t i = 16; i >= 1; i--)
    {
    	HAL_GPIO_WritePin(leds[i].port, leds[i].pin,SET);
    	HAL_Delay(100);
    	HAL_GPIO_WritePin(leds[i-1].port, leds[i-1].pin,RESET);
    	HAL_Delay(100);
    }
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
