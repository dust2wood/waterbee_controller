#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f10x.h"



#define NEW_BOARD


//#include "fsmc_sram.h"
//#include "delay.h"
#include "misc.h"

//#include "stm32f10x_exti.h"
#include "stm32f10x_conf.h"


//#include "RTC_Time.h"
//#include "serial.h"


//#include "stm32f10x_tim.h"
//#include "stm32f10x_adc.h"
//#include "platform_config.h"
//#include "Watchdog.h"

//#include "Flash.h"
//#include "Initialize.h"
//#include "Display.h"
#include "NAND.h"
//#include "Picture.h"
#include "NandInterface.h"
//#include "Interrupt.h"

//#include "tft.h"


typedef struct{
	uint16_t LimitV;
	uint16_t LimitI;
	uint16_t SPDVoltage;
	uint16_t SPDPhase;
	uint16_t SPDMode;
	
	uint16_t InitialFlash;
	uint16_t touchCalibrationXLeft;
	uint16_t touchCalibrationXRight;
	uint16_t touchCalibrationYTop;
	uint16_t touchCalibrationYBottom;
	
}SetupData;

#define ROTARY_A							GPIO_Pin_0
#define ROTARY_B							GPIO_Pin_1
#define ROTARY_PUSH					GPIO_Pin_2

#define GRAPH_X_ZERO				50
#define GRAPH_Y_ZERO				60+310 - 100   //310 == Min  ,  +100 == Zero X Line

#define FLAG50MS_TOUCH								0x0100


#define STATE_READY										0x0000
#define STATE_DISCHARGE								0x0001
#define STATE_SET_CHANNEL							0x0002

#define STATE_CHECKTRANS							0x0003
#define STATE_CHECK_LED								0x0004
#define STATE_SEARCH_VARISTOR					0x0005


#define STATE_RESULT_LIMIT							0x0006
#define STATE_RESULT_NORMAL					0x0007
#define STATE_RESULT_TRANS						0x0008
#define STATE_RESULT_TIMEOUT					0x0009
#define STATE_RESULT_LED							0x000A

#define STATE_RUN											0x000B
#define STATE_CONFIG									0x000C
#define STATE_VOLTAGEGAIN_ADJUST			0x000D
#define STATE_RESULT_DISPLAY					0x000E
#define STATE_RELEASE									0x000F

#define CONFIG_STATE_NULL							0
#define CONFIG_STATE_VOLTAGE_LIMIT			1
#define CONFIG_STATE_CURRENT_LIMIT			2
#define CONFIG_STATE_PHASE						3
#define CONFIG_STATE_VOLTAGE					4
#define CONFIG_STATE_OPERATING				5


#define SPDCHANNEL1_ENABLE				GPIO_SetBits(GPIOE, GPIO_Pin_4);
#define SPDCHANNEL1_DISABLE				GPIO_ResetBits(GPIOE, GPIO_Pin_4);
#define SPDCHANNEL2_ENABLE				GPIO_SetBits(GPIOE, GPIO_Pin_3);
#define SPDCHANNEL2_DISABLE				GPIO_ResetBits(GPIOE, GPIO_Pin_3);
#define SPDCHANNEL3_ENABLE				GPIO_SetBits(GPIOE, GPIO_Pin_2);
#define SPDCHANNEL3_DISABLE				GPIO_ResetBits(GPIOE, GPIO_Pin_2);

#define OUTPOWER_POSITIVE_ON				GPIO_SetBits(GPIOE, GPIO_Pin_5);
#define OUTPOWER_POSITIVE_OFF				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
#define OUTPOWER_NEGATIVE_ON				GPIO_SetBits(GPIOE, GPIO_Pin_6);
#define OUTPOWER_NEGATIVE_OFF			GPIO_ResetBits(GPIOE, GPIO_Pin_6);

#define DISCHARGE_SWITCH_ON			GPIO_SetBits(GPIOA, GPIO_Pin_3);
#define DISCHARGE_SWITCH_OFF			GPIO_ResetBits(GPIOA, GPIO_Pin_3);

#define LIMIT_VOLTAGE_MAX					1000
#define LIMIT_VOLTAGE_MIN					300
#define LIMIT_CURRENT_MAX					5000
#define LIMIT_CURRENT_MIN					3000

#define UC_VOLTAGE_MAX						500
#define UC_VOLTAGE_MIN						200


extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR;//背景颜色.默认为白色
extern uint8_t tmp[50];
extern uint16_t state;
extern uint16_t configState;
extern struct tm time_now;
extern struct tm configTime;
extern SetupData SetupVal;
extern SetupData tmpVal;
extern volatile uint16_t flag1ms;
extern volatile uint16_t flag50ms;
extern uint16_t* GlobalMemory;
extern uint32_t* GlobalMemory32;

extern uint32_t RX1Size;
extern uint32_t TX1Count;
extern uint32_t TX1Index;

/* Private function prototypes -----------------------------------------------*/
void lcd_rst(void);
void Delay(volatile uint32_t nCount);
void SPI_TP_Init(void);
void RXProcess1(uint8_t data);
void RXProcess3(uint8_t data);
void UserDMAInit(void);
void Set_System(void);
void USB_Interrupts_Config(void);
void Set_USBClock(void);
void USB_SendString(u8 *str);
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);
void GlobalMemoryRelease(uint16_t size);


void CheckRotary(void);
void RotaryHandler(void);
void StateProcess(void);

void TouchCalibrationMode(void);
void TouchPointAdjust(void);
void User_ADC_Configuration(void);





#endif
