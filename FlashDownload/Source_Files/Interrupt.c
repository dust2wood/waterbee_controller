#include "Interrupt.h"


uint32_t flag10ms = 0;

uint16_t rx1Buffer[50] = {0,};
uint16_t rx2Buffer[256] = {0,};
uint16_t rx3Buffer[256] = {0,};
uint16_t tx1Buffer[15] = {0,};
uint16_t tx2Buffer[256] = {0,};
uint16_t tx3Buffer[256] = {0,};
uint32_t tx1Size = 0, tx1Count = 0, rx1Size = 0;
uint32_t tx2Size = 0, tx2Count = 0, rx2Size = 0;
uint32_t tx3Size = 0, tx3Count = 0, rx3Size = 0;

uint32_t flag_wash1_time_counter = 0;
uint32_t flag_wash2_time_counter = 0;
uint32_t flag_wash3_time_counter = 0;

uint32_t flag_wash1_relay_time_counter = 0;
uint32_t flag_wash2_relay_time_counter = 0;
uint32_t flag_wash3_relay_time_counter = 0;

uint32_t zero_Alarm_Time_couter = 0;

uint32_t MicroSdCard_Time_Coutner = 0;
uint32_t MicrosdCard_Run_flag = 0;

void TIM2_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	
	if( wash_run_flag == 1) 	{  flag_wash1_time_counter++; }
  else { flag_wash1_time_counter = 0;   }
	
	if(Relay1_run_on_flag == 1) { flag_wash1_relay_time_counter++;  }
	
	if(Alarm_OP_flag == 0 && configData.alarmConfig.zeroAlarm != 0) { zero_Alarm_Time_couter++; }
	else { zero_Alarm_Time_couter = 0; }
	
 //-------------------------------------------------------------------------------------//
  
  //-------------------------------------------------------------------------------------//
  if( wash_run_flag3 == 1) 	{  flag_wash3_time_counter++; }
  else { flag_wash3_time_counter = 0;   }
	
	if(Relay3_run_on_flag == 1) { flag_wash3_relay_time_counter++;  }
	//---------------------------------------------------------------------------------------//
	
	if( MicrosdCard_Run_flag == 0 && sd_dec_flag == 1)
	{
     MicroSdCard_Time_Coutner++;
		 if(MicroSdCard_Time_Coutner == 1350)
	    {
		    MicroSdCard_Time_Coutner = 0;
	      MicrosdCard_Run_flag = 1;		 
      }
  }	
	
}

uint16_t Counter10msec = 0;
uint16_t Comm_time_flag = 0;

void TIM3_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
	flag10ms = 0xFFFFFFFF;	
	
	if(Comm_time_flag == 1)
	{
      Counter10msec++;
  }
}

void TIM4_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
}
void TIM5_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update); 
}

  

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET) 
	{
		USART1->SR &= USART1->SR&(~0x40);
		
		if(tx1Count < tx1Size)
			USART1->DR = tx1Buffer[tx1Count++];
		else
		{
			USART1->CR1 &= ~0x40;		
		}
	}
	else
	{
		USART1->SR = (USART1->SR)&(~((USART1->SR)&0X20));
		rx1Buffer[rx1Size] = USART1->DR;
		++rx1Size;
		rx1HandlerCount  = 0;
		if(rx1Size == 128)
			rx1Size = 0;
	}
}

void USART2_IRQHandler(void)
{
		if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) 
	{
		USART2->SR &= USART2->SR&(~0x40);
		
		if(tx2Count < tx2Size)
			USART2->DR = tx2Buffer[tx2Count++];
		else
		{
			USART2->CR1 &= ~0x40;		
		}
	}
	else
	{
		USART2->SR = (USART2->SR)&(~((USART2->SR)&0X20));
		rx2Buffer[rx2Size] = USART2->DR;
		++rx2Size;
		rx2HandlerCount  = 0;
		if(rx2Size == 128)
			rx2Size = 0;
	}
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_TC) != RESET) 
	{
		USART3->SR &= USART3->SR&(~0x40);
		
		if(tx3Count < tx3Size)
			USART3->DR = tx3Buffer[tx3Count++];
		else
		{
			USART3->CR1 &= ~0x40;		
			com485State = 3;
		}
	}
	else
	{
		USART3->SR = (USART3->SR)&(~((USART3->SR)&0X20));
		rx3Buffer[rx3Size] = USART3->DR;
		++rx3Size;
		rx3HandlerCount  = 0;
		if(rx3Size == 128)
			rx3Size = 0;
	}
}









