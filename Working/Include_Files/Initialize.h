#ifndef INITIALIZE_H
#define INITIALIZE_H
#include "Main.h"

void Initialize(void);
void System_TIMConfig(void);
void System_InterruptConfig(void);
void RCC_Configuration(void);
void User_ADC_Configuration(void);
void GPIO_Configuration(void);
void FSMC_NAND_Init(void);
void FSMC_LCD_Init(void);
void TIM8_Configuration(void);
void Spi_configuration (void);
void I2C_configuration (void);

#endif




