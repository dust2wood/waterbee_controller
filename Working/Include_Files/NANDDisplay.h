#ifndef NAND_DISPLAY_H
#define NAND_DISPLAY_H

#include "Main.h"

#define DRAW_IMAGE_DISABLE					0
#define DRAW_IMAGE_ENABLE					1

#define ICON_CI										0
#define ICON_BELL									1
#define ICON_SDCARD									2
#define ICON_CAUTION								3
#define ICON_WORKING								4
#define ICON_CAUTION1								5

#define UNIT_RX											0
#define UNIT_TX											1
#define UNIT_YEAR										2
#define UNIT_MONTH									3
#define UNIT_DAY										4
#define UNIT_HOUR										5
#define UNIT_MIN											6
#define UNIT_SEC										7

#define TEXT55_ENABLE_WHITE					0
#define TEXT55_DISABLE_WHITE					1
#define TEXT55_ENABLE_YELLOW				2
#define TEXT55_DISABLE_YELLOW				3
#define TEXT55_ALARM								4
#define TEXT55_COMMUNICATION				5
#define TEXT55_OUTPUT								6
#define TEXT55_WASH									7
#define TEXT55_TIME									8
#define TEXT55_TEMPERATURE					9
#define TEXT55_CYCLE								10


#define TEXT96_NORAMAL				0    // ����
#define TEXT96_AUTO_CALIBRATION				1
#define TEXT96_ABNORAMAL			2  //  ������
#define TEXT96_SENSOR2_YELLOW			3
#define TEXT96_4MA									4
#define TEXT96_20MA									5
#define TEXT96_CALIBRATION						6
#define TEXT96_CALIBRATION_LOG				7
#define TEXT96_GRADIENT							8
#define TEXT96_RELAY								9
#define TEXT96_RELAY1								10
#define TEXT96_RELAY2								11
#define TEXT96_RELAY3								12
#define TEXT96_CONFIG								13
#define TEXT96_SENSOR2_CYCLE				14
#define TEXT96_MANUAL_CALIBRATION		15
#define TEXT96_TIME									16
#define TEXT96_ANALOG								17
#define TEXT96_ALARM								18
#define TEXT96_OFFSET								19
//#define TEXT96_AUTO_CALIBRATION			20 
#define TEXT96_TEMP_CALIBRATION			    20 
#define TEXT96_CI										21
#define TEXT96_ZERO_CALIBRATION			22
#define TEXT96_ADJUST								23
#define TEXT96_DIAGNOSIS							24
#define TEXT96_FACTORY_RESET				25
#define TEXT96_TREND								26
#define TEXT96_FRIME									27
#define TEXT96_FILTER								28
#define TEXT96_CI2                      29  //
#define TEXT96_CI3                      30
#define TEXT96_AUTO_WASHING      31
#define TEXT96_MANUAL_WASH       32
#define TEXT96_AUTO_ALARM           33
#define TEXT96_ALARM_OFF              34
#define TEXT96_SET_OPERATION      35
#define TEXT96_MANUAL_OPERATION      36


#define TEXT120_MEASURING						0
#define TEXT120_WASHING							1
#define TEXT120_CHECKING						2
#define TEXT120_ERROR								3
#define TEXT120_COM_METHOD					4
#define TEXT120_COM_BAUD						5
#define TEXT120_MODBUSID						6
#define TEXT120_COM_DATABIT					7
#define TEXT120_LIMIT_HIGH						8
#define TEXT120_LIMIT_LOW						9
#define TEXT120_ZERO_ALARM					10
/*
#define TEXT180_S1_ZERO_CAL					0
#define TEXT180_S2_ZERO_CAL					1
#define TEXT180_S1_MANUAL_CAL				2
#define TEXT180_S2_MANUAL_CAL				3
#define TEXT180_S1_VALUE							4
#define TEXT180_S2_STD_VALUE				5
#define TEXT180_ERR_RANGE						6
#define TEXT180_S2_PERIOD						7
*/
#define TEXT180_S1_ZERO_CAL					              0
#define TEXT180_S1_MANUAL_CAL			            	1
#define TEXT180_S1_TEMP_CAL				                2
#define TEXT180_S1_AUTO_CAL_VAULE				    3
#define TEXT180_S1_AUTO_CAL_REF				        4
#define TEXT180_S1_AUTO_CAL_COMPARE	    5
#define TEXT180_ERR_RANGE				                		6
#define TEXT180_S1_Current_Measure              7

#define TEXT260_S2_PRIME_W					0
#define TEXT260_S2_PRIME_Y						1
#define TEXT260_S2_PRIMEEXE_W				2
#define TEXT260_S2_PRIMEEXE_Y				3
#define TEXT260_S2_PRIME_ING_W				4
#define TEXT260_S2_PRIME_ING_Y				5

#define TEXT_ETC_PPM								0
#define TEXT_ETC_TEMPUNIT						1
#define TEXT_ETC_MGL								2
#define TEXT_ETC_MA									3
#define TEXT_ETC_MV									4
#define TEXT_ETC_RUN_W							5
#define TEXT_ETC_RUN_Y							6
#define TEXT_ETC_HOLD_W						7
#define TEXT_ETC_HOLD_Y							8
#define TEXT_ETC_NTU                   9
#define TEXT_ETC_STOP_W            10
#define TEXT_ETC_STOP_Y              11
#define TEXT_ETC_F_RESET_MESSAGE		12

//extern uint8_t imageBuffer[43200];
//extern uint8_t imageBuffer[48000];
//extern uint8_t imageBuffer[40960];
extern uint8_t imageBuffer[47104]; /* 50-row title slices are drawn as 49 rows + 1 row to stay within RAM */

extern uint16_t* GlobalMemory;

extern const uint16_t YellowMinus[], WhiteMinus[];

void DrawTextETC(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);
void DrawTextsize260(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);
void DrawTextsize180(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);
void DrawTextsize120(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);
void DrawTextsize96(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);
void DrawTextsize55(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);
void DrawIcon(uint32_t icon, uint32_t draw);
void DrawUnit(uint32_t x, uint32_t y, uint32_t image, uint32_t draw);

void DrawSmallNumber(uint32_t x, uint32_t y, char* str, uint16_t color);
void DrawMediumNumber(uint32_t x, uint32_t y, char* str, uint16_t color);
void DrawLargeNumber(uint32_t x, uint32_t y, char* str);

void DrawLineRectangle(uint32_t x, uint32_t y, uint32_t xEnd, uint32_t yEnd, uint16_t color);
void DrawBottomLine(void);
void ClearTitleArea(void);
void ClearTitle(void);
void ClearViewArea(void);
void ClearBottomArea(void);
void TFTDrawImage(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t* Image, uint16_t Flag);

#endif

