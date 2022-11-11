#ifndef __touch_h
#define __touch_h
 

#include "main.h"
#define CS_TOUCH 11
#define IRQ_T    10
 
#define CS_SENSOR_LO (GPIOA->ODR &=~  (1<<CS_TOUCH)  )
#define CS_SENSOR_HI (GPIOA->ODR |=   (1<<CS_TOUCH)  )
#define pi 3.14159265358979
 void init_touch(void);

uint16_t read_touch_DataX(void);
uint16_t read_touch_DataY(void);
void touch_decode(u16 X, u16 Y);
 void run_button( unsigned short * arr);
 
#endif  


