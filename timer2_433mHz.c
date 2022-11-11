#include "timer2_433mHz.h"
 u16 v1,v2;
void TIM2_IRQHandler(void)
{
TIM2->CNT = 0;
TIM2->SR &=~(1<<0);
v1 = TIM2->CCR1;
v2 = TIM2->CCR2;
     
}    









void init_timer_2(void)
{
    
    // A1 CH2
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
GPIOA->CRL &=~ 0x000000FF;
GPIOA->CRL |=  0x00000043;
    
    
GPIOA->ODR &=~ (1<<0);
    
    // 10000  ns = 10 ms  длительность начала посылки
    // 150000 ns = 150 ms длительность конца  посылки
 TIM2->CR1  &=~ (1<<0);   
 TIM2->CR1 |= (1<<4);           // Счет в верх
 TIM2->CR1 |= TIM_CR1_ARPE;     // Буферизация   
 TIM2->PSC = 719;           // 2000 тиков за 1 сек 
 TIM2->ARR = 9999;             // 300 тиков я думаю 150 мс
 TIM2->RCR = 0x0000;            // Повторы
 TIM2->EGR |= (1<<0);           // Обновление регистров    
 TIM2->SR  &=~(1<<0);           // Сброс Флага   
 
//-------------------------------------------------------------

 TIM2->SMCR =0; // SMS 000 частота Fcpu
 
 
 
 TIM2->CCMR1 |= (1<<8);  // Канал 2 вход подключен к TimInput 2
 TIM2->CCMR1 |= (1<<1);  // Канал 1 вход подключен к TimInput 2
 
 TIM2->CCER  &=~ (1<<5)  ;      //   2 канал  спад  
 TIM2->CCER  |=  (1<<1)   ;     //   канал 1  фронт  
 TIM2->CCER  |=  (1<<4) |(1<<0); //   вкл каналы    


     








 TIM2->DIER  |= (1<<2);          // прерывания после спада
 NVIC_EnableIRQ(TIM2_IRQn);      // по идее обработчик события переполнения таймера 
 TIM2->CR1   |= (1<<0);    
}


 



void init_timer_3(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    //Вывод таймера PB_0   TIM3 CH3
    GPIOB->CRL &=~ 0x0000000F;           // AF PUSH_PULL 
    GPIOB->CRL |=  0x0000000B;           // AF PUSH_PULL    
    TIM3->CCER    = 0;                   // Сброс разрешений CCER
    TIM3->ARR     = 100;                 // максимальное значение, до которого таймер ведет счет 
    TIM3->PSC     = 360-1;             // предделитель     
    TIM3->RCR     = 0;                   // кол-во повторов        
    TIM3->CCR3 = 10-1;                   // Скважность
    TIM3->CCMR2 &=~0x00000003;           // Канал СН3 На выход 
    TIM3->CCER  |= (1<<8);               // Отключение/включение выхода (входа) канала.
    TIM3->CCMR2 |= 0x00000070;           // Прямой/обратный ШИМ
    TIM3->CR1   |= (1<<0);               // Вкл. таймер
}















