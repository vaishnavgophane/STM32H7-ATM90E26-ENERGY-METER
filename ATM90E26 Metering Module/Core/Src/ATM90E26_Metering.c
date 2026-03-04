/*
 * Ram_Debugger.c
 *
 *  Created on: Feb 16, 2026
 *      Author: Vaishnav
 */

#include "main.h"
#include "stm32h7xx_hal.h"

#include "ATM90E26_Metering.h"
#include <stdio.h>
#include <string.h>
extern SPI_HandleTypeDef hspi1;

		/* ======= System Error Indicating Variable =========== */
extern uint16_t System_Status;

		/* ======= Register Read Variables =========== */

extern float vRMS;
extern float  iRMS;
extern float Freq;
extern float Apparent_Power;
extern uint16_t lPF;
extern float Reactive_Power;
extern float Active_Power;
extern float  signedPF;
extern int16_t Phase_Angle;
extern char line[32];

		/* ========= Checksum Variables ========= */

extern uint16_t cs1;		// Explicit CheckSum1 Calculation
extern uint16_t cs2;		// Explicit CheckSum2 Calculation
extern uint16_t CS1;		// Implicit CheckSum1 Calculation
extern uint16_t CS2;		// Implicit CheckSum2 Calculation

		/* ========= Function Variables for Debug ========= */

extern uint8_t rx_buf[3];
extern volatile char line2[30];
extern volatile char line1[30];

		/* ========= Checksum Calculation Arrays ========= */

extern uint16_t meteringBlock[11];
extern uint16_t measurementBlock[10];


void ATM90_Get_Values(void) {
	  vRMS = ATM90_ReadReg(0x49)/ 100.0f;
// 1. Vrms
	  vRMS = (float)ATM90_ReadReg(0x49)/ 100.0f;// The concept on repeted reading is to tackel the value refresh error.(650,0,0,0,0.....)

// 2. Irms
	  iRMS = (float)ATM90_ReadReg(0x48) / 260.0f;
// 3. Power Factor
	  lPF = (float)ATM90_ReadReg(0x4D);
	  //  Check the Sign Bit (Bit 15)
	  if (lPF & 0x8000) {
	  // If Bit 15 is 1, the value is negative (Leading/Capacitive)
	  // Masking out the sign bit, convert to float, and make it negative.
	  signedPF = (float)(lPF & 0x7FFF) / -1000.0f;
	  } else {
	  // If Bit 15 is 0, the value is positive (Lagging/Inductive)
	  signedPF = (float)lPF / 1000.0f;
	  }
	  //  Safety Check: PF cannot mathematically exceed 1.0
	  if (signedPF > 1.0f) signedPF = 1.0f;

	  if (signedPF < -1.0f) signedPF = -1.0f;

// 4. Frequency
	  Freq = (float)ATM90_ReadReg(0x4C)/100;

// 5. Aparent Power (S) : Unit - Volts Ampear(VA)
	  Apparent_Power = (float)ATM90_ReadReg(0x4F);

// 6. Active Power (P) : Unit - Watt (W)
	  Active_Power = (float)ATM90_ReadReg(0x4A) * 1.0f;

// 7. Reactive Power (Q) : Unit - VAR
	  Reactive_Power = (float)ATM90_ReadReg(0x4B);

// 8. Phase angle
	  Phase_Angle = (float)ATM90_ReadReg(0x4E)/100;
	  HAL_Delay(1);
	  Phase_Angle = (float)ATM90_ReadReg(0x4E)/100;
	  System_Status = ATM90_ReadReg(0x01); // Represents '0' if no errors in system or Checksum's
}
uint16_t CalculateChecksum(uint16_t *regs, int length) {
    uint8_t lowSum = 0;   // For the Addition (Modulo 256)
    uint8_t highXor = 0;  // For the XOR logic

    for (int i = 0; i < length; i++) {
        // Extract High Byte and Low Byte of the 16-bit register
        uint8_t highByte = (uint8_t)(regs[i] >> 8);
        uint8_t lowByte  = (uint8_t)(regs[i] & 0xFF);

        // Update Sum (automatically overflows at 255/Modulo 256)
        lowSum += highByte;
        lowSum += lowByte;

        // Update XOR
        highXor ^= highByte;
        highXor ^= lowByte;
    }

    // Combine: XOR result becomes High Byte, Sum result becomes Low Byte
    return ((uint16_t)highXor << 8) | lowSum;
}
uint16_t ATM90_ReadReg(uint8_t addr) {
	uint8_t tx_buf[3] = { (addr | 0x80), 0x00, 0x00 }; // MSB = 1 for Read [cite: 73]
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

    // Blocking call: HAL handles the H7 SPI FIFO internally
    if (HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, 3, 10) != HAL_OK) {
        HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
        return 0xFFFF;
    }

    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);

    // M90E26 shifts data out on cycles 9-24 [cite: 77]
    return (uint16_t)((rx_buf[1] << 8) | rx_buf[2]);
}

void WriteReg(uint8_t addr, uint16_t val) {
    uint8_t tx_buf[3];
    tx_buf[0] = addr & 0x7F; // MSB = 0 for Write
    tx_buf[1] = (val >> 8) & 0xFF;
    tx_buf[2] = val & 0xFF;

    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);

    // Transmit in Blocking Mode
    HAL_SPI_Transmit(&hspi1, tx_buf, 3, 10);

    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(4);
}
void ATM90E26_Init(void) {

	// 1. Power-on Delay
    HAL_Delay(100);

    // 2. Soft Reset
    WriteReg(0x00, 0x789A);
    HAL_Delay(50);

    // 3. Functional Configuration (FuncEn) = 0x000C is the default. 0x0000 disables direction change interrupts.
    WriteReg(0x02, 0x0000);
    HAL_Delay(20);
}
void ATM90E26_Metering_Enable(void)
{
	/***************************************** Metering Calibration Registers (Unlocked by CalStart) ******************************************/

	  WriteReg(0x20, 0x5678);	// CalStart Unlocks Metering Registers

	  /* Setting the pulse constant. Even for raw values, the chip needs a timebase to integrate power into energy. */
	  WriteReg(0x21, 0x0015);	// PL_constH [ Default Value ]

	  WriteReg(0x22, 0xD174);	// PL_constL [Default Value ]

	  /* Sets the fundamental gain for the Line current channels. */
	  WriteReg(0x23, 0x1D39);	//  L Line Calibration Gain [ Default Value ]

	  /* Sets phase compensation. For raw readings, keeping this at zero ensures no phase shifting is applied. */
	  WriteReg(0x24, 0x0000);	//  N Line Gain Angle [ Default Value ]

	  /* Sets the fundamental gain for the Neutral current channels. */
	  WriteReg(0x25, 0x0000);	//  L Line Calibration Gain [ Default Value ]

	  /* Sets phase compensation. For raw readings, keeping this at zero ensures no phase shifting is applied. */
	  WriteReg(0x26, 0x0000);	//  N Line Gain Angle [ Default Value ]

	  /* Startup Thresholds: Prevents "creeping" by requiring a minimum signal level before the IC starts measuring. */
	  WriteReg(0x27, 0x08BD);	//  Active Startup Power Threshold [ Default Value ]

	  /* No-Load Thresholds: Setting these to zero ensures the chip doesn't ignore very small loads while you are testing. */
	  WriteReg(0x28, 0x0000);	//  Active No-Load Power Threshold [ Default Value ]

	  /* Startup Thresholds: Prevents "creeping" by requiring a minimum signal level before the IC starts measuring. */
	  WriteReg(0x29, 0x0AEC);	// Reactive Startup Power Threshold [ Default Value ]

	  /* No-Load Thresholds: Setting these to zero ensures the chip doesn't ignore very small loads while you are testing. */
	  WriteReg(0x2A, 0x0000);	//  Reactive No-Load Power Threshold [ Default Value ]

	  WriteReg(0x2B, 0x9422);	//  MMode Confguration [ Default Value ]

	  cs1 = CalculateChecksum(meteringBlock, 11);

	  WriteReg(0x2C, cs1);     	// CS1 Register
	  HAL_Delay(100);

	    CS1 = ATM90_ReadReg(0x2C);

	  WriteReg(0x20, 0x8765);	// Metering Values Locked, CalStart Register will Check the CheckSum1 value written to the Checksum Register
	  	  	  	  	  	  	  	 // & it is compared to the value that the IC Calculates internally. If both missmatchs then CalErr is trigerred

	  /*********************************************************************************************************************************/
}
void ATM90E26_Mesurement_Enable(void)
{
	  /***************************************** Mesurement Registers (Unlocked by AdjStart) ******************************************/

	  WriteReg(0x30, 0x5678);		// AdjStart Uncloed

	  WriteReg(0x31, 0x6720);		// Voltage rms Gain

	  WriteReg(0x32, 0x7A13);		// L Line Current rms Gain

	  WriteReg(0x33, 0x7530);		// N Line Current rms Gain [ Default Value ]

	  WriteReg(0x34, 0x0000);		// Voltage Offset [ Default Value ]

	  WriteReg(0x35, 0x0000);		// L Line Current Offset [ Default Value ]

	  WriteReg(0x36, 0x0000);		// N Line Current Offset [ Default Value ]

	  WriteReg(0x37, 0x0000);		//  L Line Active Power Offset [ Default Value ]

	  WriteReg(0x38, 0x0000);		//  L Line Reactive Power Offset [ Default Value ]

	  WriteReg(0x39, 0x0000);		//  N Line Active Power Offset [ Default Value ]

	  WriteReg(0x3A, 0x0000);		//  N Line Reactive Power Offset [ Default Value ]

	  cs2 = CalculateChecksum(measurementBlock, 10);

	  WriteReg(0x3B, cs2);     	// CS1 Register
	  HAL_Delay(100);

	    CS2 = ATM90_ReadReg(0x3B);

	/* Mesurement Registers Locked, AdjStart Register will Check the CheckSum1 value written to the Checksum Register
	 & it is compared to the value that the IC Calculates internally. If both missmatchs then CalErr is trigerred */
	  WriteReg(0x30, 0x8765);		// AdjStart Locked
	  HAL_Delay(20);
}
