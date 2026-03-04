/*
 * Ram_Debugger.h
 *
 *  Created on: Feb 16, 2026
 *      Author: Vaishnav
 */

#ifndef INC_ATM90E26_METERING_H_
#define INC_ATM90E26_METERING_H_

#include "main.h"
#include "stm32h7xx_hal.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//#define Vrms 0x49
//#define Irms 0x48
//#define PF 	 0x4D
//#define Freq 0x4C
//#define Apparent_Power 0x4F
//#define Active_Power 0x4A
//#define Reactive_Power 0x4B


uint16_t ATM90_ReadReg(uint8_t addr);
void WriteReg(uint8_t addr, uint16_t val);
uint16_t CalculateChecksum(uint16_t *regs, int length);
void ATM90E26_Metering_Enable(void);
void ATM90E26_Mesurement_Enable(void);
void Calibrating_Power(void);
void ATM90_Get_Values(void);
#endif /* INC_ATM90E26_METERING_H_ */
