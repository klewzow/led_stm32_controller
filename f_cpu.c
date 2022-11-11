#include "f_cpu.h"
void init_CPU(void)
{
 RCC->CR |= (1<<0);                       // включить внутр.тактовый генератор
    while(!(RCC->CR &(RCC_CR_HSIRDY)));      // Ожидание подтверждения включения
    
    RCC->CFGR &=~ 0x00000003;                // Переключение на внутреннее тактирование 
    while((RCC->CFGR &(RCC_CFGR_SWS))!=0);   // Ожидание подтверждения включения
    
    RCC->CFGR &=~ 0x0000FFF0;                // Сброс делителей шин APB1 APB2 AHB
    
    RCC->CR &=~ (1<<24);                    // Отключение PLL
    while((RCC->CR &(RCC_CR_PLLRDY))!=0);  
    
    RCC->CFGR &=~ 0x003C0000;                // Сброс делителя PLL
    RCC->CFGR &=~ 0x00010000;                // Сброс тактирования PLL
    RCC->CFGR &=~ 0x00020000;                // Сброс XTPRE  делителя  
    RCC->CFGR &=~ 0x07000000;                // Сброс МСО
    RCC->CR   &=~ (1<<16);                   // Отключение HSE
    while((RCC->CR &(RCC_CR_HSERDY))!=0);
    
    RCC->CSR |= RCC_CSR_RMVF;                // сброс Флагов
    RCC->APB1ENR &=~ 0x3AFED9FF;             // Сброс тактинования всего на шине APB1
    RCC->APB2ENR &=~ 0x0038FFFB;             // Сброс тактинования всего на шине APB2
    RCC->AHBENR = 0;                         // Сброс тактинования всего на шине AHB
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN  ;    // Вкл тактирования GPIO A
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;      // Вкл тактирования GPIO D
  
    GPIOA->CRH &=~ 0xFFFFFFFF;               // Сброс портов
    GPIOA->CRL &=~ 0xFFFFFFFF;  
    GPIOD->CRH &=~ 0xFFFFFFFF;                     
    GPIOD->CRL &=~ 0xFFFFFFFF;
    
    GPIOA->BRR = (uint32_t)0xFFFF;
    GPIOD->BRR = (uint32_t)0xFFFF;  
    
    RCC->APB2ENR |= 0x00000001;    // Тактирование AFIO Для РАботы альтернативный ф-ций
    AFIO->MAPR &=~0;               // Сброс всех ремапов
    AFIO->MAPR |= (1<<25);         // Откл JTAG Вкл. SWD отладку 
    
    
    RCC->CR |= (1<<16);                                    // Вкл работу HSE
    while(!(RCC->CR & (RCC_CR_HSERDY))){}; 
    
    FLASH->ACR |= (1<<4);                                  // Вкл буфер предварительной выборки
    FLASH->ACR &=~0x00000007;                              // сбросим делитель   Flash памяти 
    FLASH->ACR |= (1<<1);                                  // Установим делитель Flash памяти 
        
        
    RCC->CFGR  |=  (1<<16);                                // Тактирование PLL от внешнего кварца
    RCC->CFGR  &=~ (1<<17);                                // Откл делитель PLL
        
    RCC->CFGR &=~  0x003C0000;                             // Сброс множитель PLL
    RCC->CFGR |=   0x001C0000;                             // Множитель PLL
    RCC->CFGR |=   (1<<10);                                // Делитель APB1 max 36 mHz 
        
      
        
    RCC->CR |=(1<<24);                                     // Вкл работу PLL 
    while(!(RCC->CR & (RCC_CR_PLLRDY))){};   
        
    RCC->CFGR |=(1<<1);                                    // переход системы на тактироапние от PLL
    while((RCC->CFGR &(RCC_CFGR_SWS))!=0x00000008){};      // Ожидание установки флага SWS
             
 
    RCC->CR &=~ (1<<0);                                    // Выкл. Внутренний источник тактирования
    while((RCC->CR & (RCC_CR_HSIRDY))){};





}


