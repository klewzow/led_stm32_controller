#include "ws28.h"

#define VALUE_LED  50
#define DATA_LEN ((VALUE_LED*24) + 3)
#define TIM4_START (TIM4->CR1 |=  (1<<0))
#define TIM4_STOP  (TIM4->CR1 &=~ (1<<0))
//Период следования бит в тиках таймера
//должно быть 1.25мкс
#define WS2812B_TIMER_AAR       0x0059
//Передача лог. нуля 0.4мкс
#define WS2812B_0_VAL           26
//Передача лог. единицы 0.85мкс
#define WS2812B_1_VAL           62  
//Сигнал RET или RESET более 50мкс

uint8_t led_array[DATA_LEN];

 
 
 
 
 void TIM4_IRQHandler(void)
 {
   
 }

 
 
 

void DMA1_Channel5_IRQHandler()
{
   

 DMA1->IFCR|=(1<<16);
 DMA1_Channel5->CCR  &=~   (1<<0);
 TIM4_STOP;

    
GPIOB->ODR |= (1<<8); 
}
 
 
 
void tim_4_inint(void)
{
   // OUTPUT CHANEL PB8 PWM
   // Channel 7 from DMA 
    

    
RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
RCC->AHBENR  |= RCC_AHBENR_DMA1EN;
GPIOB->CRH   &=~ 0x0000000F;
GPIOB->CRH   |=  0x0000000B;
GPIOB->ODR &=~ (1<<8);   
TIM4->PSC  =  0;                      // делитель
TIM4->ARR  =  WS2812B_TIMER_AAR;      // значение перезагрузки 1.25 us
TIM4->CCR3 =  0;                      // коэф. заполнения
TIM4->DIER  |= (1<<11)|(1<<0)|(1<<3) ;               // Прерывание DMA
TIM4->CCMR2 |= (1<<6)|(1<<5) ;        // Режим ШИМ
TIM4->CCMR2 |= (1<<3);                // Предзагрузка(буферизация) CCR4
TIM4->CCER  |= (1<<8);                // включение порта
TIM4->CR1   |= (1<<7);                // Буферизация ARR
    
 //*****************************************************************************  
 
     

 // NVIC_EnableIRQ(TIM4_IRQn);
NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    
 
 
}
    uint32_t tmp ;
    uint32_t inc  ;
    uint32_t tmp_clr  ;

void fill_LED_array(uint32_t color)
{
  
      
      inc = 0;
      
    
    
 for(uint16_t i = 0; i<VALUE_LED; i++)
     {  
 tmp = DATA_LEN/VALUE_LED;
         tmp_clr =color;
         while(tmp >0 )
         {
if(tmp_clr & 0x800000)
{
led_array[inc] = WS2812B_1_VAL;
}
else
{
led_array[inc] = WS2812B_0_VAL;

}
tmp_clr<<=1;
tmp--;
inc++;
       }
    
    }
 
while(tmp <=2)
 {
 led_array[inc] = 90;     
 inc++;   
 tmp++;     
}        
 
}
void led_send(uint32_t cl)
{
    
 
DMA1_Channel5->CCR  &=~   (1<<0);
DMA1_Channel5->CCR  &=~   0x7FFF;
DMA1_Channel5->CPAR  =   (uint32_t)(&TIM4->CCR3 );                     //заносим адрес регистра куда пишем
DMA1_Channel5->CMAR  =   (uint32_t)(led_array);                        //откуда берем
DMA1_Channel5->CNDTR =    sizeof(led_array);                           //Устанавливаем количество данных
DMA1_Channel5->CCR  |= (1<<13)| 
                       (1<<12)| 
                       (1<<4) |                                         // Из памяти
                       (1<<8) |                                         // Переферия 16 бит
                       (1<<1) |                                         // прерывание по окончанию   
                       (1<<7) ; 
    
    TIM4->CCR3 =  0;
    TIM4_START;
    
    fill_LED_array(cl);
    DMA1_Channel5->CCR |= (1<<0);

}





