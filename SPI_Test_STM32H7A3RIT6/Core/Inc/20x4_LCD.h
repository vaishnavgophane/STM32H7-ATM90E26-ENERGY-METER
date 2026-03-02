/*
 * 16x2_LCD.h
 *
 *  Created on: Feb 23, 2026
 *      Author: Vaishnav
 */

#ifndef INC_20X4_LCD_H_
#define INC_20X4_LCD_H_

#include "stm32h7xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "main.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);
void LCD_PrintChar(char c);
void Display_Live_Values(int selection);
#endif /* INC_20X4_LCD_H_ */
