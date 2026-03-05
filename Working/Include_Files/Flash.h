#ifndef FLASH_H
#define FLASH_H

#include <stm32f10x.h>
#include <stm32f10x_flash.h>


void Flash_Handler(void);
void Flash_Write(u32 Addr, vu32* Data, uint32_t Size);


/*
#define FLASH_INITSTATE				0
#define FLASH_INITREAD					1

#define FLASH_CLEAR1					2
#define FLASH_CLEAR2					3//Delay
#define FLASH_CLEAR3					4//End? No->CLEAR1


#define FLASH_WRITEDELAY1			5
#define FLASH_WRITEDELAY2			6
#define FLASH_IDLE							7*/
/*
#define FLASH_LOG_ADDR				0x0100
#define FLASH_LOG_SIZE				0X0010

#define FLASH_MAX_LOG 				50*/

#define SAVEADDR_BASE				0x08070000
#define FLASH_SETUPVAL				0
//#define FLASH_LOG_BASE				0x800	
#define FLASH_PAGE_SIZE 				((uint16_t)0x800)

/*
#define EEPROM_INITSTATE			0
#define EEPROM_INITREAD				1

#define EEPROM_CLEAR1				2
#define EEPROM_CLEAR2				3//Delay
#define EEPROM_CLEAR3				4//End? No->CLEAR1


#define EEPROM_WRITEDELAY1			5
#define EEPROM_WRITEDELAY2			6
#define EEPROM_IDLE					7

#define EEPROM_LOG_ADDR				0x0100
#define EEPROM_LOG_SIZE				0X0010

#define EEPROM_MAX_LOG 				100
*/




#endif



