#include "flash25.h"
uint32_t circ_dma_spi_1 = 0;
 
 
void DMA1_Channel3_IRQHandler()
{
   
}
void DMA1_Channel2_IRQHandler()
{
            DMA1->IFCR |= (1<<4); 
    
    
 
            if(circ_dma_spi_1 == 102400)
            {
            DMA1_Channel3->CCR   &=~  (1<<0);
            DMA1_Channel2->CCR   &=~  (1<<0);
            cs_25w_HI;    
            CS_TFT_HI;
            DC_TFT_COMMAND;
            circ_dma_spi_1 = 0;   
     SPI1->CR2 &=~ SPI_CR2_TXDMAEN;        //    Разрешить отправлять данные для передачи через DMA
     SPI1->CR2 &=~ SPI_CR2_RXDMAEN;        //    Разрешить отправлять данные для передачи через DMA
    
     
            }else{
            circ_dma_spi_1++; 
                
                }
 
               
                
}
void flash_25w_transmit_8(uint8_t data)
{
 
 
    while(!(SPI1->SR & SPI_SR_TXE)){;;};
     SPI1->DR = data;
   while(!(SPI1->SR & SPI_SR_TXE)){;;};
  
}
uint16_t flash_25w_resive_8(uint8_t data)
{
  
   
 while(!(SPI1->SR & (1<<1)) ){;;}  ;
     SPI1->DR = data; //запускаем обмен
 
    while(!(SPI1->SR & (1<<0)) ){;;}  ;
   while( (SPI1->SR & SPI_SR_BSY) ){;;}  ;
  //возвращаем значение буфера приемника
  return  SPI1->DR;
 
    
}
void flash_init_SPI_1(void)
{
      
     RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
     RCC->APB2ENR |=  RCC_APB2ENR_SPI1EN;
     RCC->AHBENR |= RCC_AHBENR_DMA1EN;
     SPI1->CR1 &=~(1<<6);
     GPIOA->CRL   &=~ 0xFFF00000;
     GPIOA->CRL   |=  0xB4B00000;
     SPI1->CR1 =0;
     SPI1->CR1 |= 0b0000001100001100;  
     SPI1->CR2 = 0;
     SPI1->CR2 |=SPI_CR2_TXDMAEN;        //    Разрешить отправлять данные для передачи через DMA
     SPI1->CR2 |=SPI_CR2_RXDMAEN;        //    Разрешить отправлять данные для передачи через DMA
     SPI1->CR1 |=(1<<6);     // EnableSPI

}



void flash_init(void)
{
flash_init_SPI_1(); 
delayMicro(1000);    
cs_port->CRL &=~ (0x0F<<(4*cs_flash_25w));  
cs_port->CRL |= (1<<(4*cs_flash_25w));
cs_25w_LO;  
flash_25w_transmit_8(F25_RES) ;

cs_25w_HI;
delayMicro(10000);
    

}
void flash_25w_read_DMA(  uint32_t val,  uint32_t  *adress_mem_TX ,  uint32_t adress_PF )
{
 
 
 CS_TFT_LO;
    
TFT24_SET_Column_SET_Page(0,0,( 320-1),( 240-1));   
DC_TFT_DATA;
 
 
 
    cs_25w_LO;     
    flash_25w_transmit_8(F25_READ);
    flash_25w_transmit_8(0x00);  //три
    flash_25w_transmit_8(0x00);  //байта
    flash_25w_transmit_8(0x00);  //адресса первой ячейки
 
    
  //************************TX**************************************************** 
    DMA1_Channel3->CCR   &=~  (1<<0);
    DMA1_Channel3->CCR   &=~  0x7FFF;
    DMA1_Channel3->CPAR  =   (uint32_t)(adress_PF);             //заносим адрес регистра DR в CPAR
    DMA1_Channel3->CMAR  =   (uint32_t) adress_mem_TX;          //заносим адрес данных в регистр CMAR  
    DMA1_Channel3->CNDTR =    val;                              //количество передаваемых данных
    DMA1_Channel3->CCR   |=     (1<<12)|
                                (1<<4 )|
                                (1<<5 );
 
 
    
//************************RX****************************************************       
      
        

   // CS_TFT_LO;
    DMA1_Channel2->CCR   &=~  (1<<0);
    DMA1_Channel2->CCR   &=~  0x7FFF;
    DMA1_Channel2->CPAR  =   (uint32_t)(adress_PF);                   //заносим адрес регистра DR в CPAR
    DMA1_Channel2->CMAR  =   (uint32_t)adress_mem_TX ;                       //заносим адрес данных в регистр CMAR  
    DMA1_Channel2->CNDTR =    val;                                    //количество передаваемых данных
    DMA1_Channel2->CCR   |=      
                                (1<<5 )|
                                (1<<1 ) ;
                                
    NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    //NVIC_EnableIRQ(DMA1_Channel3_IRQn);  
    DMA1_Channel2->CCR   |=  (1<<0);   
    DMA1_Channel3->CCR   |=  (1<<0);
 
    
}



