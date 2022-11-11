#include "tft.h"
uint16_t X_SIZE = 320;
uint16_t Y_SIZE = 240;

 
void pushData(uint16_t data)
{
 while(!(SPI1->SR & SPI_SR_TXE));
 SPI1->DR = data;
 while((SPI1->SR & SPI_SR_BSY)) continue;
}

//-------------------------------------------------------------------
void TFT9341_SendCommand(uint16_t cmd)
{
  DC_TFT_COMMAND;
  pushData(cmd);
  delayMicro(1);
   
}
//-------------------------------------------------------------------
void TFT9341_SendData(uint16_t dt)
{
	DC_TFT_DATA;
    pushData(dt);
    delayMicro(2);  
}
void TFT9341_WriteData(uint16_t dt)
{
	DC_TFT_DATA;
    pushData(dt>>8);
    pushData(dt);
}
//-------------------------------------------------------------------
void TFT9341_reset(void)
{
	DC_TFT_RES_LO;
	delayMicro(1000);
	DC_TFT_RES_HI;
    delayMicro(1000);
}
//-------------------------------------------------------------------

void TFT24_SET_Column_SET_Page(uint16_t column_X1, uint16_t page_Y1, uint16_t column_X2, uint16_t page_Y2)
{
 
    
  TFT9341_SendCommand(0x2A);           // Column Addres Set
 
  TFT9341_SendData(column_X1>>8);      // Старшший байт 
  TFT9341_SendData(column_X1 );        // Млфдший байт 
  TFT9341_SendData(column_X2>>8);      // Старшший байт 
  TFT9341_SendData(column_X2  );       // Млфдший байт 
    
   
  TFT9341_SendCommand(0x2B);           // Column Page Set
   
  TFT9341_SendData(page_Y1>>8);        // Старшший байт 
  TFT9341_SendData(page_Y1 & 0xFF);    // Млфдший байт 
  TFT9341_SendData(page_Y2>>8);        // Старшший байт 
  TFT9341_SendData(page_Y2 & 0xFF);    // Млфдший байт 
    
 delayMicro(50);
          // write to RAM
 TFT9341_SendCommand(0x2C); // RAMWR
}

void TFT9341_FillScreen(uint16_t color)
{
CS_TFT_LO;
    delayMicro(1);
        uint32_t iMAX = (240*320) ;    
        TFT24_SET_Column_SET_Page(0,0,(X_SIZE-1),(Y_SIZE-1));
        while(iMAX > 0)
        {
        TFT9341_WriteData(color);
        iMAX--;
          
}
   delayMicro(1);
 CS_TFT_HI;
}


void TFT24_SetRotation(uint8_t r)
{
  TFT9341_SendCommand(0x36);
  switch(r)
  {
    case 0:
      TFT9341_SendData(0x48);
      X_SIZE = 240;
      Y_SIZE = 320;
      break;
    case 1:
      TFT9341_SendData(0x28);
      X_SIZE = 320;
      Y_SIZE = 240;
      break;
    case 2:
      TFT9341_SendData(0x88);
      X_SIZE = 240;
      Y_SIZE = 320;
      break;
    case 3:
      TFT9341_SendData(0xE8);
      X_SIZE = 320;
      Y_SIZE = 240;
      break;
  }
}
void tft_INIT_REGISTER(uint8_t rotation)
{
 
TFT9341_reset();
 
CS_TFT_LO;

//****************************************************************
TFT9341_SendCommand(0x01);//Software Reset
 delayMicro(500);   
TFT9341_SendCommand(0xCB);//Power Control A
TFT9341_SendData(0x39);
TFT9341_SendData(0x2C);
TFT9341_SendData(0x00);
TFT9341_SendData(0x34);
TFT9341_SendData(0x02);
  
    
TFT9341_SendCommand(0xCF);//Power Control B
TFT9341_SendData(0x00);
TFT9341_SendData(0xC1);
TFT9341_SendData(0x30);
  
    
TFT9341_SendCommand(0xE8);//Driver timing control A
TFT9341_SendData(0x85);
TFT9341_SendData(0x10);
TFT9341_SendData(0x78);
 
 
TFT9341_SendCommand(0xEA);//Driver timing control B
TFT9341_SendData(0x00);
TFT9341_SendData(0x00);
 
 
TFT9341_SendCommand(0xED);//Power on Sequence control
TFT9341_SendData(0x64);
TFT9341_SendData(0x03);
TFT9341_SendData(0x12);
TFT9341_SendData(0x81);
 

TFT9341_SendCommand(0xF7);//Pump ratio control
TFT9341_SendData(0x20);
 

TFT9341_SendCommand(0xC0);//Power Control 1
TFT9341_SendData(0x23);
 

TFT9341_SendCommand(0xC1);//Power Control 2
TFT9341_SendData(0x10);
 

TFT9341_SendCommand(0xC5);//VCOM Control 1
TFT9341_SendData(0x3E);
TFT9341_SendData(0x28);
 

TFT9341_SendCommand(0xC7);//VCOM Control 2
TFT9341_SendData(0x86);
 

TFT24_SetRotation(rotation);
 TFT24_SET_Column_SET_Page(0,0,(X_SIZE-1),(Y_SIZE-1)); 

TFT9341_SendCommand(0x3A);    //Pixel Format Set
TFT9341_SendData(0x55);       //16bit
 

TFT9341_SendCommand(0xB1);
TFT9341_SendData(0x00);
TFT9341_SendData(0x18);       // Частота кадров 79 Гц
 




TFT9341_SendCommand(0xB6);    //Display Function Control
TFT9341_SendData(0x08);
TFT9341_SendData(0x82);
TFT9341_SendData(0x27);       //320 строк
 

TFT9341_SendCommand(0xF2);    //Enable 3G (пока не знаю что это за режим)
TFT9341_SendData(0x00);       //не включаем
 

TFT9341_SendCommand(0x26);    //Gamma set
TFT9341_SendData(0x01);       //Gamma Curve (G2.2) (Кривая цветовой гаммы)
 
 
TFT9341_SendCommand(0xE0);    //Positive Gamma Correction
    TFT9341_SendData(0x0F);
    TFT9341_SendData(0x31);
    TFT9341_SendData(0x2B);
    TFT9341_SendData(0x0C);
    TFT9341_SendData(0x0E);
    TFT9341_SendData(0x08);
    TFT9341_SendData(0x4E);
    TFT9341_SendData(0xF1);
    TFT9341_SendData(0x37);
    TFT9341_SendData(0x07);
    TFT9341_SendData(0x10);
    TFT9341_SendData(0x03);
    TFT9341_SendData(0x0E);
    TFT9341_SendData(0x09);
    TFT9341_SendData(0x00);
 
 

TFT9341_SendCommand(0xE1);//Negative Gamma Correction
    TFT9341_SendData(0x00);
    TFT9341_SendData(0x0E);
    TFT9341_SendData(0x14);
    TFT9341_SendData(0x03);
    TFT9341_SendData(0x11);
    TFT9341_SendData(0x07);
    TFT9341_SendData(0x31);
    TFT9341_SendData(0xC1);
    TFT9341_SendData(0x48);
    TFT9341_SendData(0x08);
    TFT9341_SendData(0x0F);
    TFT9341_SendData(0x0C);
    TFT9341_SendData(0x31);
    TFT9341_SendData(0x36);
    TFT9341_SendData(0x0F);
  

TFT9341_SendCommand(0x11);//Выйдем из спящего режим
delayMicro(1500);
TFT9341_SendCommand(0x29);//Включение дисплея
//****************************************************************

        // write to RAM
        

CS_TFT_HI;
delayMicro(1000);   

 
}
  
void init_tft(void)
{
RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
GPIOB->CRH &=~ 0x000F0000; // 12  
GPIOB->CRH |=  0x00030000; // 12  
GPIOA->CRH &=~ 0x000000FF; // 8 9  
GPIOA->CRH |=  0x00000033; // 8 9
GPIOB->ODR |= (1<<12);     // CS HI
tft_INIT_REGISTER(1);


}








