#ifndef __flash_h
#define __flash_h
 

#include "main.h"
 
#define SPI1_16_BIT  (SPI1->CR1  |= (1<<11))
#define SPI1_8_BIT   (SPI1->CR1  &=~(1<<11))

#define F25_READ            0x03        //  Чтение байта 3-байта адреса от  1 до бесконечности 40 MHz
#define F25_FAST_READ       0x0B        //  Чтение байта FAST SPEED 3-байта адреса от  1 до бесконечности 104 MHz
#define F25_DOR             0x3B        //  Dual Output Read  
#define F25_QOR             0x6B        //  Quad Output Read  
#define F25_DIOR            0xBB        //  Dual I/O High Performance Read 
#define F25_QIOR            0xEB        //  Quad I/O High Performance Read  
#define F25_RDID            0x9F        //  Read Identification  
#define F25_READ_ID         0x90        //  Read Manufacturer and Device Identification  
//Write Control
#define F25_WREN            0x06        //  Write Enable  
#define F25_WRDI            0x04        //  Write Disable  
//Erase
#define F25_P4E             0x20        //  4-KB Parameter Sector Erase  
#define F25_P8E             0x40        //  8-KB (two 4KB) Parameter Sector Erase 
//Erase 3 0 0 0
#define F25_SE              0xD8        //  64-KB Sector Erase  
#define F25_BE              0x60        //  or (C7h)   Bulk Erase  
//Program
#define F25_PP              0x02        // Page Programming   1 to 256
#define F25_QPP             0x32        // Quad Page Programming  1 to 256
#define F25_RDSR            0x05        // Read Status Register 0 0 0 1 to 
#define F25_WRR             0x01        // Write (Status & Configuration) Register 0 0 0 1 to 2
#define F25_RCR             0x35        // Read Configuration Register (CFG) 0 0 0 1 to 
#define F25_CLSR            0x30        // Сбросить флаг стирания и сбоя программы(SR5 и SR6) и восстановить нормальный операция)
 
//Power Saving
#define F25_DP              0xB9        // Deep Power-Down 0 0 0 0
#define F25_RES             0xAB        // Release from Deep Power-Down Mode 0 0 0 0
 
//OTP
#define F25_OTPP            0x42        // Program one byte of data in OTP  memory space 3 0 0 1
#define F25_OTPR            0x4B        // Read data in the OTP memory space 3 0 1 1 to 

#define SPI1_DMA_ADRESS   0x4001300C
#define cs_flash_25w     4
#define cs_port      GPIOA
#define cs_25w_HI       (cs_port->BSRR |= (1<<cs_flash_25w))
#define cs_25w_LO       (cs_port->BRR  |= (1<<cs_flash_25w))

#define  SIZEI_IMG  (240*320)
void flash_init(void);
void flash_25w_read_DMA(  uint32_t val,  uint32_t  *adress_mem_TX ,  uint32_t adress_PF );
void flash_25w_transmit_8(uint8_t data);
uint16_t flash_25w_resive_8(uint8_t data);

#endif 




