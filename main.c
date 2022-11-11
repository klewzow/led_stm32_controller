#include "main.h"
#define F_CPU 72000000UL
 
 uint32_t ad_TX[1] = {0x0000};
int main(void)
	{

                init_CPU();
                delayMicro(10000);
                flash_init();
                delayMicro(10000);
                init_tft();       
                delayMicro(10000);
                tim_4_inint();
                __enable_irq ();          // Разрешить прерывания глобально
                delayMicro(10000);        
                // TFT9341_FillScreen(BLACK);

                flash_25w_read_DMA( 1 ,  (uint32_t *)ad_TX,  (uint32_t)SPI1_DMA_ADRESS);   
                delayMicro(10000);        
                init_touch();       
                delayMicro(1000);

                led_send(0x000000);   
                run_button( (unsigned short *)on_green);


        
        while(1)
{
 

} //while(1)


    

 






    }// MAIN END
  
      

 void delayMicro(uint32_t micro)
 {
 micro *=F_CPU/1000000 ;
     while(micro--);
 }

 
































































