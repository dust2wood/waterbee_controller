#ifndef DAC7512_INTERFACE_H
#define DAC7512_INTERFACE_H

#include "Main.h"

#define SYNC1_LOW							GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define SYNC1_HIGH							GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define SYNC2_LOW							GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define SYNC2_HIGH							GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define SYNC3_LOW							GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define SYNC3_HIGH							GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define DIN1_LOW								GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define DIN1_HIGH								GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define SCLK1_LOW							GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define SCLK1_HIGH							GPIO_SetBits(GPIOA, GPIO_Pin_5)


void InitDAC(void);
void ControlDAC(uint32_t outDAC);

void DelaySPI(volatile uint32_t t);
#endif

