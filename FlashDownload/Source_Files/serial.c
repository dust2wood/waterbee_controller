
#include "stm32f10x_usart.h"
#include "serial.h"
#include "Main.h"
#include "misc.h"


SERIAL_CALLBACK SerialCallbackTable[SERIAL_PORT_MAX] = {(void *)0, };
const uint8_t *HexaString = "0123456789ABCDEF";


void Serial_Open(SERIAL_PORT Port, uint32_t Baudrate)
{
	uint32_t TempReg, APBClock, USARTDiv;
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_TypeDef *USART_Port = USART1;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	NVIC_InitTypeDef NVIC_InitStructure;
	switch(Port)
	{
		case SERIAL_PORT1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);		
		
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;						
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;						
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
		

		
			USART_Port = USART1;

			break;
			
		case SERIAL_PORT2 :			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);			

	
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;						
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStruct);	
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;						
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
			USART_Port = USART2;
			break;
			
		case SERIAL_PORT3 :			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		 
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;						
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOB, &GPIO_InitStruct);	
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;							
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOB, &GPIO_InitStruct);			
		
			USART_Port = USART3;
			break;
			
		case SERIAL_PORT4 :			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);		 
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;						
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOC, &GPIO_InitStruct);	
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;						
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOC, &GPIO_InitStruct);	
			USART_Port = UART4;
			break;
			
		default :
			break;
	}

	TempReg = USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE;
	USART_Port->CR1 = TempReg;

	USART_Port->CR3 |= USART_CR3_DMAR;		

	TempReg = 0;
	USART_Port->CR2 = TempReg;	

	RCC_GetClocksFreq(&RCC_ClocksStatus);	
	if (Port == SERIAL_PORT1)		
	{
	  APBClock = RCC_ClocksStatus.PCLK2_Frequency;
	}
	else							
	{
	  APBClock = RCC_ClocksStatus.PCLK1_Frequency;
	}

	USARTDiv = APBClock * 2 / Baudrate;		
	
	if(USARTDiv	& 0x00000001)	
	{
		USARTDiv >>= 1;
		USARTDiv += 1;
	}
	else		 
	{
		USARTDiv >>= 1;		
	}	
	USART_Port->BRR = USARTDiv;


	
	switch(Port)			 
	{
		case SERIAL_PORT1 :
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			break;
		case SERIAL_PORT2 :
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
			break;
		case SERIAL_PORT3 :
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
			break;
		case SERIAL_PORT4 :
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			break;
		default :
			break;
	}
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



}


void Serial_Close(SERIAL_PORT Port)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	switch(Port)			
	{
		case SERIAL_PORT1 :
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			break;
		case SERIAL_PORT2 :
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			break;
		case SERIAL_PORT3 :
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			break;
		case SERIAL_PORT4 :
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			break;			
		default :
			break;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	
	SerialCallbackTable[Port] = (void *)0;
}

