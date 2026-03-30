#include "Initialize.h"
#include <stdio.h>

static int month_from_str(const char *m)
{
	if (m[0] == 'J' && m[1] == 'a' && m[2] == 'n') return 0;
	if (m[0] == 'F' && m[1] == 'e' && m[2] == 'b') return 1;
	if (m[0] == 'M' && m[1] == 'a' && m[2] == 'r') return 2;
	if (m[0] == 'A' && m[1] == 'p' && m[2] == 'r') return 3;
	if (m[0] == 'M' && m[1] == 'a' && m[2] == 'y') return 4;
	if (m[0] == 'J' && m[1] == 'u' && m[2] == 'n') return 5;
	if (m[0] == 'J' && m[1] == 'u' && m[2] == 'l') return 6;
	if (m[0] == 'A' && m[1] == 'u' && m[2] == 'g') return 7;
	if (m[0] == 'S' && m[1] == 'e' && m[2] == 'p') return 8;
	if (m[0] == 'O' && m[1] == 'c' && m[2] == 't') return 9;
	if (m[0] == 'N' && m[1] == 'o' && m[2] == 'v') return 10;
	if (m[0] == 'D' && m[1] == 'e' && m[2] == 'c') return 11;
	return 0;
}

static void set_rtc_to_build_time(void)
{
	struct tm t;
	char mon_str[4] = {0};
	int day = 1;
	int year = 2000;
	int hour = 0;
	int min = 0;
	int sec = 0;

	if (sscanf(__DATE__, "%3s %d %d", mon_str, &day, &year) == 3) {
		t.tm_year = year;
		t.tm_mon = month_from_str(mon_str);
		t.tm_mday = day;
	} else {
		t.tm_year = 2000;
		t.tm_mon = 0;
		t.tm_mday = 1;
	}

	if (sscanf(__TIME__, "%d:%d:%d", &hour, &min, &sec) == 3) {
		t.tm_hour = hour;
		t.tm_min = min;
		t.tm_sec = sec;
	} else {
		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;
	}

	Time_SetCalendarTime(t);
}

void DAC_Conf(void);

void Initialize(void)
{
	uint8_t rtc_fresh = 0;

	RCC_Configuration();
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Enable SPI2 clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	GPIO_Configuration();

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	rtc_fresh = (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5);
	RTC_Config();
	if (rtc_fresh) {
		set_rtc_to_build_time();
	}
	FSMC_LCD_Init();
	Delay(500000UL);    /* FSMC stabilize before TFT init (원본: 넉넉한 안정화) */
	TFT_Init();
	Serial_Open(SERIAL_PORT1,115200);
	Serial_Open(SERIAL_PORT2,115200);
	Serial_Open(SERIAL_PORT3,9600);
	FSMC_NAND_Init();

	System_TIMConfig();
	System_InterruptConfig();
	TIM8_Configuration();

	Spi_configuration();


	User_ADC_Configuration();

#ifdef NEW_BOARD
	 DAC_Conf();
#else
//	User_ADC_Configuration();
#endif


//	I2C_configuration();


	TFT_clear(0x07E0);  /* GREEN: LCD init diagnostic */
	Set_IWDG_ResetTime(5);   /* ~5초 (리셋 원인 조사용, 충분히 여유) */
	// IWDG_Start();   /* 일시 중지: 리셋 루프 원인 파악 시까지 */

	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	InitEeprom();
	SDIO_DeInit();

	RS485_DRIVE_LOW;

	RELAY1_OFF;
	RELAY2_OFF;
	RELAY3_OFF;


	GlobalMemory = (uint16_t*)imageBuffer;
}


void Spi_configuration (void)
{	 	
	  SPI_InitTypeDef   SPI_InitStructure;   
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                

    //SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                          
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  //SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 
    SPI_InitStructure.SPI_CRCPolynomial = 7;                            
    SPI_Init(SPI2, &SPI_InitStructure);
	   /* Enable SPI1 */
    SPI_Cmd(SPI2, ENABLE);
}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
						 RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG
						 , ENABLE);

	///////////////////////////////////////////////FSMC Configuration///////////////////////////////
#ifndef NEW_BOARD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; 	 //LCD-RST
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9 | 
													GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


///////////////////////////////////////////////////////////////////////FSMC Configuration/////////////////////////////////////
	//Input Buttton
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//Changed Input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//Out Relay
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	


#else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 	 //LCD-RST
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9 | 
								GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


#endif
///////////////////////////////////////////////////////////////////////FSMC Configuration/////////////////////////////////////
	//Input Buttton
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Changed Input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//Out Relay
#ifndef NEW_BOARD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
#else		   // 		C6,7, B5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
#endif	

	//RS485 Drive
#ifndef NEW_BOARD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
#else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	// I2C1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

	//SPI Drive


	// 4-20mA OUT PWM 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; // SENSOR 1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // SENSOR 2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // TEMP
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*Sys INDLED*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_12;	// 6=buz, 12=led
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*SD_DECTIVE & SelectorJumper */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;   // GPIO_Pin_8 : SD_DECTIVE ,  GPIO_Pin_11 : SelectorJumper
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//----------------------------- SPI2 ---------------------------------//

	/* Configure SPI2 pins: SCK, MISO and MOSI */
   GPIO_InitStructure.GPIO_Pin = SPI2_PIN_SCK | SPI2_PIN_MISO | SPI2_PIN_MOSI;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);

	/* Configure SPI2 pins: nCS ------------------------------------------------*/
   GPIO_InitStructure.GPIO_Pin = GPIO_SPI_NCS_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIO_SPI_CS, &GPIO_InitStructure);
}
void User_ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3 , ENABLE);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;	

	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADC1 regular channels configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);    
	/* Enable ADC1 DMA */
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	///////////////////////////////////	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));		


	////////////////////	
}
void System_TIMConfig(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	TIM_TimeBaseStructure.TIM_Period = 7200;  //50ms  1ms : 9
	TIM_TimeBaseStructure.TIM_Prescaler = 499;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);    
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
	TIM_TimeBaseStructure.TIM_Period = 7200;  //10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 99;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);    
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
	TIM_TimeBaseStructure.TIM_Period = 7200;  // every 50usec : 3600  
	TIM_TimeBaseStructure.TIM_Prescaler = 499; //50ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);    
	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
	TIM_TimeBaseStructure.TIM_Period = 7200;  //100us
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);    
	TIM_ITConfig(TIM5,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, DISABLE);
}

void TIM8_Configuration(void)
{

	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;

	// 1.TIM1 Clock - APB2
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 | RCC_APB2Periph_AFIO, ENABLE);

	// 2.Time Base
	//	- Carrier Frequency
	//	- 72MHz/6800 = 10Khz
	TIM_TimeBaseStructure.TIM_Prescaler			= 0;		              
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period			= 50000;	// 6800

	TIM_TimeBaseStructure.TIM_ClockDivision		= 0;								
	TIM_TimeBaseStructure.TIM_RepetitionCounter	= 0;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	// 3. PWM mode
	TIM_OCInitStructure.TIM_OCMode		= TIM_OCMode_PWM1;				    // CCMR1.OC2M

  TIM_OCInitStructure.TIM_OutputState	= TIM_OutputState_Disable;		// CCER.CC2E
	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputNState_Enable;	// CCER.CC2NE
	TIM_OCInitStructure.TIM_Pulse		= 0;							              // CCR2 (PWM Duty)
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High;			// CCER.CC2NP
	TIM_OCInitStructure.TIM_OCNPolarity	= TIM_OCNPolarity_High;			// CCER.CC2P
	TIM_OCInitStructure.TIM_OCIdleState	= TIM_OCIdleState_Reset;		// CR2.OIS2,  OCx=0/1 when MOE=0
	TIM_OCInitStructure.TIM_OCNIdleState= TIM_OCNIdleState_Reset;		// CR2.OIS2N, OCx=0/1 when MOE=0

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	//TIM_OC4Init(TIM8, &TIM_OCInitStructure);

  TIM_ClearFlag(TIM8, TIM_FLAG_Update);	
   /* TIM IT enable */
  //  TIM_ITConfig(TIM8, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
  //  TIM_ITConfig(TIM8, TIM_IT_CC1 , ENABLE);
 TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);	// DIER.UIE

	// 4.TIM8 Enable
	TIM_Cmd(TIM8, ENABLE);							// CR1.CEN

	// 5.Main Output Enable
	TIM_CtrlPWMOutputs(TIM8, ENABLE);				// BDTR.MOE	

}

void System_InterruptConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

	/* Configure the Priority Group to 2 bits */

	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}
void FSMC_NAND_Init(void)
{
	FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
	FSMC_NAND_PCCARDTimingInitTypeDef  p;  
	GPIO_InitTypeDef GPIO_InitStructure;  

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	p.FSMC_SetupTime  = 1;//0x15;
	p.FSMC_WaitSetupTime = 3;//0x15;
	p.FSMC_HoldSetupTime = 2;//0x15;
	p.FSMC_HiZSetupTime = 1;//0x15;

#ifndef NEW_BOARD
	FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank3_NAND; 
#else
	FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND; 
#endif

	FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable; 
	FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b; 
	//  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable; 
	FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Disable;
	FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes; 
	FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00; 
	FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00; 
	FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p; 
	FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;

	FSMC_NANDInit(&FSMC_NANDInitStructure); 

	/* FSMC NAND Bank Cmd Test */ 
#ifndef NEW_BOARD
	FSMC_NANDCmd(FSMC_Bank3_NAND, ENABLE);
#else
	FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
#endif


	/* NWAIT NAND pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   							 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	/* INT2 NAND pin configuration */  
	//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   							 
	// GPIO_Init(GPIOG, &GPIO_InitStructure);
	/* INT3 NAND pin configuration */  
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;   							 
	// GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void FSMC_LCD_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	p.FSMC_AddressSetupTime = 0x02;
	p.FSMC_AddressHoldTime = 0x00;
	p.FSMC_DataSetupTime = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0x00;
	p.FSMC_AccessMode = FSMC_AccessMode_B;

#ifndef NEW_BOARD
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
#else
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
#endif
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;	  

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	/* Enable FSMC Bank1_SRAM Bank */
#ifndef NEW_BOARD
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  
#else
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
#endif

}
void RCC_Configuration(void)
{   
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
  /* Enable Key Button GPIO Port, GPIO_LED and AFIO clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}


void DAC_Conf(void)
{
	DAC_InitTypeDef DAC_Type;


	DAC_Type.DAC_Trigger  =  DAC_Trigger_Software;
	DAC_Type.DAC_WaveGeneration	 = DAC_WaveGeneration_None;
	//DAC_Type.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1;
	DAC_Type.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_PinRemapConfig(GPIO_Remap_TIM67_DAC_DMA, ENABLE);

//	DAC_Init(DAC_Channel_1, &DAC_Type);
	DAC_Init(DAC_Channel_2, &DAC_Type);

//	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);


//	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);


	DAC_DualSoftwareTriggerCmd(ENABLE);

}


void DAC_output(unsigned char chan, unsigned int data)
{
	if (data>4095) data=4095;

	if (chan==1) {
		DAC_SetChannel1Data(DAC_Align_12b_R, data);
		DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	}
	else if (chan==2) {
		DAC_SetChannel2Data(DAC_Align_12b_R, data);
		DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
	}
	//DAC_DualSoftwareTriggerCmd(ENABLE);
}

