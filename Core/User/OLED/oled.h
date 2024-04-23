//
// Created by nov4ou on 2024/4/23.
//

#ifndef OLED_DISPLAY_FOR_STM32_OLED_H
#define OLED_DISPLAY_FOR_STM32_OLED_H

#include "main.h"

#define PAGE_SIZE    8
#define XLevelL       0x02
#define XLevelH       0x10
#define YLevel       0xB0
#define  Brightness   0xFF
#define WIDTH       128
#define HEIGHT       64

#define OLED_CMD     0          // Command
#define OLED_dataA    1          // dataa

#define OLED_CS   GPIO_PIN_9    // Select
#define OLED_DC   GPIO_PIN_10   // dataa, command control
#define OLED_RST  GPIO_PIN_12   // Reset

#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOA, OLED_CS, GPIO_PIN_RESET)
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOA, OLED_CS, GPIO_PIN_SET)

#define OLED_DC_Clr()  HAL_GPIO_WritePin(GPIOA, OLED_DC, GPIO_PIN_RESET)
#define OLED_DC_Set()  HAL_GPIO_WritePin(GPIOA, OLED_DC, GPIO_PIN_SET)

#define OLED_RST_Clr()  HAL_GPIO_WritePin(GPIOA, OLED_RST, GPIO_PIN_RESET)
#define OLED_RST_Set()  HAL_GPIO_WritePin(GPIOA, OLED_RST, GPIO_PIN_SET)

void OLED_WR_Byte(uint8_t data,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Reset(void);
void OLED_Init(void);
void OLED_Set_Pixel(uint8_t x, uint8_t y, uint8_t color);
void OLED_Display(void);
void OLED_Clear(uint8_t data);

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Size, uint8_t mode);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t mode);

#endif //OLED_DISPLAY_FOR_STM32_OLED_H
