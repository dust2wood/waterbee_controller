#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Main.h"

#ifndef NEW_BOARD
	#define RS485_DRIVE_LOW						GPIO_ResetBits(GPIOF, GPIO_Pin_12)	
	#define RS485_DRIVE_HIGH					GPIO_SetBits(GPIOF, GPIO_Pin_12)
#else
	#define RS485_DRIVE_LOW						GPIO_ResetBits(GPIOC, GPIO_Pin_4)	
	#define RS485_DRIVE_HIGH					GPIO_SetBits(GPIOC, GPIO_Pin_4)
#endif


extern uint32_t rx1HandlerCount, rx2HandlerCount, rx3HandlerCount;
extern uint32_t manual_cal_flag;
extern uint32_t manual_cal_temp_flag;
//extern uint32_t PPmSD_card_Data [60];
extern uint16_t WirteSDCard_flag;
extern const uint16_t CRC16_Table[256];
extern const uint16_t POLYNOMIAL;

void SendCalData(char dest, uint32_t vaule);
void SendCalDataTemp(char dest, uint32_t vaule);
void SendCalZeroData(char dest, uint32_t vaule);

void SensorComHandler(void);
void S1PPm_Data_offset_function (void);
uint32_t S1PPm_Filter_OUT_function(uint8_t sensor_no, uint32_t sensor);
void CalData_reqTx_handler(void);

uint16_t CRC16Modbus(uint16_t* nData, uint16_t wLength);
void Modbus232Handler(void);
void Modbus485Handler(void);

#endif

