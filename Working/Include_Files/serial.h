#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "stm32f10x_usart.h"

typedef enum{
	SERIAL_PORT1,
	SERIAL_PORT2,
	SERIAL_PORT3,
	SERIAL_PORT4,
	SERIAL_PORT_MAX	
}SERIAL_PORT;

typedef void (*SERIAL_CALLBACK)(uint8_t Data);
extern SERIAL_CALLBACK SerialCallbackTable[SERIAL_PORT_MAX];

void Serial_Open(SERIAL_PORT Port, uint32_t Baudrate);
void Serial_Close(SERIAL_PORT Port);
void Serial_WriteByte(SERIAL_PORT Port, uint8_t Data);
void Serial_WriteHexa(SERIAL_PORT Port, uint32_t Data);
void Serial_WriteString(SERIAL_PORT Port, uint8_t* String);


#endif





