/*
 * 16x2_LCD.c
 *
 *  Created on: Feb 23, 2026
 *      Author: Vaishnav
 */
#include "main.h"
#include "stm32h7xx_hal.h"
#include "20x4_LCD.h"
#include <stdio.h>

extern char line2[30];
extern char line1[30];

extern float vRMS;
extern float iRMS;
extern float signedPF;
extern float Freq;
extern float Apparent_Power;
extern float Active_Power;
extern float Reactive_Power;
extern int16_t Phase_Angle;

typedef enum {
    STATE_WELCOME,
    STATE_MENU,
    STATE_LIVE_DATA
} SystemState_t;

extern SystemState_t currentState;
extern int cursorLine; // 0 to 3 for our 4 menu items
extern char *menuItems[];

extern uint8_t x;
uint8_t base;
static void LCD_WriteNibble(uint8_t nibble);
static void LCD_Enable(void);
/* Pin Definitions */

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

/* Internal Functions */
uint8_t Keypad_Read(void)
{
	base = 0x0F;
    if (!HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin)){
    	HAL_Delay(20);
    	if(!HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin)) {
    		while(!HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin));
        return (base & ~(1 << 0)); // returns 0x0E
    	}
    }

    if (!HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin)){
    	HAL_Delay(20);
    	if(!HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin)) {
    		while(!HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin));
        return (base & ~(1 << 1)); // returns 0x0D
    	}
    }

    if (!HAL_GPIO_ReadPin(ENTER_GPIO_Port, ENTER_Pin)){
    	HAL_Delay(20);
    	if(!HAL_GPIO_ReadPin(ENTER_GPIO_Port, ENTER_Pin)) {
    		while(!HAL_GPIO_ReadPin(ENTER_GPIO_Port, ENTER_Pin));
        return (base & ~(1 << 2)); // returns 0x0B
    	}
    }

    if (!HAL_GPIO_ReadPin(ESC_GPIO_Port, ESC_Pin)){
    	HAL_Delay(20);
    	if(!HAL_GPIO_ReadPin(ESC_GPIO_Port, ESC_Pin)) {
    		while(!HAL_GPIO_ReadPin(ESC_GPIO_Port, ESC_Pin));
        return (base & ~(1 << 3)); // returns 0x07
    	}
    }

    return 0x0F; // no key

}
// Helper to convert bitmask (1,2,4,8) to index (0,1,2,3)

void Display_20x4_Menu(void) {
    char buf[21];
    for (int i = 0; i < 4; i++) {
        LCD_SetCursor(i, 0);
        // Print menu items with a consistent 2-space margin
        snprintf(buf, sizeof(buf), "%-18s", menuItems[i]);
        LCD_Print(buf);
    }
    // Final step: Move the blinking cursor back to the start of the active line
    LCD_SetCursor(cursorLine, 2);
}

void Display_Live_Values(int selection) {
    char buf[21];
    LCD_SetCursor(0, 0);

    switch(selection) {
        case 0: // RMS Data
            sprintf(buf, "V_RMS: %.2f V", vRMS); LCD_SetCursor(0,0); LCD_Print(buf);
            sprintf(buf, "I_RMS: %.3f A", iRMS); LCD_SetCursor(1,0); LCD_Print(buf);
            break;

        case 1: // PF and Frequency
            sprintf(buf, "PF: %.3f", signedPF); LCD_SetCursor(0,0); LCD_Print(buf);
            sprintf(buf, "Freq: %.1f Hz", 50.0); LCD_SetCursor(1,0); LCD_Print(buf);
            break;

        case 2: // Power Angle & Apparent
            // Calculated Apparent Power (S = V*I)
            sprintf(buf, "S: %.2f W", Apparent_Power); LCD_SetCursor(0,0); LCD_Print(buf);
            sprintf(buf, "Angle: %d \xDF""C", Phase_Angle);  LCD_SetCursor(1,0); LCD_Print(buf);
            break;

        case 3: // Active/Reactive Power
            sprintf(buf, "P: %.2f W", Active_Power);   LCD_SetCursor(0,0); LCD_Print(buf);
            sprintf(buf, "Q: %.2f VAR", Reactive_Power); LCD_SetCursor(1,0); LCD_Print(buf);
            break;
    }
}
/* Enable Pulse */

static void LCD_Enable(void)
{
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
//    HAL_Delay(1);
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);

}
/* Write 4 bits */
static void LCD_WriteNibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (nibble >> 0) & 1);
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (nibble >> 1) & 1);
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (nibble >> 2) & 1);
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (nibble >> 3) & 1);
    LCD_Enable();
}

/* Send Command */
void LCD_SendCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);

    LCD_WriteNibble(cmd >> 4);
    LCD_WriteNibble(cmd & 0x0F);

    HAL_Delay(5);
}

/* Send Data */
void LCD_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);

    LCD_WriteNibble(data >> 4);
    LCD_WriteNibble(data & 0x0F);
}

/* LCD Initialization */
void LCD_Init(void)
{
    HAL_Delay(100);

    /* Initialize LCD in 8-bit mode */
    LCD_WriteNibble(0x03);
    HAL_Delay(5);
    LCD_WriteNibble(0x03);
    HAL_Delay(1);
    LCD_WriteNibble(0x03);
    HAL_Delay(1);

    /* ============ Now forcing it into 8-bit Mode, 2 lines, 5x8 font ============= */
    LCD_WriteNibble(0x02);
    HAL_Delay(1);

    // Initialization commands
        LCD_SendCommand(0x28); // 2 Lines, 5x8 Matrix
        LCD_SendCommand(0x08); // Display off cursor off
        LCD_SendCommand(0x01); // Clear Display
        HAL_Delay(5);
        LCD_SendCommand(0x06); // Shift cursor right
        LCD_SendCommand(0x0C); // Display ON, Cursor OFF
}

/* Clear Display */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
    HAL_Delay(5);
}

/* Set Cursor */
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address;
    switch(row) {
        case 0: address = 0x80 + col; break;
        case 1: address = 0xC0 + col; break;
        case 2: address = 0x94 + col; break; // Address for 3rd line
        case 3: address = 0xD4 + col; break; // Address for 4th line
        default: address = 0x80;
    }
    LCD_SendCommand(address);
}
/* Print Character */
void LCD_PrintChar(char c)
{
    LCD_SendData(c);
}

/* Print String */
void LCD_Print(char *str)
{
    while (*str)
    {
        LCD_PrintChar(*str++);
    }
}

