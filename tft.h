#ifndef __tft_h
#define __tft_h
#include "main.h"
#define CS_TFT_LO         (GPIOB->BRR   |= (1<<12))
#define CS_TFT_HI         (GPIOB->BSRR  |= (1<<12))
#define DC_TFT_DATA       (GPIOA->BSRR  |=  (1<<8))
#define DC_TFT_COMMAND    (GPIOA->BRR   |=  (1<<8))
#define DC_TFT_RES_LO     (GPIOA->BRR   |=  (1<<9))
#define DC_TFT_RES_HI     (GPIOA->BSRR  |=  (1<<9))
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PINK    0x90F2 
void init_tft(void);
void TFT9341_FillScreen(uint16_t color);
void TFT24_SET_Column_SET_Page(uint16_t column_X1, uint16_t page_Y1, uint16_t column_X2, uint16_t page_Y2);
 
#endif  

