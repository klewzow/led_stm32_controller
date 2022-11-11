#include "touch.h"
 
volatile u32 res_dataY[2] = {0};
volatile u32 res_dataX[2] = {0};
volatile u32 res_dataYtmp[20] = {0};
volatile u32 res_dataXtmp[20] = {0};
uint8_t on_off_flaf = 0;
    u16 tmp1,tmp2;
void EXTI15_10_IRQHandler(void)
{
 EXTI->PR |=  (1<<IRQ_T);
   
    
 while(!(GPIOA->IDR & (1<<IRQ_T)) )
 {
    res_dataX[0] = 0;
    res_dataY[0] = 0;  
 
     
 
 
 
                        for(u16 i = 0; i < 15; i++)
                        {       
                        res_dataXtmp[i] = tmp1  = read_touch_DataX();  
                        if(tmp1 > 319 ) {tmp1 = 319;}

                        res_dataX[0] +=tmp1; 


                       res_dataYtmp[i] = tmp2  = read_touch_DataY();
                        if(tmp2 > 239) {tmp1 = 239;}; 
                        res_dataY[0] +=tmp2;      
                        }
                        res_dataX[1] = res_dataX[0]/15;
                        res_dataY[1] = res_dataY[0]/15;
     
 
     
    if(res_dataX[1] > 319 || res_dataY[1] > 239 ){break;}
 
   touch_decode(res_dataX[1],res_dataY[1]); 
  delayMicro(10000);     
 
 }
 //EXTI->IMR   |=  (1<<IRQ_T);  
}



void init_touch(void)
{
    
RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;
RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;
RCC->APB1ENR |=  RCC_APB1ENR_SPI2EN;
    
GPIOB->CRH &=~ 0xFFF00000; //   13 14 15 
GPIOB->CRH |=  0xB4B00000; //   13 14 15 

GPIOA->CRH &=~ 0x0000FF00;
GPIOA->CRH |=  0x00003400;    
GPIOA->ODR |=  (1<<CS_TOUCH);        // PA 10  VALUE HI
    
SPI2->CR1   = 0; 
SPI2->CR1  |=  0b0000001100111100;
SPI2->CR2   = 0;
SPI2->CR1  |= (1<<6);                 // Enable SPI2    
delayMicro(50);
  
  
            CS_SENSOR_LO;  
            delayMicro(1);
            while(!(SPI2->SR & SPI_SR_TXE)){;;};
            SPI2->DR = 0x80; 
            while(!(SPI2->SR & SPI_SR_TXE)){;;};

            SPI2->DR = 0x00; 
            while(!(SPI2->SR & SPI_SR_TXE)){;;};

            SPI2->DR = 0x00; 
            while((SPI2->SR & SPI_SR_BSY)){;;};
            delayMicro(5);
            CS_SENSOR_HI;

            EXTI->IMR   |=  (1<<IRQ_T);            // прерывание 10 порт  
            EXTI->FTSR  |=  (1<<IRQ_T);            // прерывание по спаду
            EXTI->PR    |=  (1<<IRQ_T);            // Сброс флага события (после каждого прерывания в ручную)
            NVIC_EnableIRQ(EXTI15_10_IRQn);        // Включить прерывания в NVIC     
            AFIO->EXTICR[2] &=~ 0xF000;            // конф. регистр выбрать порт
                                                   //  0000: PA[x] pin
                                                   //  0001: PB[x] pin
                                                   //  0010: PC[x] pin
                                                   //  0011: PD[x] pin
                                                   //  0100: PE[x] pin
                                                   //  0101: PF[x] pin
                                                   //  0110: PG[x] pin   
    


}

uint16_t read_touch_DataX()
{
            u16 dataY = 0;

            //ждём пока опустошится Tx буфер
            while(!(SPI2->SR & SPI_SR_TXE));
            //активируем Chip Select
            CS_SENSOR_LO;   
            //отправляем данные     
            SPI2->DR = 0xD0;  
            while(!(SPI2->SR & SPI_SR_RXNE));
            //считываем полученные данные

            while(!(SPI2->SR & SPI_SR_TXE));
            SPI2->DR = 0x00; 
            //ждём пока опустошится Tx буфер
            while(!(SPI2->SR & SPI_SR_RXNE)); 

            dataY = SPI2->DR;  
            dataY<<=8;
            while(!(SPI2->SR & SPI_SR_TXE));
            SPI2->DR = 0x00; 
            //ждём пока опустошится Tx буфер
            while(!(SPI2->SR & SPI_SR_RXNE));

            dataY |= SPI2->DR;  
            while(!(SPI2->SR & SPI_SR_RXNE)){};    
            dataY >>=3;

 
            dataY =(319 -  dataY/12) ;    





            CS_SENSOR_HI; 

            return dataY;
}
uint16_t read_touch_DataY(void)
{
      
        u16 dataX = 0;
        //ждём пока опустошится Tx буфер
        while(!(SPI2->SR & SPI_SR_TXE));
        //активируем Chip Select
        CS_SENSOR_LO;   
        //отправляем данные     
        SPI2->DR = 0x90;  
        while(!(SPI2->SR & SPI_SR_RXNE));
        //считываем полученные данные

        while(!(SPI2->SR & SPI_SR_TXE));
        SPI2->DR = 0x00; 
        //ждём пока опустошится Tx буфер
        while(!(SPI2->SR & SPI_SR_RXNE)); 

        dataX = SPI2->DR;  
        dataX<<=8;
        while(!(SPI2->SR & SPI_SR_TXE));
        SPI2->DR = 0x00; 
        //ждём пока опустошится Tx буфер
        while(!(SPI2->SR & SPI_SR_RXNE));

        dataX |= SPI2->DR;  
        while(!(SPI2->SR & SPI_SR_RXNE)){};    
        dataX >>=3;


  
      dataX =( 239 - dataX/16)  ;            

        CS_SENSOR_HI; 

        return dataX;
}
 
 
double BAC;
double CA,AB,BC;
float sinC = 0.1236954;
float l_ = 0;
 

 

 
 
//******************************************************************************
typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb ;

typedef struct HsvColor
{
    unsigned int h;
    unsigned int s;
    unsigned int v;
} hsv;
 double rgb_res;
hsv HSV ;
  rgb RGB;
  
 #define MAX_VAL 225 
rgb hsv2rgb(hsv HSV, double dD)
{
    
 if(HSV.h <= 360 && HSV.h >= 300 )
 {
  rgb_res  =  (359  -  dD) * 3.6;   
 RGB.r = MAX_VAL;
 RGB.g = 0;
 RGB.b = rgb_res;
     
 }else  if(HSV.h <= 300 && HSV.h >= 240 )
 {
 rgb_res  =  MAX_VAL -((300  -  dD) * 3.6);
     
 RGB.r = rgb_res; //--
 RGB.g = 0;
 RGB.b = MAX_VAL;
 }else  if(HSV.h <= 240 && HSV.h >= 180 )
 {
  rgb_res  =  (240  -  dD) * 3.6;
     
 RGB.r = 0;  
 RGB.g = rgb_res;
 RGB.b = MAX_VAL;
 }else  if(HSV.h <= 180 && HSV.h >= 120 )
 {
  rgb_res  =    MAX_VAL -((180  -  dD) * 3.6);
     
 RGB.r = 0; //--
 RGB.g = MAX_VAL;
 RGB.b = rgb_res ;
 }else  if(HSV.h <= 120 && HSV.h >= 60 )
 {
  rgb_res  =  (120  -  dD) * 3.6;
     
 RGB.r = rgb_res; //--
 RGB.g = MAX_VAL;
 RGB.b = 0 ;
 }

 else  if(HSV.h <=  60 && HSV.h > 0 )
 {
  rgb_res  =      MAX_VAL -((60  -  dD) * 3.6);
     
 RGB.r = MAX_VAL; //--
 RGB.g = rgb_res;
 RGB.b = 0 ;
 } 
  
    
    
    
    
    
    
    
    
    
    return RGB;
}
 
//******************************************************************************
uint32_t res;
void touch_decode(u16 X, u16 Y)
{
                BC = 0;
                AB = 0; 
                CA =0; 
                sinC =0;
                BAC = 0;
                l_ =   0;  

u8 zone = 0;
     if(X > 140 && Y < 120){zone = 1;}
else if(X > 140 && Y > 120){zone = 2;}
else if(X < 140 && Y > 120){zone = 3;}
else if(X < 140 && Y < 120){zone = 4;}
 
    //Синусом острого угла прямоугольного треугольника называется отношение противолежащего катета к гипотенузе.
switch(zone)
{


                case 1:
                BC =  X - 140;
                AB =  120 - Y; 
                CA = sqrt( (BC*BC)+(AB*AB)); 
                sinC = BC/CA;
                BAC = (asin(sinC)*180/pi);
                l_ =   (pi*120*BAC)/180;  

                break;
                case 2:
                BC =   Y - 120; 
                AB =   X - 140;
                CA = sqrt( (BC*BC)+(AB*AB)); 
                sinC = BC/CA;
                BAC = (asin(sinC)*180/pi)+90 ;

                l_ =   (pi*120*BAC)/180;  
                break;
                case 3:
                AB =   Y - 120;   
                BC =   140 - X; 
                CA = sqrt( (BC*BC)+(AB*AB)); 
                sinC = BC/CA;
                BAC = (asin(sinC)*180/pi)+180 ;

                l_ =   (pi*120*BAC)/180;  

                break;
                case 4:
                AB =  140 - X; 
                BC =  120 - Y;
                CA = sqrt( (BC*BC)+(AB*AB)); 
                sinC = BC/CA;
                BAC = (asin(sinC)*180/pi)+270 ;

                l_ =   (pi*120*BAC)/180;  
                break;
                default :

                break;


}
if(CA < 110 && on_off_flaf == 1 )
{
 
HSV  = (hsv){.h = BAC ,.s =100,.v=100 };
 
hsv2rgb( HSV , BAC);

res= RGB.g;
res<<=8;
res |=  RGB.r;
res<<=8;
res |=  RGB.b;
led_send(res);
}
else if(zone == 3 && CA > 100 && BC > 115 && BC <135 && AB > 75 && AB <95)
{
if(on_off_flaf == 0)
{
        run_button( (unsigned short *)off_red  );
        on_off_flaf = 1;
        led_send(0xF0F0F0);
        delayMicro(25000);
        while(!(GPIOA->IDR & (1<<IRQ_T)) ){;;};
        delayMicro(25000);
}else if(on_off_flaf == 1)
{

        run_button( (unsigned short *) on_green );
        on_off_flaf = 0;
        led_send(0x000000);
}
delayMicro(25000);
while(!(GPIOA->IDR & (1<<IRQ_T)) ){;;};
delayMicro(25000);
}



}
 void run_button( unsigned short * arr)
 {
  CS_TFT_LO;
 
TFT24_SET_Column_SET_Page(10,190,( 10+45),(190+45));   
DC_TFT_DATA;
     
     
     
     
        for(uint16_t i = 0; i < 2116; i++)
        {
        while(!(SPI1->SR & SPI_SR_TXE)){;;};

        SPI1->DR =   arr[i]>>8  ; 
        while(!(SPI1->SR & SPI_SR_TXE)){;;};

        SPI1->DR =  arr[i] & 0xFF ; 
        }
     
     
  CS_TFT_HI;
 
 
 }

 
 

