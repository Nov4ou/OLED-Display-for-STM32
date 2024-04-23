//
// Created by nov4ou on 2024/4/23.
//

#include "oled.h"
#include "oledfont.h"
#include <string.h>
#include <stdint-gcc.h>
#include "spi.h"


static uint8_t OLED_buffer[1024];

void OLED_WR_Byte(uint8_t data, uint8_t cmd) {
  if (cmd) {
    OLED_DC_Set();
  } else {
    OLED_DC_Clr();
  }
  OLED_CS_Clr();
  HAL_SPI_Transmit(&hspi1, &data, sizeof(data), HAL_MAX_DELAY);
  OLED_CS_Set();
}

void OLED_Set_Pos(uint8_t x, uint8_t y) {
  OLED_WR_Byte(YLevel + y / PAGE_SIZE, OLED_CMD);
  OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}

void OLED_Display_On(void) {
  OLED_WR_Byte(0X8D, OLED_CMD);  //SET DC
  OLED_WR_Byte(0X14, OLED_CMD);  //DC ON
  OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}

void OLED_Display_Off(void) {
  OLED_WR_Byte(0X8D, OLED_CMD);  //SET DC
  OLED_WR_Byte(0X10, OLED_CMD);  //DC OFF
  OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

void OLED_Set_Pixel(uint8_t x, uint8_t y, uint8_t color) {
  if (color)
    OLED_buffer[(y / PAGE_SIZE) * WIDTH + x] |= (1 << (y % PAGE_SIZE)) & 0xff;
  else
    OLED_buffer[(y / PAGE_SIZE) * WIDTH + x] &= ~((1 << (y % PAGE_SIZE)) & 0xff);
}

void OLED_Display(void) {
  // Loop through each page
  for (uint8_t i = 0; i < PAGE_SIZE; i++) {
    // Set page address (0~7)
    OLED_WR_Byte(YLevel + i, OLED_CMD);
    // Set display position - lower column address
    OLED_WR_Byte(XLevelL, OLED_CMD);
    // Set display position - higher column address
    OLED_WR_Byte(XLevelH, OLED_CMD);
    // Loop through each column within the current page
    for (uint8_t n = 0; n < WIDTH; n++) {
      // Write dataa byte to OLED display
      OLED_WR_Byte(OLED_buffer[i * WIDTH + n], OLED_dataA);
    }
  }
  // Updatae the display
}


void OLED_Clear(uint8_t data) {
  if (data)
    memset(OLED_buffer, 0xff, sizeof(OLED_buffer));
  else
    memset(OLED_buffer, 0, sizeof(OLED_buffer));
  OLED_Display();
}

void OLED_Reset(void) {
  OLED_RST_Set();
  HAL_Delay(100);
  OLED_RST_Clr();
  HAL_Delay(100);
  OLED_RST_Set();
}

void OLED_Init(void) {
  OLED_Reset();
  // Init SH1106
  OLED_Display_Off(); //power off
  OLED_WR_Byte(0xAE, OLED_CMD);//--turn off oled panel
  OLED_WR_Byte(0x02, OLED_CMD);//---set low column address
  OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
  OLED_WR_Byte(0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81, OLED_CMD);//--set contrast control register
  OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WR_Byte(0xC8, OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WR_Byte(0xA6, OLED_CMD);//--set normal display
  OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f, OLED_CMD);//--1/64 duty
  OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00, OLED_CMD);//-not offset
  OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9, OLED_CMD);//--set pre-charge period
  OLED_WR_Byte(0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA, OLED_CMD);//--set com pins hardware configuration
  OLED_WR_Byte(0x12, OLED_CMD);
  OLED_WR_Byte(0xDB, OLED_CMD);//--set vcomh
  OLED_WR_Byte(0x40, OLED_CMD);//Set VCOM Deselect Level
  OLED_WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02, OLED_CMD);//
  OLED_WR_Byte(0x8D, OLED_CMD);//--set Charge Pump enable/disable
  OLED_WR_Byte(0x14, OLED_CMD);//--set(0x10) disable
  OLED_WR_Byte(0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6, OLED_CMD);// Disable Inverse Display On (0xa6/a7)
  OLED_WR_Byte(0xAF, OLED_CMD);//--turn on oled panel
  OLED_Display_On(); // power on
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color) {
  OLED_Set_Pixel(x, y, color);
  OLED_Display();
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Size, uint8_t mode) {
  uint8_t c = 0, i = 0, tmp, j = 0;
  c = chr - ' ';
  if (x > WIDTH - 1) {
    x = 0;
    y = y + 2;
  }
  if (Size == 16) {
    for (i = 0; i < 16; i++) {
      if (mode) {
        tmp = F8X16[c * 16 + i];
      } else {
        tmp = ~(F8X16[c * 16 + i]);
      }
      for (j = 0; j < 8; j++) {
        if (tmp & (0x80 >> j)) {
          OLED_Set_Pixel(x + j, y + i, 1);
        } else {
          OLED_Set_Pixel(x + j, y + i, 0);
        }
      }
    }
  } else if (Size == 8) {
    for (i = 0; i < 8; i++) {
      if (mode) {
        tmp = F6x8[c][i];
      } else {
        tmp = ~(F6x8[c][i]);
      }
      for (j = 0; j < 8; j++) {
        if (tmp & (0x80 >> j)) {
          OLED_Set_Pixel(x + j, y + i, 1);
        } else {
          OLED_Set_Pixel(x + j, y + i, 0);
        }
      }
    }
  } else {
    return;
  }
  OLED_Display();
}

void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size, uint8_t mode) {
  unsigned char j = 0, csize;
  if (Char_Size == 16) {
    csize = Char_Size / 2;
  } else if (Char_Size == 8) {
    csize = Char_Size / 2 + 2;
  } else {
    return;
  }
  while (chr[j] != '\0') {
    OLED_ShowChar(x, y, chr[j], Char_Size, mode);
    x += csize;
    if (x > 120) {
      x = 0;
      y += Char_Size;
    }
    j++;
  }
}

