#ifndef WATCHDOG_H
#define WATCHDOG_H
//Project Option에서 Link Path를 맞추어 주거나 Project에 추가해 줄것.

/******************** (C) COPYRIGHT 2014 OEUN CNI ********************
* File Name        : Watchdog.h
* Author             : Min Chul Joo
* Version           : V1.0
* Date                : 06/11/2014
* Description     : Watchdog Library (IWDG WWDG)
* environment   : STM32F103VE
*/
#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"

void Set_IWDG_ResetTime(uint16_t Time);
void IWDG_Start(void);
void Set_WWDG_ResetTime(uint16_t Time);
void WWDG_Start(void);

#endif
