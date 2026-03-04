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
void Display_20x4_Menu(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
uint8_t Keypad_Read(void);


#endif /* INC_20X4_LCD_H_ */
