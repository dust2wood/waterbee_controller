#ifndef HEADER_H
#define HEADER_H


#define NEW_BOARD

#define SENSOR_PH_EC

//#define DISABLE_SLOPE_OFFSET


//#define IMSI_PPM_MODIFY	// 임시로 ppm 값을 약간 조정하는거.

#define CH2
//#define CH4


//#define BUZ_ENABLE


#define	SAVE_1_MINUTE	// 1분마다 저장, else 3분마다 저장(테스트필요)
										 
//#define SAVE_SECOND	// 초단위 데이터(60개) 저장,  else 1분데이터(1개) 저장



#include "misc.h"
#include "stm32f10x_conf.h"
#include "Initialize.h"
#include "Interrupt.h"

#include "RTC_Time.h"
#include "serial.h"
#include "Flash.h"
#include "Watchdog.h"
#include "Button.h"

#include "tft.h"
#include "NAND.h"
#include "Display.h"
#include "NandInterface.h"
#include "ReDisplay.h"
#include "NANDDisplay.h"

#include "DAC7512Interface.h"
#include "Communication.h"
#include "MicroSd.h"
#include "CurrentSorceTrans.h"
#include "sdio_sd.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>



#define SENSOR_OK		0
#define SENSOR_ERROR1	1	 // 센서점검, 범위 초과시
#define SENSOR_ERROR2	2	 // 램프이상 
#define SENSOR_ERROR3	3	 // 통신안됨, 센서연결실패
extern unsigned char Sensor_State1, Sensor_State2, Sensor_State3, Sensor_State4; 




typedef struct{                   //Menu->Config->Analog Value        아날로그 입력 변수들
	uint32_t output4mA;//Range 9999
	uint32_t output20mA;//Range 9999
	uint32_t outputReal4mA;//Range 99999
	uint32_t outputReal20mA;//Range 99999
	//uint32_t finaladj_4mA;
  //uint32_t finaladj_20mA;

	// 탁도 
	uint32_t output4mA2;//Range 9999
	uint32_t output20mA2;//Range 9999
	uint32_t outputReal4mA2;//Range 99999
	uint32_t outputReal20mA2;//Range 99999

}OutSetupValue;

typedef struct{                   //Menu->Config->Communication Value  통신 관련 입력 변수들
	uint16_t mode;//Range 1
	uint16_t baudrate;//Range 4
	uint16_t modbusSlaveAddr;//Range 246
	uint16_t databit;//Range 1
}ComSetupValue;


typedef struct{                   //Menu->Config->Communication Value  통신 관련 입력 변수들
	/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
	unsigned char IP_ADDR0;
	unsigned char IP_ADDR1;
	unsigned char IP_ADDR2;
	unsigned char IP_ADDR3;
	
	/*NETMASK*/
	unsigned char NETMASK_ADDR0;
	unsigned char NETMASK_ADDR1;
	unsigned char NETMASK_ADDR2;
	unsigned char NETMASK_ADDR3;
	
	/*Gateway Address*/
	unsigned char GW_ADDR0;
	unsigned char GW_ADDR1;
	unsigned char GW_ADDR2;
	unsigned char GW_ADDR3;
	
	unsigned short int ETHERNET_PORT;
}EthernetSetupValue;


typedef struct{                   //Menu->Config->Relay Value          릴레이 :  경보 설정 , 세정주기 , 세정기간 , 
	uint32_t relay1WashCycle;  // 세정으로 사용
	uint32_t relay1WashTime;
	uint32_t relay1manual;
	uint32_t relay2AutoAlarm;
	uint32_t relay2WashTime;
	uint32_t relay2AlarmOFF;
	uint32_t relay3WashCycle;  // 릴레이 출력으로 사용
	uint32_t relay3WashTime;
	uint32_t relay3manual;
}RelaySetupValue;

typedef struct{                    //Menu->Adjust Value
	uint32_t gradientS1;//Range 999  기울기
	uint32_t gradientS2;//Range 999
	int32_t offsetS1;//Range 999    오프셋
	int32_t offsetS2;//Range 999
	uint32_t filterS1;//Range 999    필터
	uint32_t filterS2;//Range 999
	uint32_t filter_update_flag;
}AdjustSetupValue;

typedef struct{                    //Menu->Calibration Value
	int32_t S1zeroCal;//Range 9999  //제로교정	
	int32_t S2zeroCal;//Range 9999


	uint32_t S1manualCal;//Range 9999 // 수동교정
	uint32_t S2manualCal;//Range 9999 
	int32_t S1Value;//Range 999      // 자동교정
	uint32_t S2StdValue;//Range 999
	uint32_t errorRange;//Range 999 
	uint32_t S2cycle;//Range 999      // 주기교정


	// ==============================										   	
	// PH
	int32_t PH4_Cal;// 버퍼교벙 ph4
	int32_t PH7_Cal;// 버퍼교정 PH7

	int32_t PH4_Value; // ph4 value
	int32_t PH7_Value;// PH7 value

	int32_t PH_Span_Cal;//Range 999      // 자동교정
	int32_t PH_Span_Value;//Range 999      // 자동교정


	// 전기 전도도 
	int32_t EC_Cal;//Range 9999
	int32_t EC_Value; // --> ???

	int32_t EC_Span_Cal;//Range 999      // 자동교정
	int32_t EC_Span_Value;//Range 999      // 자동교정


	// 온도 
	int16_t TEMP_Span_Cal1;//Range 999      // 자동교정
	int16_t TEMP_Span_Value1;//Range 999      // 자동교정
	int16_t TEMP_Span_Cal2;//Range 999      // 자동교정
	int16_t TEMP_Span_Value2;//Range 999      // 자동교정
	int16_t TEMP_Span_Cal3;//Range 999      // 자동교정
	int16_t TEMP_Span_Value3;//Range 999      // 자동교정
	int16_t TEMP_Span_Cal4;//Range 999      // 자동교정
	int16_t TEMP_Span_Value4;//Range 999      // 자동교정

}CalibrationSetupValue;


typedef struct{
	struct tm calibrationTime;
	uint32_t calibrationMethod;
	uint32_t calibrationValue;	
}CalibrationLogValue;

typedef struct{//Menu->Alarm Value
	uint32_t highLimit;//Range 9999
	uint32_t lowLimit;//Range 9999
	uint32_t zeroAlarm;//Range 999

	uint32_t highLimit2;//Range 9999
	uint32_t lowLimit2;//Range 9999
	uint32_t zeroAlarm2;//Range 999

	uint32_t highLimit3;//Range 9999
	uint32_t lowLimit3;//Range 9999
	uint32_t zeroAlarm3;//Range 999

	uint32_t highLimit4;//Range 9999
	uint32_t lowLimit4;//Range 9999
	uint32_t zeroAlarm4;//Range 999
}AlarmSetupValue;


typedef struct{//Config Value 
	OutSetupValue outputConfig;
	ComSetupValue modbusConfig;
	EthernetSetupValue EthernetConfig;
	RelaySetupValue relayConfig;
	AdjustSetupValue adjustConfig;
	CalibrationSetupValue calibrationConfig;
	AlarmSetupValue alarmConfig;
}ConfigSet;

typedef struct{                    //Measuring Value
	uint32_t current4_20mA;
	uint32_t S1PPM;
	uint32_t S2PPM;
	uint32_t S1mV;
	uint32_t S2mV;
	uint32_t S3mV;
	uint32_t S4mV;
	uint32_t S1mA;
	uint32_t S2mA;
	int16_t temperature;
	int16_t temperature1;
	int16_t temperature2;
	int16_t temperature3;
	uint32_t holdState;
	uint32_t relay1Prime;
	uint32_t relay2Prime;
	uint32_t relay3Prime;
	uint32_t adj_S1PPM;
	uint32_t adj_S2PPM;
	uint32_t comm_S1PPM;
	uint32_t comm_S2NTU;
	uint32_t comm_temperature;
	uint32_t filterout_S1PPM;
	uint32_t filterout_S1Current;
	uint32_t filterout_S2PPM;
	uint32_t S1mVolt;
	uint32_t S2mVolt;
	uint32_t S3mVolt;
	uint32_t S4mVolt;
	uint32_t Device_Selector_Mode;
	
}DataSet;

struct EEPROM_DATA {	
     uint16_t ReadEEpromData_Buffer[121];		// 111
     uint16_t WriteEEpromData_Buffer[121];	   	// 111
	   uint16_t EEPROM_write_responed;	
	   uint16_t EEPROM_Read_responed;	 
	   uint16_t EEPROM_DATA_Confirm[4];	 	   
};
extern struct EEPROM_DATA  EEpromdata;

	


#define INDLED_ON                  GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define INDLED_OFF                 GPIO_ResetBits(GPIOA, GPIO_Pin_12)


#ifndef NEW_BOARD
#define RELAY1_ON									GPIO_SetBits(GPIOF, GPIO_Pin_8)
#define RELAY1_OFF								GPIO_ResetBits(GPIOF, GPIO_Pin_8)
#define RELAY2_ON									GPIO_SetBits(GPIOF, GPIO_Pin_9)
#define RELAY2_OFF								GPIO_ResetBits(GPIOF, GPIO_Pin_9)
#define RELAY3_ON									GPIO_SetBits(GPIOF, GPIO_Pin_10)
#define RELAY3_OFF								GPIO_ResetBits(GPIOF, GPIO_Pin_10)
#else
	#define RELAY1_ON								GPIO_SetBits(GPIOC, GPIO_Pin_6)
	#define RELAY1_OFF								GPIO_ResetBits(GPIOC, GPIO_Pin_6)
	#define RELAY2_ON								GPIO_SetBits(GPIOC, GPIO_Pin_7)
	#define RELAY2_OFF								GPIO_ResetBits(GPIOC, GPIO_Pin_7)
	#define RELAY3_ON								GPIO_SetBits(GPIOB, GPIO_Pin_5)
	#define RELAY3_OFF								GPIO_ResetBits(GPIOB, GPIO_Pin_5)

	#define BUZ_ON                  GPIO_SetBits(GPIOA, GPIO_Pin_6)
	#define BUZ_OFF                 GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#endif

#define ADCBUFFER_SIZE						32

#define STATE_MAIN													0x00000000

#define STATE_MENU													0x00000001

#define STATE_CONFIG												0x00000011
#define STATE_CALIB													0x00000012
#define STATE_TREND													0x00000013
#define STATE_ALARM													0x00000014
#define STATE_DIAGNOSTIC										0x00000015

#define STATE_CONFIG_OUTPUT				  				0x00000111
#define STATE_CONFIG_COMM							  		0x00000112
#define STATE_CONFIG_RELAY									0x00000113
#define STATE_CONFIG_TIME										0x00000114
#define STATE_CONFIG_ADJUST									0x00000115
#define STATE_CALIB_ZERO										0x00000121
#define STATE_CALIB_MANUAL									0x00000122
#define STATE_CALIB_TEMP										0x00000123
#define STATE_CALIB_LOG											0x00000124
#define STATE_CALIB_S2_CYCLE							  0x00000125

#define STATE_CALIB_BUFF										0x00000121
#define STATE_CALIB_BUFF_PH4										0x00001211
#define STATE_CALIB_BUFF_PH7										0x00001212
#define STATE_CALIB_SPAN 									0x00000122	//	STATE_CALIB_MANUAL

#define STATE_CONFIG_COMM_ETHERNET					  		0x000001121



#define STATE_CONFIG_RELAY_RELAY1						0x00001131
#define STATE_CONFIG_RELAY_RELAY2						0x00001132
#define STATE_CONFIG_RELAY_RELAY3						0x00001133
#define STATE_CONFIG_ADJUST_GRADIENT				0x00001151
#define STATE_CONFIG_ADJUST_OFFSET					0x00001152
#define STATE_CONFIG_ADJUST_FILTER				  0x00001153
#define STATE_CONFIG_ADJUST_FACTORYRESET		0x00001154

#define STATE_CONFIG_RELAY_RELAY1_AUTO_WASH			0x00011311
#define STATE_CONFIG_RELAY_RELAY1_MAN_WASH			0x00011312
#define STATE_CONFIG_RELAY_RELAY2_AUTO_ALARM	0x00011321
#define STATE_CONFIG_RELAY_RELAY2_ALARM_OFF		0x00011322
#define STATE_CONFIG_RELAY_RELAY3_SET			0x00011331
#define STATE_CONFIG_RELAY_RELAY3_MAN			0x00011332

#define SAVEADDR_CONFIG_BASE								0x08070000

#define SENSOR_1_MODE										1
#define SENSOR_2_MODE										2
#define SENSOR_12_MODE										3


// SPI 설정 Define EEPROM
#define SPI2_GPIO              GPIOB
#define SPI2_PIN_SCK           GPIO_Pin_13
#define SPI2_PIN_MISO          GPIO_Pin_14
#define SPI2_PIN_MOSI          GPIO_Pin_15
#define GPIO_SPI_CS            GPIOB
#define GPIO_SPI_NCS_PIN       GPIO_Pin_12


//모드 점퍼 입력 
#define ReadJumper         GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
// 외부 입력 포트 
#ifndef NEW_BOARD
#define Ext_Input1             GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_13)
#define Ext_Input2             GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_14)
#define Ext_Input3             GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_15)
#else
	#define Ext_Input1             GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
	#define Ext_Input2             GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
	#define Ext_Input3             GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)
#endif



#define COMM_4_20MA 0
#define COMM_RS232 	1
#define COMM_RS485  2


extern unsigned int buz_time;
extern unsigned char comm_type;




extern unsigned char tmpIP_ADDR0;
extern unsigned char tmpIP_ADDR1;
extern unsigned char tmpIP_ADDR2;
extern unsigned char tmpIP_ADDR3;

/*NETMASK*/
extern unsigned char tmpNETMASK_ADDR0;
extern unsigned char tmpNETMASK_ADDR1;
extern unsigned char tmpNETMASK_ADDR2;
extern unsigned char tmpNETMASK_ADDR3;

/*Gateway Address*/
extern unsigned char tmpGW_ADDR0;
extern unsigned char tmpGW_ADDR1;
extern unsigned char tmpGW_ADDR2;
extern unsigned char tmpGW_ADDR3;

extern unsigned char tmpETHERNET_PORT;



extern const uint32_t Device_Selector;
//extern uint16_t LEDcoutner;
extern uint16_t zero_cal_update_flag ;
extern  uint16_t zero_cal_update_flag_S2 ;
extern uint32_t MicroSdCard_Time_Coutner;
extern uint32_t MicrosdCard_Run_flag;

extern int SD_TotalSize(void);	
extern FRESULT scan_files (char* path);
extern void MicroSD_Write (void);

extern uint32_t zero_Alarm_Time_couter;
extern uint32_t zero_Alarm_Time_couter2;
extern uint16_t wash_run_flag;
extern uint16_t Relay1_run_on_flag;

extern uint16_t wash_run_flag2 ;
extern uint16_t Relay2_run_on_flag;

extern uint16_t wash_run_flag3 ;
extern uint16_t Relay3_run_on_flag;


extern uint32_t flag_wash1_time_counter;
extern uint32_t flag_wash2_time_counter;
extern uint32_t flag_wash3_time_counter;

extern uint32_t flag_wash1_relay_time_counter;
extern uint32_t flag_wash2_relay_time_counter;
extern uint32_t flag_wash3_relay_time_counter;

//extern AdjustSetupValue adjData;
//extern CalibrationSetupValue CalSetpVauleData;

extern uint32_t state, subState, compareValue, cursor;
extern uint16_t adcBuffer[ADCBUFFER_SIZE], adcIndex;
//extern const uint32_t SENSOR_MODE;
extern char strBuffer[35];

extern DataSet currentData;
extern ConfigSet configData, tempConfigData;
extern CalibrationLogValue calibLog[4];
extern struct tm prevTime, currentTime, tempConfigTime;
extern uint16_t  trendValue[2][61];
//extern  int16_t  Mesure_Data_Value[4][61];	// [3][61]
extern uint16_t  Filter_Data_Value[4][50];	// [2][100]
extern  int32_t compareSignValue;
extern uint16_t sd_dec_flag;

extern  int16_t PPmSD_card_Data [60];
extern  int16_t TEMP_card_Data [60];
extern  int16_t PPmSD_card_Data2 [60];
extern  int16_t TEMP_card_Data2 [60];

extern uint16_t comm_run_flag ;
//extern uint16_t filter_update_flag;
extern uint16_t Alarm_OP_flag;

extern uint32_t Tx_Adj_TempertureVaule;
extern uint32_t Tx_S1manualCal;
extern uint32_t Tx_S2manualCal;
extern uint32_t req_ppm_com_flag;
extern uint32_t req_temp_com_flag;
extern uint32_t Tx_S1ZeroCal;
extern uint32_t Tx_S2ZeroCal;
extern uint32_t req_current_com_flag;
extern uint32_t req_zero_ppm_com_flag;
extern uint32_t req_zero_current_com_flag;

extern uint32_t SendParmeter_Current;
extern uint32_t SendParmeter_Zero_Current;

extern void InitEeprom(void);

extern void CalData_reqTx_handler(void);

extern void Update_CurrentTrans (void);
extern uint16_t Read_Channel_ADC1 (void);
extern uint16_t MpuToEEprom (uint16_t *MsgData , uint16_t Start_Address , uint16_t Byte_Counter);
extern uint16_t EEpromToMpu (uint16_t ReadStart_Address , uint16_t Byte_Counter);
extern void Clear_EEprom_Data (void);

void Delay_10msec (uint16_t Time);
void SysIND_LED (void);
void ClearstrBuffer(void);
void ReadSenosr2ADC (void);

void adj_Current_Cal (void);
void Wash1_Run_Handler (void);
void Set_Relay_OP_Run_Handler (void);
void Trend_Data_calc (void);
void bufferData_reset_function (void);
void SD_Detect_Draw (void);
void Alarm_Icon_Draw (void);
void Warning_Process_function(void);

void Update_Cal_Logdata (void);
void Save_CalLog_Data(uint16_t Type , int32_t calvaule);

void SystemSetting(void);
void StateHandler(void);
void State_Main(void);
void State_Menu(void);
void State_Config(void);
void State_Calib(void);
void State_Trend(void);
void State_Alarm(void);
void State_Diagnostic(void);
void State_ConfigOutput(void);
void State_ConfigOutput2(void);
void State_ConfigComm(void);
void State_ConfigRelay(void);
void State_ConfigTime(void);
void State_ConfigAdjust(void);
void State_CalibZero(void);
void State_CalibTemp(void);
void State_CalibManual(void);
void State_CalibS2Cycle(void);
void State_CalibLog(void);
void State_ConfigCommMode(void);
void State_ConfigCommBaud(void);
void State_ConfigCommID(void);
void State_ConfigCommDatabit(void);
void State_ConfigRelayRelay1(void);
void State_ConfigRelayRelay2(void);
void State_ConfigRelayRelay3(void);
void State_ConfigAdjustGradient(void);
void State_ConfigAdjustOffset(void);
void State_ConfigAdjustFilter(void);
void State_ConfigAdjustFactoryReset(void);
void State_ConfigRelayRelay1AutoWash(void);
void State_ConfigRelayRelay1manualWASH(void);
void State_ConfigRelayRelay2AutoAlarm(void);
void State_ConfigRelayRelay2AlarmOFF(void);
void State_ConfigRelayRelay3SetOP(void);
void State_ConfigRelayRelay3manualOP(void);
void MoveBackState(void);
void MoveSelectedState(void);
void RedrawTime(void);
void FactoryReset(void);
#endif
