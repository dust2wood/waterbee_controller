//Project Option에서 Link Path를 맞추어 주거나 Project에 추가해 줄것.
#include "Watchdog.h"

/******************** (C) COPYRIGHT 2014 OEUN CNI ********************
* File Name        : Watchdog.c
* Author             : Min Chul Joo
* Version           : V1.0
* Date                : 06/11/2014
* Description     : Watchdog Library (IWDG WWDG)
* environment   : STM32F103VE
*/


void Set_IWDG_ResetTime(uint16_t Time)
{	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	if(Time!=0 && Time<21)
		IWDG_SetReload(Time * 156);
	else
		IWDG_SetReload(156); 
	IWDG_ReloadCounter();
}
void IWDG_Start(void)
{
	RCC_LSICmd(ENABLE); 
	IWDG_Enable();
}
void Set_WWDG_ResetTime(uint16_t Time)
{	

}
void WWDG_Start(void)
{

}

