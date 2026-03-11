#include "Main.h"
#include "Sensor_Manager.h"
#include "sensor_autodetect.h"
#include "ui_mode.h"


unsigned int buz_time;	// max=65535*10msec=655.35sec
unsigned char comm_type=COMM_4_20MA;


unsigned char day_up=1, time_up=1;

unsigned char Sensor_State1 = SENSOR_ERROR3;
unsigned char Sensor_State2 = SENSOR_ERROR3;
unsigned char Sensor_State3 = SENSOR_ERROR3;
unsigned char Sensor_State4 = SENSOR_ERROR3;

const uint32_t Device_Selector = SENSOR_1_MODE;
char strBuffer[35];

uint32_t state = 0, subState = 0;
//uint16_t adcBuffer[ADCBUFFER_SIZE] = {0,};
uint16_t testImageCount = 0;
struct tm prevTime, currentTime, tempConfigTime;

uint32_t cursor = 0, cursorMax = 0, cursorWidth = 0;

DataSet currentData;
ConfigSet configData, tempConfigData;


// add pH, EC
uint16_t data_pH, data_pH_imsi, data_EC;		// ph = 0.00  temp = 0.0  ec=0.0
int16_t data_TEMP, SET_data_TEMP;


#define DEFAULT_4MA		6920 //1125   //1090
#define DEFAULT_20MA	41962 // 2710   //2525

#define DEFAULT_4MA2	6925 //1125   //1090
#define DEFAULT_20MA2	41971// 2710   //2525


//uint32_t adjdefault4mA = 400; 	//600;	  4-20mA ???????? ????????? ???? ??????. ?????? 600???.
uint32_t adjdefault4mA = 0; 	//600;	  4-20mA ???????? ????????? ???? ??????. ?????? 600???.
uint32_t adjdefault20mA = 0; 	// 5000;
uint32_t campare_4mA = 0;
uint32_t campare_20mA = 0;

uint16_t adj_run_flag = 0;

uint32_t compareValue = 0;
int32_t compareSignValue = 0;
int32_t compareSigntemp = 0;

uint16_t wash_run_flag = 0;
uint16_t Relay1_run_on_flag = 0;

uint16_t Relay2_off_time=0;
uint16_t Relay2_off_time2=0;


uint16_t wash_run_flag2 = 0;
uint16_t Relay2_run_on_flag = 0;

uint16_t wash_run_flag3 = 0;
uint16_t Relay3_run_on_flag = 0;

uint16_t sd_flag = 0;
uint16_t Alarm_OP_flag = 0;

uint32_t WashCycleVaule1 = 0;
uint32_t Run_ON_RelayVaule1 = 0;

uint32_t WashCycleVaule3 = 0;
uint32_t Run_ON_RelayVaule3 = 0;

CalibrationLogValue calibLog[4];


// trand ??????? ?????? 
// ??????? ????? ?????? ?????? ??????? ??????? ???.
uint16_t trand_time1, trand_time6, trand_time12, trand_time24, trand_time168, trand_time336;

// Trand ?? 60??(1??????), 6????(6??????), 12????(12??????), 24????(24??????), 168????=7??(168??????), 336????=14??(336??????)
#define trand_select_time_NO 6		// ???????? ?????????
uint16_t trand_select_time=0;		// ???????? ?????????
uint16_t trand_select_Y=0, trand_select_Y_NO=3;		// Y ?? ?????? ?????????


// ???????? ??? ????
char flag_write_trand1, flag_write_trand6, flag_write_trand12, flag_write_trand24, flag_write_trand168, flag_write_trand336;


int16_t trandData[trand_select_time_NO][61] = {	 // [6][61]
    {0,},
    {0,},
    {0,},
    {0,},
    {0,},
    {0,}};

int16_t trandData2[trand_select_time_NO][61] = {	 // [6][61]  ???
    {0,},
    {0,},
    {0,},
    {0,},
    {0,},
    {0,}};


uint8_t trandData_TIME[trand_select_time_NO][61][6]; 


uint16_t trendValue[2][61] = {
    {0,},
    {0,}};

uint16_t Filter_Data_Value[4][50] = {	 //  [2][100]
    {0,},
    {0,},
    {0,},
    {0,}};

int16_t PPmSD_card_Data [60] = {0,};
int16_t TEMP_card_Data [60] = {0,};

int16_t PPmSD_card_Data2 [60] = {0,};	 // ??? 
int16_t TEMP_card_Data2 [60] = {0,};


unsigned char LogData [111] = {0,};
uint16_t LogDataCounter = 0;

int temp_year_mask = 0;
int32_t temp_vaule_mask = 0;

uint32_t testvaule = 1000;

uint32_t Tx_Adj_TempertureVaule;
uint32_t Tx_S1manualCal;
uint32_t Tx_S2manualCal;

uint32_t Tx_S1ZeroCal;
uint32_t Tx_S2ZeroCal;

uint32_t manual_cal_flag = 0;
uint32_t manual_cal_temp_flag = 0;

uint16_t sd_dec_flag = 0;
uint16_t tp_cotner = 0;
uint16_t tp2_counter = 0;

uint16_t zero_cal_update_flag = 0;
uint16_t zero_cal_update_flag_S2 = 0;

int caldisp_counter = 0;
uint16_t caldisp_array_counter = 0;

uint16_t tp_counter = 0;
uint16_t tx_trans_flag = 0;


extern uint16_t Counter10msec;
extern uint16_t Comm_time_flag;


void display_set_set(unsigned char select);
void display_set_cal(unsigned char select);
void display_set_trand(unsigned char select);
void display_set_alarm(unsigned char select);
void display_set_diag(unsigned char select);


void State_CalibBuff(void);
void State_CalibBuffPH4(void);
void State_CalibBuffPH7(void);
void State_CalibSpan(void);

void State_CalibBuff_EC(void);


//Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 
extern unsigned char IP_ADDR0;
extern unsigned char IP_ADDR1;
extern unsigned char IP_ADDR2;
extern unsigned char IP_ADDR3;

//NETMASK
extern unsigned char NETMASK_ADDR0;
extern unsigned char NETMASK_ADDR1;
extern unsigned char NETMASK_ADDR2;
extern unsigned char NETMASK_ADDR3;

//Gateway Address
extern unsigned char GW_ADDR0;
extern unsigned char GW_ADDR1;
extern unsigned char GW_ADDR2;
extern unsigned char GW_ADDR3;

extern unsigned short int ETHERNET_PORT;

void Netif_Config(void);
void State_ConfigEthernet(void);


//#define SD_TEST


//extern int16_t trandData[trand_select_time_NO][61];


#ifdef CH2
	#include "Redisplay_ch2.c"
#elif CH4
	#include "Redisplay_ch4.c"
#else 
	#include "Redisplay.c"
#endif


int main(void) 
{
	char led = 0;
	unsigned int data=0;

	/* 백라이트 최우선: 모든 초기화보다 앞서 PD3 High (리셋 루프 시에도 화면부터 켜짐) */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	{ GPIO_InitTypeDef g = {0}; g.GPIO_Pin = GPIO_Pin_3; g.GPIO_Mode = GPIO_Mode_Out_PP; g.GPIO_Speed = GPIO_Speed_50MHz; GPIO_Init(GPIOD, &g); }
	GPIO_SetBits(GPIOD, GPIO_Pin_3);

	Initialize();
	SysIND_LED();
    SystemSetting();

	/* PnP 안전: 로고 먼저 표시 후 3초 대기, 그 다음 센서 스캔 */
	display_logo();
	Delay_10msec(300);
    sensor_autodetect_run();
    sensor_manager_init();
    bufferData_reset_function();
    Update_Cal_Logdata();
    configData.adjustConfig.filter_update_flag = 1;

BUZ_ON;
	buz_time=500;
	Delay_10msec(3000);
	Delay_10msec(3000);

//    DrawBack1();


	MicroSD_Trand_Read();

#ifdef  SENSOR_PH_EC	
	init_tx3Buffer();
#endif

	sensor_manager_init();   /* ???? ??? ???? ????? ???? */
    GetUiMode();

    // test
    //	DrawBack2();
    //	DrawIcon(ICON_CI, DRAW_IMAGE_ENABLE);		// test
    //	DrawIcon(ICON_SDCARD, DRAW_IMAGE_ENABLE);	// test
    //	DrawIcon(ICON_BELL, DRAW_IMAGE_ENABLE);		// test
    //	DrawIcon(ICON_CAUTION, DRAW_IMAGE_ENABLE);	// test
    //	DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE);	// test


//for (data=0;;)
{

//	TIM8_Chage_Duty_Channel(0,data);
//	TIM8_Chage_Duty_Channel(1,data);
//	TIM8_Chage_Duty_Channel(2,data);   	// PB0, LAN.5
//	TIM8_Chage_Duty_Channel(3,data);	// PB1, LAN.4, TEMP

//data++;

//	if (data>0x1000) data=0;
//	 Delay_10msec(10);
}

data=1000;
//    	for (;;){
//			DAC_output(1, data);
			TIM8_Chage_Duty_Channel(2,data);   	// PB0, LAN.5

			DAC_output(2, data);
  			 Delay_10msec(50);
//		}

    DrawBack1();


	// ??????? ???????
	if (configData.relayConfig.relay1WashCycle>0) wash_run_flag = 1;	
	// ????3 ???????
	if (configData.relayConfig.relay3WashCycle>0) wash_run_flag3 = 1;

	//======================================
	if (configData.EthernetConfig.IP_ADDR0>0 && configData.EthernetConfig.IP_ADDR0<255) {
		IP_ADDR0=configData.EthernetConfig.IP_ADDR0;
		IP_ADDR1=configData.EthernetConfig.IP_ADDR1;
		IP_ADDR2=configData.EthernetConfig.IP_ADDR2;
		IP_ADDR3=configData.EthernetConfig.IP_ADDR3;

		//NETMASK
		NETMASK_ADDR0 =  configData.EthernetConfig.NETMASK_ADDR0;
		NETMASK_ADDR1 =  configData.EthernetConfig.NETMASK_ADDR1;
		NETMASK_ADDR2 =  configData.EthernetConfig.NETMASK_ADDR2;
		NETMASK_ADDR3 =  configData.EthernetConfig.NETMASK_ADDR3;

		//Gateway Address
		GW_ADDR0 =  configData.EthernetConfig.GW_ADDR0;
		GW_ADDR1 =  configData.EthernetConfig.GW_ADDR1;
		GW_ADDR2 =  configData.EthernetConfig.GW_ADDR2;
		GW_ADDR3 =  configData.EthernetConfig.GW_ADDR3;

		ETHERNET_PORT = configData.EthernetConfig.ETHERNET_PORT;
	}
	else {
		//Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 
		IP_ADDR0=192;
		IP_ADDR1=168;
		IP_ADDR2=1;
		IP_ADDR3=25;

		//NETMASK
		NETMASK_ADDR0 =  255;
		NETMASK_ADDR1 =  255;
		NETMASK_ADDR2 =  255;
		NETMASK_ADDR3 =  0;

		//Gateway Address
		GW_ADDR0 =  192;
		GW_ADDR1 =  168;
		GW_ADDR2 =  1;
		GW_ADDR3 =  1;

		ETHERNET_PORT=502;

		configData.EthernetConfig.IP_ADDR0 = IP_ADDR0;
		configData.EthernetConfig.IP_ADDR1 = IP_ADDR1;
		configData.EthernetConfig.IP_ADDR2 = IP_ADDR2;
		configData.EthernetConfig.IP_ADDR3 = IP_ADDR3;

		configData.EthernetConfig.NETMASK_ADDR0 = NETMASK_ADDR0;
		configData.EthernetConfig.NETMASK_ADDR1 = NETMASK_ADDR1;
		configData.EthernetConfig.NETMASK_ADDR2 = NETMASK_ADDR2;
		configData.EthernetConfig.NETMASK_ADDR3 = NETMASK_ADDR3;

		configData.EthernetConfig.GW_ADDR0 = GW_ADDR0;
		configData.EthernetConfig.GW_ADDR1 = GW_ADDR1;
		configData.EthernetConfig.GW_ADDR2 = GW_ADDR2;
		configData.EthernetConfig.GW_ADDR3 = GW_ADDR3;

		configData.EthernetConfig.ETHERNET_PORT = ETHERNET_PORT;
	}

	main_ethernet();

	//======================================


BUZ_OFF;


    while (1) {
		IWDG_ReloadCounter();  /* 매 루프 워치독 리셋 방지 */

		if (led==0) { INDLED_ON; led=1; }
		else 		{ INDLED_OFF; led=0; }


        // STATE, LCD OUT
        StateHandler();

        //for (;;){}				  

        // SENSOR INPUT
        SensorComHandler();

        /* ???? ??? ???? ???? (Modbus485Handler ???? ???? ?????? ??) */
        sensor_manager_update();

        // 4-20mA ???
        Update_CurrentTrans();

        // RELAY1 ON/OFF
        Wash1_Run_Handler();

        // RELAY3 ON/OFF
        Set_Relay_OP_Run_Handler();

        // WARNING
        Warning_Process_function();

        // 232		
        if (configData.modbusConfig.mode == 0 && currentData.holdState == 0) {
            Modbus232Handler();
        }            // 485 
        else if (configData.modbusConfig.mode == 1 && currentData.holdState == 0) {
            Modbus485Handler();
        }
        else if (configData.modbusConfig.mode == 2 && currentData.holdState == 0) {
       		Ethernet_loop();
        }

        /* ???? ????? ???? (Modbus ???? currentData ???) */
        sensor_manager_update();

        // CALIBRATION. ???? ???				
        if (manual_cal_flag == 1) {
            if ((currentData.Device_Selector_Mode & SENSOR_1_MODE) && (req_ppm_com_flag == 0)) {
                SendCalData(1, Tx_S1manualCal);
            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE && tx_trans_flag == 0) {
                SendCalData(2, Tx_S2manualCal);
                tx_trans_flag = 1;
            }
            if (state == 0) {
                //DrawTextsize96(40, 70, TEXT96_MANUAL_CALIBRATION, DRAW_IMAGE_ENABLE);
				//display_set3_span(1);
            }
            Delay_10msec(300);

//            if ((currentData.Device_Selector_Mode & SENSOR_1_MODE) && (req_ppm_com_flag == 1)) {
            if ((currentData.Device_Selector_Mode & SENSOR_1_MODE) ) {
                Delay_10msec(300);
                //if (req_current_com_flag == 1 && req_ppm_com_flag == 1) {
                //if (req_ppm_com_flag == 1) {
                    manual_cal_flag = 0;
                    req_ppm_com_flag = 0;
                    req_current_com_flag = 0;
                //}
            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                Delay_10msec(300);
                //if (req_current_com_flag == 1 && req_ppm_com_flag == 1) {
//                if (req_ppm_com_flag == 1) {
                    tx_trans_flag = 0;
                    manual_cal_flag = 0;
                    req_ppm_com_flag = 0;
                    req_current_com_flag = 0;
  //              }
            }
        }

        // TEMP. CAL ??? ????
        if (manual_cal_temp_flag == 1) {
            manual_cal_temp_flag = 0;

#ifndef SENSOR_PH_EC
            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
 	            SendCalDataTemp(1, Tx_Adj_TempertureVaule);
            if (currentData.Device_Selector_Mode & SENSOR_2_MODE) 
 	            SendCalDataTemp(1, Tx_Adj_TempertureVaule);
#else
            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
 	            SendCalDataTemp(3, Tx_Adj_TempertureVaule);
            if (currentData.Device_Selector_Mode & SENSOR_2_MODE) 
 	            SendCalDataTemp(4, Tx_Adj_TempertureVaule);
#endif

            if (state == 0) {
                //DrawTextsize96(40, 70, TEXT96_TEMP_CALIBRATION, DRAW_IMAGE_ENABLE);
				//display_set3_temp(1);
            }
            Delay_10msec(300);
            if (req_temp_com_flag == 1) {
                manual_cal_temp_flag = 0;
                req_temp_com_flag = 0;
            }
        }

        // ZERO CAL   ???? ????
        if (zero_cal_update_flag == 1 && manual_cal_flag == 0) {
            zero_cal_update_flag = 0;

            if ((currentData.Device_Selector_Mode & SENSOR_1_MODE) && (req_zero_ppm_com_flag == 0)) {
                SendCalZeroData(1, Tx_S1ZeroCal);
//            } else if ((currentData.Device_Selector_Mode & SENSOR_2_MODE) && (req_zero_ppm_com_flag == 0) && (tx_trans_flag == 0)) {
            } else if ((currentData.Device_Selector_Mode & SENSOR_2_MODE) && (req_zero_ppm_com_flag == 0) ) {
                SendCalZeroData(2, Tx_S2ZeroCal);
                tx_trans_flag = 1;
            }

            if (state == 0) {
                //DrawTextsize96(40, 70, TEXT96_ZERO_CALIBRATION, DRAW_IMAGE_ENABLE);
				//display_set3_zero(1);
            }
            Delay_10msec(300);
            if (req_zero_ppm_com_flag == 1) {// && req_zero_current_com_flag == 1) {
                zero_cal_update_flag = 0;
                req_zero_current_com_flag = 0;
                req_zero_ppm_com_flag = 0;
                tx_trans_flag = 0;
            }
        }

        // SD ?? ???????
#ifdef SD_TEST
        if (sd_dec_flag == 1) {	  
#else
        if (MicrosdCard_Run_flag == 1 && sd_dec_flag == 1) {
#endif
            MicroSD_Write();
            MicrosdCard_Run_flag = 0;
        }


		MicroSD_Trand_Write();

    }//while

}

//=======================================================

volatile uint16_t delay_temp;

void Delay_10msec(uint16_t Time) {
    int i = 0,j;
    for (i = 0; i < Time; i++) {
		IWDG_ReloadCounter();
		for (j=0;j<1000;j++) {
        delay_temp = 1;
		}
    }
}

/* 1ms 단위 딜레이 - while 루프 내 1ms 간격 IWDG 킥용 */
void Delay_1msec(uint16_t Time) {
    int i, j;
    for (i = 0; i < Time; i++) {
		IWDG_ReloadCounter();
		for (j = 0; j < 100; j++) delay_temp = 1;
    }
}

void SysIND_LED(void) {
    if (ReadJumper == 1) {
        INDLED_ON;
        currentData.Device_Selector_Mode = SENSOR_1_MODE;
    } else if (ReadJumper == 0) {
        INDLED_OFF;
        currentData.Device_Selector_Mode = SENSOR_2_MODE;
    }
}

void Update_Cal_Logdata(void) {
    uint16_t i;

    EEpromToMpu(0, 111);

    for (i = 0; i < 111; i++) {
        LogData[i] = EEpromdata.ReadEEpromData_Buffer[i];
    }

    caldisp_counter = 0;
    caldisp_array_counter = 0;
}

void bufferData_reset_function(void) {
    int i = 0;
    int j = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 60; j++) {
            trendValue[i][j] = 0x00;
//            Mesure_Data_Value[i][j] = 0xFFFF;
        }
    }

    for (j = 0; j < 60; j++) {
        PPmSD_card_Data [j] = 0x00;
        PPmSD_card_Data2[j] = 0x00;
        TEMP_card_Data[j] = 0;
        TEMP_card_Data2[j] = 0;
    }
}


#define RELAY2_OFF_TIME	300		// 5??...

uint32_t warning_counter = 0;
uint32_t warning_counter1 = 0;
uint32_t warning_counter2 = 0;
uint32_t warning_counter3 = 0;

uint16_t zero_Alarm_sec = 0;
uint16_t zero_Alarm_min = 0;
uint16_t zero_error_flag = 0;

uint16_t zero_Alarm_sec2 = 0;
uint16_t zero_Alarm_min2 = 0;
uint16_t zero_error_flag2 = 0;

void Warning_Process_function(void) {
//    if (Alarm_OP_flag == 1 && configData.relayConfig.relay2AlarmOFF == 0) {
//    if (Alarm_OP_flag == 1 && configData.relayConfig.relay2AutoAlarm == 1) {
    if (configData.relayConfig.relay2AutoAlarm == 1) {
        if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
            if (currentData.S1PPM > (configData.alarmConfig.highLimit )) {
                warning_counter++;
				if (warning_counter>50) warning_counter=50;
            } else {
                if (warning_counter > 0) warning_counter--;
            }
            if (currentData.S1PPM < (configData.alarmConfig.lowLimit )) {
                warning_counter1++;
				if (warning_counter1>50) warning_counter1=50;
            } else {
                if (warning_counter1 > 0) warning_counter1--;
            }
        } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
            if (currentData.S2PPM > (configData.alarmConfig.highLimit2)) {
                warning_counter2++;
				if (warning_counter2>50) warning_counter2=50;
            } else {
                if (warning_counter2 > 0) warning_counter2--;
            }
            if (currentData.S2PPM < (configData.alarmConfig.lowLimit2)) {
                warning_counter3++;
				if (warning_counter3>50) warning_counter3=50;
            } else {
                if (warning_counter3 > 0) warning_counter3--;
            }
        }


//       if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
 #ifndef SENSOR_PH_EC
	   		if (currentData.S1PPM == 0) {
	            if (zero_Alarm_Time_couter >= 10 && zero_error_flag == 0) {
					zero_Alarm_Time_couter =0;

	                zero_Alarm_sec++;
		            if (zero_Alarm_sec >= 60) {
		                zero_Alarm_sec = 0;
		                zero_Alarm_min++;
		                zero_Alarm_Time_couter = 0;
			            if (configData.alarmConfig.zeroAlarm == zero_Alarm_min) {
			                zero_error_flag = 1;
			            }
		            }
	            }
	        } else {
	            zero_Alarm_Time_couter = 0;
	            zero_Alarm_sec = 0;
	            zero_Alarm_min = 0;
	            zero_error_flag = 0;
	        }
//		}

//        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
			if (currentData.S2PPM == 15) { //50msec
            	if (zero_Alarm_Time_couter2 >= 10 && zero_error_flag2 == 0) {
					zero_Alarm_Time_couter2 =0;

	                zero_Alarm_sec2++;
		            if (zero_Alarm_sec2 >= 60) {
	    	            zero_Alarm_sec2 = 0;
	        	        zero_Alarm_min2++;
	            	    zero_Alarm_Time_couter2 = 0;
		            	if (configData.alarmConfig.zeroAlarm2 == zero_Alarm_min2) {
		                	zero_error_flag2 = 1;
			            }
		            }
        		}
        	} else {
	            zero_Alarm_Time_couter2 = 0;
	            zero_Alarm_sec2 = 0;
	            zero_Alarm_min2 = 0;
	            zero_error_flag2 = 0;
	        }
#else
	   		if (currentData.S1PPM == 0) {
	            if (zero_Alarm_Time_couter >= 10 && zero_error_flag == 0) {
					zero_Alarm_Time_couter =0;

	                zero_Alarm_sec++;
		            if (zero_Alarm_sec >= 60) {
		                zero_Alarm_sec = 0;
		                zero_Alarm_min++;
		                zero_Alarm_Time_couter = 0;
			            if (configData.alarmConfig.zeroAlarm == zero_Alarm_min) {
			                zero_error_flag = 1;
			            }
		            }
	            }
	        } else {
	            zero_Alarm_Time_couter = 0;
	            zero_Alarm_sec = 0;
	            zero_Alarm_min = 0;
	            zero_error_flag = 0;
	        }
//		}

//        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
			if (currentData.S2PPM == 0) { //50msec
            	if (zero_Alarm_Time_couter2 >= 10 && zero_error_flag2 == 0) {
					zero_Alarm_Time_couter2 =0;

	                zero_Alarm_sec2++;
		            if (zero_Alarm_sec2 >= 60) {
	    	            zero_Alarm_sec2 = 0;
	        	        zero_Alarm_min2++;
	            	    zero_Alarm_Time_couter2 = 0;
		            	if (configData.alarmConfig.zeroAlarm2 == zero_Alarm_min2) {
		                	zero_error_flag2 = 1;
			            }
		            }
        		}
        	} else {
	            zero_Alarm_Time_couter2 = 0;
	            zero_Alarm_sec2 = 0;
	            zero_Alarm_min2 = 0;
	            zero_error_flag2 = 0;
	        }
#endif
//		}


		if (Ext_Input2!=0)  RELAY2_ON;

        else if (Relay2_off_time==0 && (warning_counter >=20 || warning_counter1 >=20 || zero_error_flag == 1)) {
            if (state == 0) {
                DrawIcon(ICON_BELL, DRAW_IMAGE_ENABLE);
            } else {
                DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
            }
            RELAY2_ON;

		}

        else if (Relay2_off_time2==0 && (warning_counter2 >=20 || warning_counter3 >=20 || zero_error_flag2 == 1)) {
            if (state == 0) {
                DrawIcon(ICON_BELL, DRAW_IMAGE_ENABLE);
            } else {
                DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
            }
            RELAY2_ON;

		} else {
            if (state == 0) {
                DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
            }
//            if (configData.relayConfig.relay2AutoAlarm == 1) {
                RELAY2_OFF;
//            }
        }


//    } else if (Alarm_OP_flag == 0 && configData.relayConfig.relay2AlarmOFF == 1) {
    }
	else {
           DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
           RELAY2_OFF;

            zero_Alarm_Time_couter = 0;
            zero_Alarm_sec = 0;
            zero_Alarm_min = 0;
            zero_error_flag = 0;

            zero_Alarm_Time_couter2 = 0;
            zero_Alarm_sec2 = 0;
            zero_Alarm_min2 = 0;
            zero_error_flag2 = 0;

			warning_counter=0;
			warning_counter1=0;
			warning_counter2=0;
			warning_counter3=0;
	}


	// ????????????? ????????????? ??????. 
	if (state==0) {	
    	if (Sensor_State2 == SENSOR_OK) DrawIcon(ICON_CAUTION, DRAW_IMAGE_DISABLE);
    	else 							DrawIcon(ICON_CAUTION, DRAW_IMAGE_ENABLE);
	}
	if (state==0) {	
    	if (Sensor_State1 == SENSOR_OK) DrawIcon(ICON_CAUTION1, DRAW_IMAGE_DISABLE);
    	else 							DrawIcon(ICON_CAUTION1, DRAW_IMAGE_ENABLE);
	}
}


// ???????...
void Wash1_Run_Handler(void) {

	if (configData.relayConfig.relay1manual == 0) 
	{
	    WashCycleVaule1 =    (configData.relayConfig.relay1WashCycle * 60) / 0.1;
	    Run_ON_RelayVaule1 = (configData.relayConfig.relay1WashTime  * 60) / 0.1;

	    if ((WashCycleVaule1 < flag_wash1_time_counter) && (wash_run_flag == 1)) {
	        flag_wash1_time_counter = 0;
	        wash_run_flag = 0;
	        Relay1_run_on_flag = 1;
	    } else {

	    }

		if (Ext_Input1!=0)  RELAY1_ON;

	    else if ((Run_ON_RelayVaule1 > flag_wash1_relay_time_counter) && (Relay1_run_on_flag == 1)) {
	        if (state == 0) {
	            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                //DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_ENABLE);
					display_message1();
	            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
	                //DrawTextsize120(20, 140, TEXT120_WASHING, DRAW_IMAGE_ENABLE);
					display_message1();
	            }
	        } else {
	            RedrawValue();
	        }
	        RELAY1_ON;
	        Relay2_off_time = RELAY2_OFF_TIME;
	        Relay2_off_time2 = RELAY2_OFF_TIME;

	    } else if ((Run_ON_RelayVaule1 < flag_wash1_relay_time_counter) && (Relay1_run_on_flag == 1)) {

	        if (state == 0) {
	            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                //DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_DISABLE);
					display_message1();
	            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
	                //DrawTextsize120(20, 140, TEXT120_WASHING, DRAW_IMAGE_DISABLE);
					display_message1();
	            }
	        } else {
	            RedrawValue();
	        }

			RELAY1_OFF;
	        Relay1_run_on_flag = 0;
	        Relay2_off_time = RELAY2_OFF_TIME;
	        Relay2_off_time2 = RELAY2_OFF_TIME;

	        flag_wash1_relay_time_counter = 0;
	        wash_run_flag = 1;

			ClearViewArea5();
			RedrawMainValue();
	        RedrawValue();
	    }
	}
}

void Set_Relay_OP_Run_Handler(void) {

	if (configData.relayConfig.relay3manual==0)
	{
	    WashCycleVaule3    = (configData.relayConfig.relay3WashCycle * 60) / 0.1;
	    Run_ON_RelayVaule3 = (configData.relayConfig.relay3WashTime  * 60) / 0.1;

	    if ((WashCycleVaule3 < flag_wash3_time_counter) && (wash_run_flag3 == 1)) {
	        flag_wash3_time_counter = 0;
	        wash_run_flag3 = 0;
	        Relay3_run_on_flag = 1;
	    } else {

	    }				  	

		if (Ext_Input3!=0)  RELAY3_ON;

		else if ((Run_ON_RelayVaule3 > flag_wash3_relay_time_counter) && (Relay3_run_on_flag == 1)) {
	        if (state == 0) {
	            // DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_ENABLE);
				display_message1();
	        } else {
	            RedrawValue();
	        }
	        RELAY3_ON;
	    } else if ((Run_ON_RelayVaule3 < flag_wash3_relay_time_counter) && (Relay3_run_on_flag == 1)) {

	        if (state == 0) {
	            //DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_DISABLE);
				display_message1();
	        } else {
	            RedrawValue();
	        }

	        RELAY3_OFF;
	        Relay3_run_on_flag = 0;
	        flag_wash3_relay_time_counter = 0;
	        wash_run_flag3 = 1;

			ClearViewArea5();
			RedrawMainValue();
	        RedrawValue();
	    }
	}
}

void SystemSetting(void) {
    configData = *((ConfigSet*) (SAVEADDR_CONFIG_BASE));

#ifdef SENSOR_PH_EC
    /* RS485 센서 Slave ID 검증 - 중복 시 보정, 무효 시 기본값 */
    if (configData.modbusConfig.modbusSensor1Addr < 1 || configData.modbusConfig.modbusSensor1Addr > 247)
        configData.modbusConfig.modbusSensor1Addr = 2;
    if (configData.modbusConfig.modbusSensor2Addr < 1 || configData.modbusConfig.modbusSensor2Addr > 247)
        configData.modbusConfig.modbusSensor2Addr = 4;
    if (configData.modbusConfig.modbusSensor1Addr == configData.modbusConfig.modbusSensor2Addr)
        configData.modbusConfig.modbusSensor2Addr = (configData.modbusConfig.modbusSensor1Addr % 247) + 1;
#endif

	configData.outputConfig.output4mA = 0;
	configData.outputConfig.output4mA2 = 0;

    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        configData.outputConfig.output20mA = 2000;
        Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
    } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        configData.outputConfig.output20mA2 = 10000;
        Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
    }

#ifdef SENSOR_PH_EC
	// PH
	if (configData.calibrationConfig.PH4_Cal<=0 || configData.calibrationConfig.PH4_Cal>1400)
		configData.calibrationConfig.PH4_Cal=400;
	if (configData.calibrationConfig.PH4_Value<=0 || configData.calibrationConfig.PH4_Value>1400)
		configData.calibrationConfig.PH4_Value=400;

	if (configData.calibrationConfig.PH7_Cal<=0 || configData.calibrationConfig.PH7_Cal>1400)
		configData.calibrationConfig.PH7_Cal=700;
	if (configData.calibrationConfig.PH7_Value<=0 || configData.calibrationConfig.PH7_Value>1400)
		configData.calibrationConfig.PH7_Value=700;

	// ??????????
	if (configData.calibrationConfig.EC_Value<=0 || configData.calibrationConfig.EC_Value>20000)
		configData.calibrationConfig.EC_Value=0;

#endif


    if (configData.outputConfig.output4mA == 0xFFFFFFFF)
        FactoryReset();

    switch (configData.modbusConfig.baudrate) {
        case 0:
            Serial_Open(SERIAL_PORT2, 9600);
            Serial_Open(SERIAL_PORT3, 9600);
            break;
        case 1:
            Serial_Open(SERIAL_PORT2, 19200);
            Serial_Open(SERIAL_PORT3, 19200);
            break;
        case 2:
            Serial_Open(SERIAL_PORT2, 38400);
            Serial_Open(SERIAL_PORT3, 38400);
            break;
        case 3:
            Serial_Open(SERIAL_PORT2, 115200);
            Serial_Open(SERIAL_PORT3, 115200);
            break;
    }


    //  LCD INIT DISPLAY	
    //	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	
    //			DrawTextsize96(5, 5, TEXT96_CI, DRAW_IMAGE_ENABLE);	
    //	else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
    //			DrawTextsize96(5, 5, TEXT96_CI3, DRAW_IMAGE_ENABLE);

    //	DrawUnit(300, 5, UNIT_RX, DRAW_IMAGE_ENABLE);
    //	DrawUnit(335, 5, UNIT_TX, DRAW_IMAGE_ENABLE);
    //	TFT_Fill(0, 41, 480, 43, WHITE);	

    currentTime = Time_GetCalendarTime();
    state = STATE_MAIN;
}


char sd_detect=0;
void SD_Detect_Draw(void) {

    sd_dec_flag = SD_Detect();
    if (sd_dec_flag == 0x00) {
        DrawIcon(ICON_SDCARD, DRAW_IMAGE_DISABLE);
		sd_detect=0;
    } else if (sd_dec_flag == 0x01) {
		if (sd_detect==0) {
			sd_detect=1;
        	DrawIcon(ICON_SDCARD, DRAW_IMAGE_ENABLE);
		}
    }
}


//====================================
// STATE, LCD OUT
//====================================

void StateHandler(void) {
    SD_Detect_Draw();
//    Alarm_Icon_Draw();

    switch (state) {
        case STATE_MAIN:
            State_Main();
            break;
        case STATE_MENU:
            State_Menu();
            break;
        case STATE_CONFIG:
            State_Config();
            break;
        case STATE_CALIB:
            State_Calib();
            break;
        case STATE_TREND:
            State_Trend();
            break;
        case STATE_ALARM:
            State_Alarm();
            break;
        case STATE_DIAGNOSTIC:
            State_Diagnostic();
            break;

        case STATE_CONFIG_OUTPUT:
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
            	State_ConfigOutput();
			}
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            State_ConfigOutput2();
			}

            break;
        case STATE_CONFIG_COMM:
            State_ConfigComm();
            break;


        case STATE_CONFIG_COMM_ETHERNET:
            State_ConfigEthernet();
            break;


        case STATE_CONFIG_RELAY:
            State_ConfigRelay();
            break;
        case STATE_CONFIG_TIME:
            State_ConfigTime();
            break;
        case STATE_CONFIG_ADJUST:
            State_ConfigAdjust();
            break;
#ifndef SENSOR_PH_EC
        case STATE_CALIB_ZERO:
            State_CalibZero();
            break;
		// ???????
        case STATE_CALIB_MANUAL:
            State_CalibManual();
            break;
        case STATE_CALIB_TEMP:
            State_CalibTemp();
            break;
#else
        case STATE_CALIB_ZERO:
		    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
				// ???????
	            State_CalibBuff();
			}
			else 
				State_CalibBuff_EC();
            break;


		// 
        case STATE_CALIB_BUFF_PH4:
            State_CalibBuffPH4();	 
            break;
		// 
        case STATE_CALIB_BUFF_PH7:
            State_CalibBuffPH7();
            break;

		// ???????
        case STATE_CALIB_SPAN:
            State_CalibSpan();
            break;
        case STATE_CALIB_TEMP:
            State_CalibTemp();
            break;
#endif
        case STATE_CALIB_S2_CYCLE:
            State_CalibS2Cycle();
            break;
        case STATE_CALIB_LOG:
            State_CalibLog();
            break;

		// 	???? - ??????- ???? : ???????-????????
        case STATE_CONFIG_RELAY_RELAY1:
            State_ConfigRelayRelay1();
            break;
		// ???? - ??????- ?? : ??????, ???????
        case STATE_CONFIG_RELAY_RELAY2:
            State_ConfigRelayRelay2();
            break;

		// ???? - ??????- ?????? : ????????, ????????
        case STATE_CONFIG_RELAY_RELAY3:
            State_ConfigRelayRelay3();
            break;


		// ???? - ???? - ????
        case STATE_CONFIG_ADJUST_GRADIENT:
            State_ConfigAdjustGradient();
            break;
		// ???? - ???? - ???
        case STATE_CONFIG_ADJUST_OFFSET:
            State_ConfigAdjustOffset();
            break;
		// ???? - ???? - ????
        case STATE_CONFIG_ADJUST_FILTER:
            State_ConfigAdjustFilter();
            break;
		// ???? - ???? - ????
        case STATE_CONFIG_ADJUST_FACTORYRESET:
            State_ConfigAdjustFactoryReset();
            break;


		//  ???? - ??????- ???? - ???????
        case STATE_CONFIG_RELAY_RELAY1_AUTO_WASH:
            State_ConfigRelayRelay1AutoWash();
            break;
		//  ???? - ??????- ???? - ????????
        case STATE_CONFIG_RELAY_RELAY1_MAN_WASH:
            State_ConfigRelayRelay1manualWASH();
            break;

		//  ???? - ??????- ?? - ?????? 
        case STATE_CONFIG_RELAY_RELAY2_AUTO_ALARM:
            State_ConfigRelayRelay2AutoAlarm();
            break;
		//  ???? - ??????- ?? - ???????
        case STATE_CONFIG_RELAY_RELAY2_ALARM_OFF:
            State_ConfigRelayRelay2AlarmOFF();
            break;

        // ????-??????-??????-????????
        case STATE_CONFIG_RELAY_RELAY3_SET:
            State_ConfigRelayRelay3SetOP();
            break;

        // ????-??????-??????-????????
        case STATE_CONFIG_RELAY_RELAY3_MAN:
            State_ConfigRelayRelay3manualOP();
            break;
    }
}


//====================================
// STATE NAIN,
//====================================
unsigned char btn=0, btn1=0,btn2=0,btn3=0;

void State_Main(void) {
    switch (subState) {
        case 0:
			day_up=2; time_up=2;

            RedrawTitle_Main();
            RedrawViewArea();
            RedrawBottomArea();
            subState = 1;

            break;
        case 1:
            RedrawValue();
            subState = 2;
            break;
        case 2:

			if (flag10ms & FLAG10MS_BUTTON) {
				flag10ms &= ~FLAG10MS_BUTTON;
				btn = 	CheckButton();
				if (btn!=0) {btn1=btn2; btn2=btn3; btn3=btn;}
				switch (btn) {
					case BUTTON_MENU:
						subState = 3;
						cursor = 0;
						//      MoveSelectedState();
						ClearBottomArea();
						break;
					case BUTTON_ENTER:
						//RedrawTime();
						RedrawBottomArea();

						if (btn1==BUTTON_LEFT && btn2==BUTTON_LEFT)
						{
							if (currentData.holdState==0) currentData.holdState=1;
							else currentData.holdState=0;
						}

						break;
					default:
					//RedrawTime();
					RedrawBottomArea();
				}
			}
            break;

        case 3:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_BACK:
	                    subState = 0;
						break;

                    case BUTTON_ENTER:
	                    subState = 0;
	                    cursor = 0;
	                    MoveSelectedState();
						break;

                	case BUTTON_LEFT:
						if  (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
							 currentData.Device_Selector_Mode = SENSOR_2_MODE; 
						else currentData.Device_Selector_Mode = SENSOR_1_MODE; 

						RedrawBottomArea_CH2();
						break;
                	case BUTTON_RIGHT:
						if  (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
							 currentData.Device_Selector_Mode = SENSOR_2_MODE; 
						else currentData.Device_Selector_Mode = SENSOR_1_MODE; 

						RedrawBottomArea_CH2();
						break;
					default:
	                    //RedrawTime();
						RedrawBottomArea_CH2();
	        	}
			}
            break;
    }
}


//====================================

void display_menu_icon(void) {
    display_set_set(0);
    display_set_cal(0);
    display_set_trand(0);
    display_set_alarm(0);
    display_set_diag(0);

    if (cursor == 0) display_set_set(1);
    else if (cursor == 1) display_set_cal(1);
    else if (cursor == 2) display_set_trand(1);
    else if (cursor == 3) display_set_alarm(1);
    else if (cursor == 4) display_set_diag(1);
}
//====================================

void State_Menu(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:


            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                    case BUTTON_BACK:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_LEFT:
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
                        if (cursor == 0)
                            cursor = 4;
                        else
                            --cursor;
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
                        display_menu_icon();
                        break;
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
                        ++cursor;
                        cursor = cursor % 5;
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
                        display_menu_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        break;
                }
            }
            break;
        case 2:
            break;
    }
}


//===========================================================

void display_config_icon(void) {
    display_set2_analog(0);
    display_set2_calib(0);
    display_set2_comm(0);
    display_set2_relay(0);
    display_set2_time(0);

    if (cursor == 0) display_set2_analog(1);
    else if (cursor == 1) display_set2_comm(1);
    else if (cursor == 2) display_set2_relay(1);
    else if (cursor == 3) display_set2_time(1);
    else if (cursor == 4) display_set2_calib(1);
}

void State_Config(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
                        if (cursor == 0)
                            cursor = 4;
                        else
                            --cursor;
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
                        display_config_icon();
                        break;
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
                        ++cursor;
                        cursor = cursor % 5;
                        //DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
                        display_config_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}


//==========================================
#ifndef   SENSOR_PH_EC

void display_calib_icon(void) {
    display_set3_zero(0);
    display_set3_span(0);
    display_set3_temp(0);
    display_set3_log(0);

    if (cursor == 0) display_set3_zero(1);	  
    else if (cursor == 1) display_set3_span(1);
    else if (cursor == 2) display_set3_temp(1);
    else if (cursor == 3) display_set3_log(1);
}

#else

void display_calib_icon(void) {						  

	if (currentData.Device_Selector_Mode == SENSOR_1_MODE)    display_set3_buff(0);
	else     display_set3_zero(0);

    display_set3_span(0);
    display_set3_temp(0);
    display_set3_log(0);

    if (cursor == 0) {
		if (currentData.Device_Selector_Mode == SENSOR_1_MODE)    display_set3_buff(1);
		else     display_set3_zero(1);
	}
    else if (cursor == 1) display_set3_span(1);
    else if (cursor == 2) display_set3_temp(1);
    else if (cursor == 3) display_set3_log(1);
}

#endif


void State_Calib(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                cursorMax = 4;
                cursorWidth = 120;
            } else {
                cursorMax = 5;
                cursorWidth = 96;
            }
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        //DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, BLACK);
                        if (cursor == 0)
                            cursor = cursorMax - 1;
                        else
                            --cursor;

#ifndef    SENSOR_PH_EC
						if (currentData.Device_Selector_Mode == SENSOR_2_MODE && cursor == 2) cursor=1;
#endif
                        //DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, YELLOW);

                        display_calib_icon();
                        break;
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, BLACK);
                        ++cursor;
#ifndef    SENSOR_PH_EC
						if (currentData.Device_Selector_Mode == SENSOR_2_MODE && cursor == 2) cursor=3;
#endif
                        cursor = cursor % cursorMax;

                        //DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, YELLOW);
                        display_calib_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}

// trand_select_time
void Trend_Data_calc(void) {
    int i = 0;
    if (comm_run_flag != 1) {
        comm_run_flag = 7;
        for (i = 0; i < 60; i++) {
			if (currentData.Device_Selector_Mode == SENSOR_1_MODE)
            	trendValue[0][i] = trandData[trand_select_time][i];
			else if (currentData.Device_Selector_Mode == SENSOR_2_MODE)
            	trendValue[0][i] = trandData2[trand_select_time][i];
        }
        comm_run_flag = 0;
    }
} 

void State_Trend(void) {

#ifndef    SENSOR_PH_EC
	if (currentData.Device_Selector_Mode == SENSOR_1_MODE) trand_select_Y_NO=3;
#else 
	if (currentData.Device_Selector_Mode == SENSOR_2_MODE) trand_select_Y_NO=3;
#endif
	else trand_select_Y_NO=4;


    switch (subState) {
        case 0:
            Trend_Data_calc();
            RedrawTitle();
            RedrawBottomArea();
			ClearViewArea4();

            RedrawViewArea();
            subState = 1;

            cursor = 60;
            compareValue = 0;
            RedrawValue();
            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, RED);
            DrawTrandDiamond(cursor);
            DrawTrandText(cursor-1);

            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        if (cursor <= 1) {
                            ClearGraphArea();
                            cursor = 60;
                            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, RED);
                        } else {
                            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, LGRAY);
                            --cursor;
                            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, RED);
                        }
                        DrawTrandDiamond(cursor);
                        DrawTrandText(cursor-1);
                        break;
                    case BUTTON_RIGHT:
                        if (cursor == 60) {
                            ClearGraphArea();
                            cursor = 1;
                            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, RED);
                        } else {
                            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, LGRAY);
                            ++cursor;
                            TFT_Fill(GRAPH_X_START + (cursor * 6), GRAPH_Y_MAX, GRAPH_X_START + (cursor * 6) + 1, GRAPH_Y_ZERO, RED);
                        }
                        DrawTrandDiamond(cursor);
                        DrawTrandText(cursor-1);
                        break;

                    case BUTTON_UP:
						trand_select_Y++;
						if (trand_select_Y==trand_select_Y_NO) trand_select_Y=0;

                        ClearGraphArea();
			            Trend_Data_calc();

                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            compareValue ^= 1;
                            ClearGraphArea();
                            RedrawValue();
                        }
                        DrawTrandDiamond(cursor);
                        DrawTrandText(cursor);
                        break;

                    case BUTTON_DOWN:

						if (trand_select_Y==0) 	trand_select_Y=trand_select_Y_NO-1;
						else 					trand_select_Y--;

                        ClearGraphArea();
			            Trend_Data_calc();

                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            compareValue ^= 1;
                            ClearGraphArea();
                            RedrawValue();
                        }
                        DrawTrandDiamond(cursor);
                        DrawTrandText(cursor);
                        break;

                    case BUTTON_ENTER:
                        ClearGraphArea();
						trand_select_time++;
						if (trand_select_time>=trand_select_time_NO) trand_select_time=0;
			            Trend_Data_calc();

                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            compareValue ^= 1;
                            ClearGraphArea();
                            RedrawValue();
                        }
                        DrawTrandDiamond(cursor);
                        DrawTrandText(cursor);
                        break;
                }
            }
            break;
    }
}

void State_Alarm(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
			if (tempConfigData.alarmConfig.highLimit>100000 || tempConfigData.alarmConfig.highLimit<0) tempConfigData.alarmConfig.highLimit=10000;
			if (tempConfigData.alarmConfig.lowLimit >100000 || tempConfigData.alarmConfig.lowLimit <0) tempConfigData.alarmConfig.lowLimit =    0;
			if (tempConfigData.alarmConfig.zeroAlarm>100000 || tempConfigData.alarmConfig.zeroAlarm<0) tempConfigData.alarmConfig.zeroAlarm=    1;
			if (tempConfigData.alarmConfig.zeroAlarm2>100000 || tempConfigData.alarmConfig.zeroAlarm2<0) tempConfigData.alarmConfig.zeroAlarm2=    1;

            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawAlarmConfig(cursor, WHITE);
                        if (cursor == 0) cursor = 4;
                        else --cursor;
                        DrawAlarmConfig(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawAlarmConfig(cursor, WHITE);
                        ++cursor;
                        cursor = cursor % 5;
                        DrawAlarmConfig(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.alarmConfig.highLimit + 10 <= 9999)
                                        tempConfigData.alarmConfig.highLimit += 10;
                                    break;
                                case 1:
                                    if (tempConfigData.alarmConfig.highLimit < 9999)
                                        ++tempConfigData.alarmConfig.highLimit;
                                    break;
                                case 2:
                                    if (tempConfigData.alarmConfig.lowLimit + 10 <= 9999)
                                        tempConfigData.alarmConfig.lowLimit += 10;
                                    break;
                                case 3:
                                    if (tempConfigData.alarmConfig.lowLimit < 9999)
                                        ++tempConfigData.alarmConfig.lowLimit;
                                    break;
                                case 4:
                                    if (tempConfigData.alarmConfig.zeroAlarm < 999)
                                        ++tempConfigData.alarmConfig.zeroAlarm;
                                    break;
                            }
                        } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.alarmConfig.highLimit2 + 100 <= 99999)
                                        tempConfigData.alarmConfig.highLimit2 += 100;
                                    break;
                                case 1:
                                    if (tempConfigData.alarmConfig.highLimit2 < 99999)
                                        ++tempConfigData.alarmConfig.highLimit2;
                                    break;
                                case 2:
                                    if (tempConfigData.alarmConfig.lowLimit2 + 100 <= 99999)
                                        tempConfigData.alarmConfig.lowLimit2 += 100;
                                    break;
                                case 3:
                                    if (tempConfigData.alarmConfig.lowLimit2 < 99999)
                                        ++tempConfigData.alarmConfig.lowLimit2;
                                    break;
                                case 4:
                                    if (tempConfigData.alarmConfig.zeroAlarm2 < 999)
                                        ++tempConfigData.alarmConfig.zeroAlarm2;
                                    break;
                            }
                        }
                        DrawAlarmConfig(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.alarmConfig.highLimit >= 10)
                                        tempConfigData.alarmConfig.highLimit -= 10;
                                    break;
                                case 1:
                                    if (tempConfigData.alarmConfig.highLimit > 0)
                                        --tempConfigData.alarmConfig.highLimit;
                                    break;
                                case 2:
                                    if (tempConfigData.alarmConfig.lowLimit >= 10)
                                        tempConfigData.alarmConfig.lowLimit -= 10;
                                    break;
                                case 3:
                                    if (tempConfigData.alarmConfig.lowLimit > 0)
                                        --tempConfigData.alarmConfig.lowLimit;
                                    break;
                                case 4:
                                    if (tempConfigData.alarmConfig.zeroAlarm > 0)
                                        --tempConfigData.alarmConfig.zeroAlarm;
                                    break;
                            }
                        }
                        if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.alarmConfig.highLimit2 >= 100)
                                        tempConfigData.alarmConfig.highLimit2 -= 100;
                                    break;
                                case 1:
                                    if (tempConfigData.alarmConfig.highLimit2 > 0)
                                        --tempConfigData.alarmConfig.highLimit2;
                                    break;
                                case 2:
                                    if (tempConfigData.alarmConfig.lowLimit2 >= 100)
                                        tempConfigData.alarmConfig.lowLimit2 -= 100;
                                    break;
                                case 3:
                                    if (tempConfigData.alarmConfig.lowLimit2 > 0)
                                        --tempConfigData.alarmConfig.lowLimit2;
                                    break;
                                case 4:
                                    if (tempConfigData.alarmConfig.zeroAlarm2 > 0)
                                        --tempConfigData.alarmConfig.zeroAlarm2;
                                    break;
                            }
                        }
                        DrawAlarmConfig(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        if (configData.alarmConfig.highLimit != tempConfigData.alarmConfig.highLimit 
						 || configData.alarmConfig.lowLimit != tempConfigData.alarmConfig.lowLimit
                         || configData.alarmConfig.zeroAlarm != tempConfigData.alarmConfig.zeroAlarm

						 || configData.alarmConfig.highLimit2 != tempConfigData.alarmConfig.highLimit2 
						 || configData.alarmConfig.lowLimit2 != tempConfigData.alarmConfig.lowLimit2
                         || configData.alarmConfig.zeroAlarm2 != tempConfigData.alarmConfig.zeroAlarm2	) {

                            configData.alarmConfig = tempConfigData.alarmConfig;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

void State_Diagnostic(void) {
    switch (subState) {
        case 0:
			DrawBack2();
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;

			sd_detect=0;
            break;
        case 1:
            RedrawViewArea();

            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        break;
                    case BUTTON_RIGHT:
                        break;
                    case BUTTON_ENTER:
                        break;
                }
            }
            break;
    }
}

uint16_t output4ma_flag = 0;
uint16_t output20ma_flag = 0;
int32_t output_temp4 = 0;
int32_t output_temp20 = 0;


void adj_Current_Cal(void) {
    if (output4ma_flag == 1 && output20ma_flag == 0) {
        if (campare_4mA != tempConfigData.outputConfig.outputReal4mA) {
       //     configData.outputConfig.outputReal4mA = adjdefault4mA + tempConfigData.outputConfig.outputReal4mA;
        }
        TIM8_Chage_Duty_Channel(2, tempConfigData.outputConfig.outputReal4mA);
        campare_4mA = configData.outputConfig.outputReal4mA;
    } else if (output4ma_flag == 0 && output20ma_flag == 1) {
        if (campare_20mA != tempConfigData.outputConfig.outputReal20mA) {
         //   configData.outputConfig.outputReal20mA = adjdefault20mA + tempConfigData.outputConfig.outputReal20mA;
        }
        TIM8_Chage_Duty_Channel(2, tempConfigData.outputConfig.outputReal20mA);
        campare_20mA = configData.outputConfig.outputReal20mA;
    }
    else {
        campare_4mA = 0;
        campare_20mA = 0;
    }
}

void adj_Current_Cal2(void) {
    if (output4ma_flag == 1 && output20ma_flag == 0) {
        if (campare_4mA != tempConfigData.outputConfig.outputReal4mA2) {
       //     configData.outputConfig.outputReal4mA = adjdefault4mA + tempConfigData.outputConfig.outputReal4mA;
        }
        TIM8_Chage_Duty_Channel(3, tempConfigData.outputConfig.outputReal4mA2);
        campare_4mA = configData.outputConfig.outputReal4mA2;
    } else if (output4ma_flag == 0 && output20ma_flag == 1) {
        if (campare_20mA != tempConfigData.outputConfig.outputReal20mA2) {
         //   configData.outputConfig.outputReal20mA = adjdefault20mA + tempConfigData.outputConfig.outputReal20mA;
        }
        TIM8_Chage_Duty_Channel(3, tempConfigData.outputConfig.outputReal20mA2);
        campare_20mA = configData.outputConfig.outputReal20mA2;
    }
    else {
        campare_4mA = 0;
        campare_20mA = 0;
    }
}


uint16_t dac_imsi=0;

void DAC_output_imsi(uint16_t  data)
{

//    configData.outputConfig.output4mA = 0;
//    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
//        configData.outputConfig.output20mA = 2000;
//    } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
//        configData.outputConfig.output20mA = 10000;
//    }

//    configData.outputConfig.outputReal4mA =  DEFAULT_4MA;
//    configData.outputConfig.outputReal20mA = DEFAULT_20MA;


	dac_imsi = data;
//	DAC_output(1, dac_imsi);
TIM8_Chage_Duty_Channel(2, dac_imsi);

}

void DAC_output_imsi2(uint16_t  data) {
	TIM8_Chage_Duty_Channel(3, data);
}


// ANALOG
void State_ConfigOutput(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            output_temp4 = (int32_t) tempConfigData.outputConfig.outputReal4mA - (int32_t) adjdefault4mA;
            output_temp20 = (int32_t) tempConfigData.outputConfig.outputReal20mA - (int32_t) adjdefault20mA;
            if (output_temp4 < 0) {
                tempConfigData.outputConfig.outputReal4mA = 0;
            } else {
                tempConfigData.outputConfig.outputReal4mA = (uint32_t) output_temp4;
            }
            if (output_temp20 < 0) {
                tempConfigData.outputConfig.outputReal20mA = 0;
            } else {
                tempConfigData.outputConfig.outputReal20mA = (uint32_t) output_temp20;
            }
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 4;
            compareValue = currentData.holdState;
            break;
        case 1:
			if (cursor==4 || cursor==5) 		DAC_output_imsi(tempConfigData.outputConfig.outputReal4mA);
			else if (cursor==6 || cursor==7) 	DAC_output_imsi(tempConfigData.outputConfig.outputReal20mA);

            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawOutputConfig(cursor, WHITE);
                        if (cursor == 4) cursor = 7; //8;
                        else --cursor;
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawOutputConfig(cursor, WHITE);
                        ++cursor;
                        if (cursor > 7) cursor = 4; //cursor = cursor % 9;
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        switch (cursor) {
                            case 0:

                                break;
                            case 1:

                                break;
                            case 2:

                                break;
                            case 3:

                                break;
                            case 4:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA + 100 <= 99999)
                                    tempConfigData.outputConfig.outputReal4mA += 100;
                                break;
                            case 5:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA < 99999)
                                    ++tempConfigData.outputConfig.outputReal4mA;
                                break;
                            case 6:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA + 100 <= 99999)
                                    tempConfigData.outputConfig.outputReal20mA += 100;
                                break;
                            case 7:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA < 99999)
                                    ++tempConfigData.outputConfig.outputReal20mA;
                                break;
                                //				case 8:
                                //					compareValue ^= 1;
                                //				break;
                        }
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        switch (cursor) {
                            case 0:

                                break;
                            case 1:

                                break;
                            case 2:

                                break;
                            case 3:

                                break;
                            case 4:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA >= 100)
                                    tempConfigData.outputConfig.outputReal4mA -= 100;
                                break;
                            case 5:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA > 0)
                                    --tempConfigData.outputConfig.outputReal4mA;
                                break;
                            case 6:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA >= 100)
                                    tempConfigData.outputConfig.outputReal20mA -= 100;
                                break;
                            case 7:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA > 0)
                                    --tempConfigData.outputConfig.outputReal20mA;
                                break;
                            case 8:
								//compareValue ^= 1;	
                                compareValue=1; // ???1??????
                                break;
                        }
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        if (configData.outputConfig.output4mA     != tempConfigData.outputConfig.output4mA     || configData.outputConfig.output20mA     != tempConfigData.outputConfig.output20mA
                         || configData.outputConfig.outputReal4mA != tempConfigData.outputConfig.outputReal4mA || configData.outputConfig.outputReal20mA != tempConfigData.outputConfig.outputReal20mA) {


							configData = tempConfigData;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        if (configData.outputConfig.output4mA2     != tempConfigData.outputConfig.output4mA2     || configData.outputConfig.output20mA2     != tempConfigData.outputConfig.output20mA2
                         || configData.outputConfig.outputReal4mA2 != tempConfigData.outputConfig.outputReal4mA2 || configData.outputConfig.outputReal20mA2 != tempConfigData.outputConfig.outputReal20mA2) {


							configData = tempConfigData;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        currentData.holdState = compareValue;
                        subState = 0;
                        state = STATE_MAIN;
                        output4ma_flag = 0;
                        output20ma_flag = 0;
                        TIM8_Chage_Duty_Channel(2, 0);

                        break;
                }
                adj_Current_Cal();
            }
            break;
    }
}


void State_ConfigOutput2(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            output_temp4 = (int32_t) tempConfigData.outputConfig.outputReal4mA2 - (int32_t) adjdefault4mA;
            output_temp20 = (int32_t) tempConfigData.outputConfig.outputReal20mA2 - (int32_t) adjdefault20mA;
            if (output_temp4 < 0) {
                tempConfigData.outputConfig.outputReal4mA2 = 0;
            } else {
                tempConfigData.outputConfig.outputReal4mA2 = (uint32_t) output_temp4;
            }
            if (output_temp20 < 0) {
                tempConfigData.outputConfig.outputReal20mA2 = 0;
            } else {
                tempConfigData.outputConfig.outputReal20mA2 = (uint32_t) output_temp20;
            }
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 4;
            compareValue = currentData.holdState;
            break;
        case 1:
			if (cursor==4 || cursor==5) 		DAC_output_imsi2(tempConfigData.outputConfig.outputReal4mA2);
			else if (cursor==6 || cursor==7) 	DAC_output_imsi2(tempConfigData.outputConfig.outputReal20mA2);

            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawOutputConfig(cursor, WHITE);
                        if (cursor == 4) cursor = 7; //8;
                        else --cursor;
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawOutputConfig(cursor, WHITE);
                        ++cursor;
                        if (cursor > 7) cursor = 4; //cursor = cursor % 9;
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        switch (cursor) {
                            case 0:

                                break;
                            case 1:

                                break;
                            case 2:

                                break;
                            case 3:

                                break;
                            case 4:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA2 + 100 <= 99999)
                                    tempConfigData.outputConfig.outputReal4mA2 += 100;
                                break;
                            case 5:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA2 < 99999)
                                    ++tempConfigData.outputConfig.outputReal4mA2;
                                break;
                            case 6:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA2 + 100 <= 99999)
                                    tempConfigData.outputConfig.outputReal20mA2 += 100;
                                break;
                            case 7:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA2 < 99999)
                                    ++tempConfigData.outputConfig.outputReal20mA2;
                                break;
                                //				case 8:
                                //					compareValue ^= 1;
                                //				break;
                        }
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        switch (cursor) {
                            case 0:

                                break;
                            case 1:

                                break;
                            case 2:

                                break;
                            case 3:

                                break;
                            case 4:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA2 >= 100)
                                    tempConfigData.outputConfig.outputReal4mA2 -= 100;
                                break;
                            case 5:
                                output4ma_flag = 1;
                                output20ma_flag = 0;
                                if (tempConfigData.outputConfig.outputReal4mA2 > 0)
                                    --tempConfigData.outputConfig.outputReal4mA2;
                                break;
                            case 6:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA2 >= 100)
                                    tempConfigData.outputConfig.outputReal20mA2 -= 100;
                                break;
                            case 7:
                                output4ma_flag = 0;
                                output20ma_flag = 1;
                                if (tempConfigData.outputConfig.outputReal20mA2 > 0)
                                    --tempConfigData.outputConfig.outputReal20mA2;
                                break;
                            case 8:
								//compareValue ^= 1;	
                                compareValue=1; // ???1??????
                                break;
                        }
                        DrawOutputConfig(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        if (configData.outputConfig.output4mA     != tempConfigData.outputConfig.output4mA     || configData.outputConfig.output20mA     != tempConfigData.outputConfig.output20mA
                         || configData.outputConfig.outputReal4mA != tempConfigData.outputConfig.outputReal4mA || configData.outputConfig.outputReal20mA != tempConfigData.outputConfig.outputReal20mA) {


							configData = tempConfigData;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        if (configData.outputConfig.output4mA2     != tempConfigData.outputConfig.output4mA2     || configData.outputConfig.output20mA2     != tempConfigData.outputConfig.output20mA2
                         || configData.outputConfig.outputReal4mA2 != tempConfigData.outputConfig.outputReal4mA2 || configData.outputConfig.outputReal20mA2 != tempConfigData.outputConfig.outputReal20mA2) {


							configData = tempConfigData;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        currentData.holdState = compareValue;
                        subState = 0;
                        state = STATE_MAIN;
                        output4ma_flag = 0;
                        output20ma_flag = 0;
                        TIM8_Chage_Duty_Channel(3, 0);

                        break;
                }
                adj_Current_Cal2();
            }
            break;
    }
}


//======================================

void State_ConfigComm(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCommConfig(cursor, WHITE);
                        if (cursor == 0)
                            cursor = 3;
                        else
                            --cursor;
                        DrawCommConfig(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCommConfig(cursor, WHITE);
                        ++cursor;
                        cursor = cursor % 4;
                        DrawCommConfig(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        switch (cursor) {
                            case 0:
                                tempConfigData.modbusConfig.mode++;
								if (tempConfigData.modbusConfig.mode>2) tempConfigData.modbusConfig.mode=0;
                                break;
                            case 1:
                                ++tempConfigData.modbusConfig.baudrate;
                                tempConfigData.modbusConfig.baudrate = tempConfigData.modbusConfig.baudrate % 4;
                                break;
                            case 2:
                                ++tempConfigData.modbusConfig.modbusSlaveAddr;
                                tempConfigData.modbusConfig.modbusSlaveAddr = tempConfigData.modbusConfig.modbusSlaveAddr % 247;
                                break;
                            case 3:
                                tempConfigData.modbusConfig.databit ^= 1;
                                break;
                        }
                        DrawCommConfig(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        switch (cursor) {
                            case 0:
								if (tempConfigData.modbusConfig.mode==0) tempConfigData.modbusConfig.mode=2;
                                else 	tempConfigData.modbusConfig.mode--;
                                break;
                            case 1:
                                if (tempConfigData.modbusConfig.baudrate == 0)
                                    tempConfigData.modbusConfig.baudrate = 3;
                                else
                                    --tempConfigData.modbusConfig.baudrate;
                                break;
                            case 2:
                                if (tempConfigData.modbusConfig.modbusSlaveAddr == 0)
                                    tempConfigData.modbusConfig.modbusSlaveAddr = 246;
                                else
                                    --tempConfigData.modbusConfig.modbusSlaveAddr;
                                break;
                            case 3:
                                tempConfigData.modbusConfig.databit ^= 1;
                                break;
                        }
                        DrawCommConfig(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        if (configData.modbusConfig.mode != tempConfigData.modbusConfig.mode || configData.modbusConfig.baudrate != tempConfigData.modbusConfig.baudrate
                                || configData.modbusConfig.modbusSlaveAddr != tempConfigData.modbusConfig.modbusSlaveAddr || configData.modbusConfig.databit != tempConfigData.modbusConfig.databit
#ifdef SENSOR_PH_EC
                                || configData.modbusConfig.modbusSensor1Addr != tempConfigData.modbusConfig.modbusSensor1Addr
                                || configData.modbusConfig.modbusSensor2Addr != tempConfigData.modbusConfig.modbusSensor2Addr
#endif
                                ) {
                            configData.modbusConfig = tempConfigData.modbusConfig;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));

                            switch (configData.modbusConfig.baudrate) {
                                case 0:
                                    Serial_Open(SERIAL_PORT2, 9600);
                                    Serial_Open(SERIAL_PORT3, 9600);
                                    break;
                                case 1:
                                    Serial_Open(SERIAL_PORT2, 19200);
                                    Serial_Open(SERIAL_PORT3, 19200);
                                    break;
                                case 2:
                                    Serial_Open(SERIAL_PORT2, 38400);
                                    Serial_Open(SERIAL_PORT3, 38400);
                                    break;
                                case 3:
                                    Serial_Open(SERIAL_PORT2, 115200);
                                    Serial_Open(SERIAL_PORT3, 115200);
                                    break;
                            }
                        }
                        if (configData.modbusConfig.mode ==2)
						{
	                        subState = 0;
	                        state = STATE_CONFIG_COMM_ETHERNET;
	                        break;
						}
						else {
	                        subState = 0;
	                        state = STATE_MAIN;
	                        break;
						}
                }
            }
            break;
    }
}


//======================================


void State_ConfigEthernet(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;

            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCommEthernetConfig(cursor, WHITE);
                        if (cursor == 0)
                            cursor = 12;
                        else
                            --cursor;
                        DrawCommEthernetConfig(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCommEthernetConfig(cursor, WHITE);
                        ++cursor;
                        if (cursor>12) cursor=0;
                        DrawCommEthernetConfig(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        switch (cursor) {
                            case 0: tempConfigData.EthernetConfig.IP_ADDR0++;  break;
                            case 1: tempConfigData.EthernetConfig.IP_ADDR1++;	break;
                            case 2: tempConfigData.EthernetConfig.IP_ADDR2++;	break;
                            case 3: tempConfigData.EthernetConfig.IP_ADDR3++;	break;

                            case 4: tempConfigData.EthernetConfig.NETMASK_ADDR0++;	break;
                            case 5: tempConfigData.EthernetConfig.NETMASK_ADDR1++;	break;
                            case 6: tempConfigData.EthernetConfig.NETMASK_ADDR2++;	break;
                            case 7: tempConfigData.EthernetConfig.NETMASK_ADDR3++;	break;

                            case 8: tempConfigData.EthernetConfig.GW_ADDR0++;	break;
                            case 9: tempConfigData.EthernetConfig.GW_ADDR1++;	break;
                            case 10:tempConfigData.EthernetConfig.GW_ADDR2++;	break;
                            case 11:tempConfigData.EthernetConfig.GW_ADDR3++;	break;

                            case 12: tempConfigData.EthernetConfig.ETHERNET_PORT++;	break;
                        }
                        DrawCommEthernetConfig(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        switch (cursor) {
                            case 0: tempConfigData.EthernetConfig.IP_ADDR0--;  break;
                            case 1: tempConfigData.EthernetConfig.IP_ADDR1--;	break;
                            case 2: tempConfigData.EthernetConfig.IP_ADDR2--;	break;
                            case 3: tempConfigData.EthernetConfig.IP_ADDR3--;	break;

                            case 4: tempConfigData.EthernetConfig.NETMASK_ADDR0--;	break;
                            case 5: tempConfigData.EthernetConfig.NETMASK_ADDR1--;	break;
                            case 6: tempConfigData.EthernetConfig.NETMASK_ADDR2--;	break;
                            case 7: tempConfigData.EthernetConfig.NETMASK_ADDR3--;	break;

                            case 8: tempConfigData.EthernetConfig.GW_ADDR0--;	break;
                            case 9: tempConfigData.EthernetConfig.GW_ADDR1--;	break;
                            case 10:tempConfigData.EthernetConfig.GW_ADDR2--;	break;
                            case 11:tempConfigData.EthernetConfig.GW_ADDR3--;	break;

                            case 12: tempConfigData.EthernetConfig.ETHERNET_PORT--;	break;
                        }
                        DrawCommEthernetConfig(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:

						IP_ADDR0=tempConfigData.EthernetConfig.IP_ADDR0;
						IP_ADDR1=tempConfigData.EthernetConfig.IP_ADDR1;
						IP_ADDR2=tempConfigData.EthernetConfig.IP_ADDR2;
						IP_ADDR3=tempConfigData.EthernetConfig.IP_ADDR3;

						//NETMASK
						NETMASK_ADDR0 =  tempConfigData.EthernetConfig.NETMASK_ADDR0;
						NETMASK_ADDR1 =  tempConfigData.EthernetConfig.NETMASK_ADDR1;
						NETMASK_ADDR2 =  tempConfigData.EthernetConfig.NETMASK_ADDR2;
						NETMASK_ADDR3 =  tempConfigData.EthernetConfig.NETMASK_ADDR3;

						//Gateway Address
						GW_ADDR0 =  tempConfigData.EthernetConfig.GW_ADDR0;
						GW_ADDR1 =  tempConfigData.EthernetConfig.GW_ADDR1;
						GW_ADDR2 =  tempConfigData.EthernetConfig.GW_ADDR2;
						GW_ADDR3 =  tempConfigData.EthernetConfig.GW_ADDR3;

						ETHERNET_PORT=tempConfigData.EthernetConfig.ETHERNET_PORT;

					  	/* Initilaize the LwIP stack */
//					    lwip_init();

					    /* Configure the Network interface */
//					    Netif_Config();

					    /* tcp echo server Init */
//					    tcp_echoserver_init();

					    /* Notify user about the network interface config */
//					    User_notification(&gnetif);


                        configData.EthernetConfig = tempConfigData.EthernetConfig;
                        Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));	//***************

                        subState = 0;
                        state = STATE_MAIN;
                        break;

                }
            }
            break;
    }
}


//========================================

void display_relay_icon(void) {
    display_set5_clean(0);
    display_set5_alarm(0);
    display_set5_relay(0);

    if (cursor == 0) display_set5_clean(1);
    else if (cursor == 1) display_set5_alarm(1);
    else if (cursor == 2) display_set5_relay(1);
}

void State_ConfigRelay(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        //				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, BLACK);
                        if (cursor == 0) cursor = 2;
                        else --cursor;
                        //				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, YELLOW);
                        display_relay_icon();
                        break;
                    case BUTTON_RIGHT:
                        //				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, BLACK);
                        ++cursor;
                        cursor = cursor % 3;
                        //				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, YELLOW);
                        display_relay_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}

void State_ConfigTime(void) {
    switch (subState) {
        case 0:
            currentTime = Time_GetCalendarTime();
            tempConfigTime = currentTime;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawTimeConfig(cursor, WHITE);
                        if (cursor == 0) cursor = 5;
                        else --cursor;
                        DrawTimeConfig(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawTimeConfig(cursor, WHITE);
                        ++cursor;
                        cursor = cursor % 6;
                        DrawTimeConfig(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        switch (cursor) {
                            case 0:
                                if (tempConfigTime.tm_year < 2099)
                                    ++tempConfigTime.tm_year;
                                break;
                            case 1:
                                if (tempConfigTime.tm_mon == 11) tempConfigTime.tm_mon = 0;
                                else tempConfigTime.tm_mon++;
                                break;
                            case 2:
                                if (tempConfigTime.tm_mday == 31) tempConfigTime.tm_mday = 1;
                                else tempConfigTime.tm_mday++;
                                break;
                            case 3:
                                if (tempConfigTime.tm_hour == 23) tempConfigTime.tm_hour = 0;
                                else tempConfigTime.tm_hour++;
                                break;
                            case 4:
                                if (tempConfigTime.tm_min == 59) tempConfigTime.tm_min = 0;
                                else tempConfigTime.tm_min++;
                                break;
                            case 5:
                                if (tempConfigTime.tm_sec == 59) tempConfigTime.tm_sec = 0;
                                else tempConfigTime.tm_sec++;
                                break;
                        }
                        DrawTimeConfig(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        switch (cursor) {
                            case 0:
                                if (tempConfigTime.tm_year > 2000)
                                    --tempConfigTime.tm_year;
                                break;
                            case 1:
                                if (tempConfigTime.tm_mon == 0) tempConfigTime.tm_mon = 11;
                                else tempConfigTime.tm_mon--;
                                break;
                            case 2:
                                if (tempConfigTime.tm_mday == 1) tempConfigTime.tm_mday = 31;
                                else tempConfigTime.tm_mday--;
                                break;
                            case 3:
                                if (tempConfigTime.tm_hour == 0) tempConfigTime.tm_hour = 23;
                                else tempConfigTime.tm_hour--;
                                break;
                            case 4:
                                if (tempConfigTime.tm_min == 0) tempConfigTime.tm_min = 59;
                                else tempConfigTime.tm_min--;
                                break;
                            case 5:
                                if (tempConfigTime.tm_sec == 0) tempConfigTime.tm_sec = 59;
                                else tempConfigTime.tm_sec--;
                                break;
                        }
                        DrawTimeConfig(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        Time_SetCalendarTime(tempConfigTime);
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}


//==========================================

void display_ConfigAdjust_icon(void) {
    display_set4_slope(0);
    display_set4_offset(0);
    display_set4_filter(0);
    display_set4_init(0);

    if (cursor == 0) display_set4_slope(1);
    else if (cursor == 1) display_set4_offset(1);
    else if (cursor == 2) display_set4_filter(1);
    else if (cursor == 3) display_set4_init(1);
}


#ifdef DISABLE_SLOPE_OFFSET

void State_ConfigAdjust(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 2;


        	configData.adjustConfig.gradientS1 = 100;
            configData.adjustConfig.offsetS1   = 0;
            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));

            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
                        if (cursor == 2) cursor = 3;
                        else if (cursor == 3) cursor = 2;
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
                        display_ConfigAdjust_icon();
                        break;
/*                    case BUTTON_LEFT:
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
                        if (cursor == 0)
                            cursor = 3;
                        else
                            --cursor;
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
                        display_ConfigAdjust_icon();
                        break;
                    case BUTTON_RIGHT:
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
                        ++cursor;
                        cursor = cursor % 4;
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
                        display_ConfigAdjust_icon();
                        break;
						*/
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        break;
                }
            }
            break;
    }
}

#else

void State_ConfigAdjust(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
                        if (cursor == 0)
                            cursor = 3;
                        else
                            --cursor;
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
                        display_ConfigAdjust_icon();
                        break;
                    case BUTTON_RIGHT:
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
                        ++cursor;
                        cursor = cursor % 4;
                        //				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
                        display_ConfigAdjust_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        break;
                }
            }
            break;
    }
}
#endif


void Save_CalLog_Data(uint16_t Type, int32_t calvaule) {
    uint16_t address_start = 0;
    uint16_t upcounter = 0;

    LogDataCounter = LogData[110];
    currentTime = Time_GetCalendarTime();

    if (LogDataCounter >= 10) {
        LogDataCounter = 0;
        LogData[110] = LogDataCounter + 1;
        address_start = LogDataCounter;
    }

    if (LogDataCounter != 0) {
        LogData[110] = LogDataCounter + 1;
        upcounter = LogDataCounter;
        LogDataCounter = LogDataCounter * 10;
        LogDataCounter = LogDataCounter + upcounter;
        address_start = LogDataCounter;
    } else {
        LogData[110] = LogDataCounter + 1;
        address_start = LogDataCounter;

    }

    LogData[LogDataCounter++] = LogData[110] - 1;
    EEpromdata.WriteEEpromData_Buffer[0] = LogData[110] - 1;

    temp_year_mask = (currentTime.tm_year & 0x0000FF00) >> 8;
    LogData[LogDataCounter++] = (char) temp_year_mask;
    EEpromdata.WriteEEpromData_Buffer[1] = (char) temp_year_mask;

    temp_year_mask = (currentTime.tm_year & 0x000000FF);
    LogData[LogDataCounter++] = (char) temp_year_mask;
    EEpromdata.WriteEEpromData_Buffer[2] = (char) temp_year_mask;

    LogData[LogDataCounter++] = (char) currentTime.tm_mon;
    EEpromdata.WriteEEpromData_Buffer[3] = (char) currentTime.tm_mon;

    LogData[LogDataCounter++] = (char) currentTime.tm_mday;
    EEpromdata.WriteEEpromData_Buffer[4] = (char) currentTime.tm_mday;

    LogData[LogDataCounter++] = (char) currentTime.tm_hour;
    EEpromdata.WriteEEpromData_Buffer[5] = (char) currentTime.tm_hour;

    LogData[LogDataCounter++] = (char) currentTime.tm_min;
    EEpromdata.WriteEEpromData_Buffer[6] = (char) currentTime.tm_min;

    LogData[LogDataCounter++] = (char) currentTime.tm_sec;
    EEpromdata.WriteEEpromData_Buffer[7] = (char) currentTime.tm_sec;

    LogData[LogDataCounter++] = (char) Type;
    EEpromdata.WriteEEpromData_Buffer[8] = (char) Type;

    temp_vaule_mask = (calvaule & 0x000000000000FF00) >> 8;
    LogData[LogDataCounter++] = (char) temp_vaule_mask;
    EEpromdata.WriteEEpromData_Buffer[9] = (char) temp_vaule_mask;
    temp_vaule_mask = (calvaule & 0x00000000000000FF);
    LogData[LogDataCounter++] = (char) temp_vaule_mask;
    EEpromdata.WriteEEpromData_Buffer[10] = (char) temp_vaule_mask;

//    MpuToEEprom(EEpromdata.WriteEEpromData_Buffer, address_start, 11);

//    EEpromdata.WriteEEpromData_Buffer[0] = LogData[110];
//    MpuToEEprom(EEpromdata.WriteEEpromData_Buffer, 110, 2);
//    LogDataCounter = 0;


    EEpromdata.ReadEEpromData_Buffer[110] = LogData[110];
    EEpromdata.WriteEEpromData_Buffer[110] = LogData[110];
    MpuToEEprom(EEpromdata.WriteEEpromData_Buffer, address_start, 11);
    LogDataCounter = 0;

}


//????????
void State_CalibZero(void) {

    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCalibZero(cursor, WHITE);
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            if (cursor == 0) cursor = 3;
                            else --cursor;
                        } else cursor ^= 1;
                        DrawCalibZero(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCalibZero(cursor, WHITE);
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            ++cursor;
                            cursor = cursor % 4;
                        } else cursor ^= 1;
                        DrawCalibZero(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.S1zeroCal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.S1zeroCal += 10;
                            } else {
                                if (tempConfigData.calibrationConfig.S1zeroCal < 9999)
                                    ++tempConfigData.calibrationConfig.S1zeroCal;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.S2zeroCal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.S2zeroCal += 100;
                            } else {
                                if (tempConfigData.calibrationConfig.S2zeroCal < 9999)
                                    ++tempConfigData.calibrationConfig.S2zeroCal;
                            }
                        }
                        else {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S2zeroCal + 100 <= 9999)
                                        tempConfigData.calibrationConfig.S2zeroCal += 100;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S2zeroCal < 9999)
                                        ++tempConfigData.calibrationConfig.S2zeroCal;
                                    break;
                                case 2:
                                    if (tempConfigData.calibrationConfig.S2zeroCal + 100 <= 9999)
                                        tempConfigData.calibrationConfig.S2zeroCal += 100;
                                    break;
                                case 3:
                                    if (tempConfigData.calibrationConfig.S2zeroCal < 9999)
                                        ++tempConfigData.calibrationConfig.S2zeroCal;
                                    break;
                            }
                        }

                        DrawCalibZero(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:

                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.S1zeroCal >= 10)
                                    tempConfigData.calibrationConfig.S1zeroCal -= 10;
                                else if (tempConfigData.calibrationConfig.S1zeroCal < 0)
                                    tempConfigData.calibrationConfig.S1zeroCal += 10;  /* 음수일 때 0으로 수렴 */
                            } else {
                                if (tempConfigData.calibrationConfig.S1zeroCal > 0)
                                    --tempConfigData.calibrationConfig.S1zeroCal;
                                else if (tempConfigData.calibrationConfig.S1zeroCal < 0)
                                    ++tempConfigData.calibrationConfig.S1zeroCal;  /* 음수->0 */
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.S2zeroCal >= 100)
                                    tempConfigData.calibrationConfig.S2zeroCal -= 100;
                                else if (tempConfigData.calibrationConfig.S2zeroCal < 0)
                                    tempConfigData.calibrationConfig.S2zeroCal += 100;  /* 음수->0 */
                            } else {
                                if (tempConfigData.calibrationConfig.S2zeroCal > 0)
                                    --tempConfigData.calibrationConfig.S2zeroCal;
                                else if (tempConfigData.calibrationConfig.S2zeroCal < 0)
                                    ++tempConfigData.calibrationConfig.S2zeroCal;  /* 음수->0 */
                            }
                        }

                        DrawCalibZero(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        compareValue = 0;
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
                                Tx_S1ZeroCal = configData.calibrationConfig.S1zeroCal;
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.S1zeroCal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();
                                zero_cal_update_flag = 1;
                                Tx_S1ZeroCal = configData.calibrationConfig.S1zeroCal;
                                if (configData.adjustConfig.filterS1 != 0) {
                                    SendParmeter_Zero_Current = currentData.filterout_S1Current;
                                } else {
                                    SendParmeter_Zero_Current = currentData.S1mVolt;
                                }
                            }

                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
                                Tx_S2ZeroCal = configData.calibrationConfig.S2zeroCal;
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.S2zeroCal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();
                                zero_cal_update_flag = 1;
                                Tx_S2ZeroCal = configData.calibrationConfig.S2zeroCal;
                                zero_cal_update_flag_S2 = 1;

                            }
                        }
                        else {
                            zero_cal_update_flag = 0;
                            zero_cal_update_flag_S2 = 0;
                        }


                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}


// ?????? ??????? span
void State_CalibManual(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            DrawCalibManual(cursor, WHITE);
                            if (cursor == 0) cursor = 4;
                            else --cursor;
                            DrawCalibManual(cursor, YELLOW);
                        } else {
                            DrawCalibManual(cursor, WHITE);
                            if (cursor == 0) cursor = 1;
                            else --cursor;
                            DrawCalibManual(cursor, YELLOW);
                        }
                        break;
                    case BUTTON_RIGHT:
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            DrawCalibManual(cursor, WHITE);
                            ++cursor;
                            cursor = cursor % 5;
                            DrawCalibManual(cursor, YELLOW);
                        } else {
                            DrawCalibManual(cursor, WHITE);
                            ++cursor;
                            cursor = cursor % 2;
                            DrawCalibManual(cursor, YELLOW);
                        }
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S1manualCal + 10 <= 9999)
                                        tempConfigData.calibrationConfig.S1manualCal += 10;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S1manualCal < 9999)
                                        ++tempConfigData.calibrationConfig.S1manualCal;
                                    break;
                                case 2:

                                    break;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S2manualCal + 100 <= 99999)
                                        tempConfigData.calibrationConfig.S2manualCal += 100;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S2manualCal < 99999)
                                        ++tempConfigData.calibrationConfig.S2manualCal;
                                    break;
                                case 2:

                                    break;
                            }
                        }
                        DrawCalibManual(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S1manualCal >= 10)
                                        tempConfigData.calibrationConfig.S1manualCal -= 10;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S1manualCal > 0)
                                        --tempConfigData.calibrationConfig.S1manualCal;
                                    break;
                                case 2:

                                    break;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S2manualCal >= 100)
                                        tempConfigData.calibrationConfig.S2manualCal -= 100;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S2manualCal > 0)
                                        --tempConfigData.calibrationConfig.S2manualCal;
                                    break;
                                case 2:
                                    if (compareSignValue > -99999)
                                        --compareSignValue;
                                    break;
                            }
                        }
                        DrawCalibManual(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        compareValue = 0;
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            compareValue = 1;
                            if (compareValue) {

                                if (configData.adjustConfig.filterS1 != 0) {
                                    SendParmeter_Current = currentData.filterout_S1Current;
                                } else {
                                    SendParmeter_Current = currentData.S1mVolt;
                                }

                                Tx_S1manualCal = tempConfigData.calibrationConfig.S1manualCal;
                                manual_cal_flag = 1;
                                Save_CalLog_Data(TEXT96_MANUAL_CALIBRATION, Tx_S1manualCal);
                                Update_Cal_Logdata();
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                            compareValue = 1;
                            if (compareValue) {
                                Tx_S2manualCal = tempConfigData.calibrationConfig.S2manualCal;
                                manual_cal_flag = 1;
                                Save_CalLog_Data(TEXT96_MANUAL_CALIBRATION, Tx_S2manualCal);
                                Update_Cal_Logdata();
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        } else {
                        }

                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}


// PH,EC ??????? span
void State_CalibSpan(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            DrawCalibSpan(cursor, WHITE);
                            if (cursor == 0) cursor = 4;
                            else --cursor;
                            DrawCalibSpan(cursor, YELLOW);
                        } else {
                            DrawCalibSpan(cursor, WHITE);
                            if (cursor == 0) cursor = 1;
                            else --cursor;
                            DrawCalibSpan(cursor, YELLOW);
                        }
                        break;
                    case BUTTON_RIGHT:
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            DrawCalibSpan(cursor, WHITE);
                            ++cursor;
                            cursor = cursor % 5;
                            DrawCalibSpan(cursor, YELLOW);
                        } else {
                            DrawCalibSpan(cursor, WHITE);
                            ++cursor;
                            cursor = cursor % 2;
                            DrawCalibSpan(cursor, YELLOW);
                        }
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            switch (cursor) {
                                case 0:
                                    /* 자릿수 가중치: 0.0X 구간에서는 +1만 적용 (0.001->0.002 방지) */
                                    if (tempConfigData.calibrationConfig.PH_Span_Cal < 10) {
                                        if (tempConfigData.calibrationConfig.PH_Span_Cal < 9999)
                                            ++tempConfigData.calibrationConfig.PH_Span_Cal;
                                    } else if (tempConfigData.calibrationConfig.PH_Span_Cal + 10 <= 9999)
                                        tempConfigData.calibrationConfig.PH_Span_Cal += 10;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.PH_Span_Cal < 9999)
                                        ++tempConfigData.calibrationConfig.PH_Span_Cal;
                                    break;
                                case 2:

                                    break;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    /* 자릿수 가중치: 0.0X 구간에서는 +1만 적용 (0.001->0.011 튐 방지) */
                                    if (tempConfigData.calibrationConfig.EC_Span_Cal < 100) {
                                        if (tempConfigData.calibrationConfig.EC_Span_Cal < 99999)
                                            ++tempConfigData.calibrationConfig.EC_Span_Cal;
                                    } else if (tempConfigData.calibrationConfig.EC_Span_Cal + 100 <= 99999)
                                        tempConfigData.calibrationConfig.EC_Span_Cal += 100;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.EC_Span_Cal < 99999)
                                        ++tempConfigData.calibrationConfig.EC_Span_Cal;
                                    break;
                                case 2:

                                    break;
                            }
                        }
                        DrawCalibSpan(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.PH_Span_Cal < 10 && tempConfigData.calibrationConfig.PH_Span_Cal > 0)
                                        --tempConfigData.calibrationConfig.PH_Span_Cal;
                                    else if (tempConfigData.calibrationConfig.PH_Span_Cal >= 10)
                                        tempConfigData.calibrationConfig.PH_Span_Cal -= 10;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.PH_Span_Cal > 1)
                                        --tempConfigData.calibrationConfig.PH_Span_Cal;
                                    break;
                                case 2:

                                    break;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.EC_Span_Cal < 100 && tempConfigData.calibrationConfig.EC_Span_Cal > 0)
                                        --tempConfigData.calibrationConfig.EC_Span_Cal;
                                    else if (tempConfigData.calibrationConfig.EC_Span_Cal >= 100)
                                        tempConfigData.calibrationConfig.EC_Span_Cal -= 100;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.EC_Span_Cal > 0)
                                        --tempConfigData.calibrationConfig.EC_Span_Cal;
                                    break;
                                case 2:
                                    if (compareSignValue > -99999)
                                        --compareSignValue;
                                    break;
                            }
                        }
                        DrawCalibSpan(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        compareValue = 0;
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            compareValue = 1;
                            if (compareValue) {

                                if (configData.adjustConfig.filterS1 != 0) {
                                    SendParmeter_Current = currentData.filterout_S1Current;
                                } else {
                                    SendParmeter_Current = currentData.S1mVolt;
                                }
								tempConfigData.calibrationConfig.PH_Span_Value = currentData.S1PPM; 

                                Tx_S1manualCal = tempConfigData.calibrationConfig.PH_Span_Cal;
                                manual_cal_flag = 1;
                                Save_CalLog_Data(TEXT96_MANUAL_CALIBRATION, Tx_S1manualCal);
                                Update_Cal_Logdata();
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                            compareValue = 1;
                            if (compareValue) {
								tempConfigData.calibrationConfig.EC_Span_Value = data_EC; 

                                Tx_S2manualCal = tempConfigData.calibrationConfig.EC_Span_Cal;
                                manual_cal_flag = 1;
                                Save_CalLog_Data(TEXT96_MANUAL_CALIBRATION, Tx_S2manualCal);
                                Update_Cal_Logdata();
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        } else {
                        }

                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}


#ifndef  SENSOR_PH_EC

// ???? ???????
void State_CalibTemp(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            compareSignValue = currentData.temperature;
            compareSigntemp = compareSignValue;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            DrawCalibAuto(cursor, WHITE);
                            if (cursor == 0) cursor = 1;
                            else --cursor;
                            DrawCalibAuto(cursor, YELLOW);
                        } else {
                            DrawCalibAuto(cursor, WHITE);
                            if (cursor == 0) cursor = 2;
                            else --cursor;
                            DrawCalibAuto(cursor, YELLOW);
                        }
                        break;
                    case BUTTON_RIGHT:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            DrawCalibAuto(cursor, WHITE);
                            ++cursor;
                            cursor = cursor % 2;
                            DrawCalibAuto(cursor, YELLOW);
                        } else {
                            DrawCalibAuto(cursor, WHITE);
                            ++cursor;
                            cursor = cursor % 3;
                            DrawCalibAuto(cursor, YELLOW);
                        }
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S1Value + 10 <= 999)
                                        tempConfigData.calibrationConfig.S1Value += 10;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S1Value < 999)
                                        ++tempConfigData.calibrationConfig.S1Value;
                                    break;
                                case 2:
                                    if (tempConfigData.calibrationConfig.errorRange < 99)
                                        ++tempConfigData.calibrationConfig.errorRange;
                                    break;
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            switch (cursor) {
                                case 0:
                                    if (compareSignValue + 100 <= 9999)
                                        compareSignValue += 100;
                                    break;
                                case 1:
                                    if (compareSignValue < 9990)
                                        compareSignValue+=10;
                                    break;
                            }

                        }
                        DrawCalibAuto(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            switch (cursor) {
                                case 0:
                                    if (tempConfigData.calibrationConfig.S1Value >= 10)
                                        tempConfigData.calibrationConfig.S1Value -= 10;
                                    break;
                                case 1:
                                    if (tempConfigData.calibrationConfig.S1Value > 0)
                                        --tempConfigData.calibrationConfig.S1Value;
                                    break;
                                case 2:
                                    if (tempConfigData.calibrationConfig.errorRange > 0)
                                        --tempConfigData.calibrationConfig.errorRange;
                                    break;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            switch (cursor) {
                                case 0:
                                    if (compareSignValue >= 100)
                                        compareSignValue -= 100;
                                    break;
                                case 1:
                                    if (compareSignValue >= 10)
                                        compareSignValue-=10;
                                    break;
                            }
                        }
                        DrawCalibAuto(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:

                        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            configData.calibrationConfig = tempConfigData.calibrationConfig;

                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        } else if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            Tx_Adj_TempertureVaule = compareSignValue;
                            manual_cal_temp_flag = 1;
                            Save_CalLog_Data(TEXT96_TEMP_CALIBRATION, Tx_Adj_TempertureVaule);
                            Update_Cal_Logdata();
                        }
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

#else

// PH, EC ???????
void State_CalibTemp(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            compareSignValue = currentData.temperature;
            compareSigntemp = compareSignValue;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCalibTemp(cursor, WHITE);
                        if (cursor == 0) cursor = 1;
                        else --cursor;
                        DrawCalibTemp(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCalibTemp(cursor, WHITE);
                        ++cursor;
                        cursor = cursor % 2;
                        DrawCalibTemp(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
					   	if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	                        switch (cursor) {
	                            case 0:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal1 + 10 <= 9999)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal1 += 10;
	                                break;
	                            case 1:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal1 < 9990)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal1+=1;
	                                break;
							}
						}								
						else {
	                        switch (cursor) {
	                            case 0:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal2 + 10 <= 9999)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal2 += 10;
	                                break;
	                            case 1:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal2 < 9990)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal2+=1;
	                                break;
							}
	                    }
                        DrawCalibTemp(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
					   	if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	                        switch (cursor) {
	                            case 0:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal1 >= 10)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal1 -= 10;
	                                break;
	                            case 1:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal1 >= 1)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal1-=1;
	                                break;
	                        }
						}
						else {
	                        switch (cursor) {
	                            case 0:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal2 >= 10)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal2 -= 10;
	                                break;
	                            case 1:
	                                if (tempConfigData.calibrationConfig.TEMP_Span_Cal2 >= 1)
	                                    tempConfigData.calibrationConfig.TEMP_Span_Cal2-=1;
	                                break;
	                        }
						}
                        DrawCalibTemp(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:

					   	if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
							tempConfigData.calibrationConfig.TEMP_Span_Value1 = SET_data_TEMP;
							Tx_Adj_TempertureVaule = tempConfigData.calibrationConfig.TEMP_Span_Cal1*10;
						}
						else {
							tempConfigData.calibrationConfig.TEMP_Span_Value2 = SET_data_TEMP;
							Tx_Adj_TempertureVaule = tempConfigData.calibrationConfig.TEMP_Span_Cal2*10;
						}

                        configData.calibrationConfig = tempConfigData.calibrationConfig;
                        Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));

                        manual_cal_temp_flag = 1;
                        Save_CalLog_Data(TEXT96_TEMP_CALIBRATION, Tx_Adj_TempertureVaule);
                        Update_Cal_Logdata();

                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

#endif

void State_CalibS2Cycle(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        break;
                    case BUTTON_RIGHT:
                        break;
                    case BUTTON_UP:
                        if (tempConfigData.calibrationConfig.S2cycle < 999)
                            ++tempConfigData.calibrationConfig.S2cycle;

                        break;
                    case BUTTON_DOWN:
                        if (tempConfigData.calibrationConfig.S2cycle > 0)
                            --tempConfigData.calibrationConfig.S2cycle;

                        break;
                    case BUTTON_ENTER:
                        if (configData.calibrationConfig.S2cycle != tempConfigData.calibrationConfig.S2cycle) {
                            configData.calibrationConfig = tempConfigData.calibrationConfig;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}


// ??????? 
void State_CalibLog(void) {
    int year_temp = 0;
    int vaule_temp = 0;

    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;

            if (caldisp_counter == LogData[caldisp_counter * 11]) {
                if (caldisp_counter == 0) {
                    caldisp_array_counter = caldisp_counter * 10;
                }
                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;
                year_temp = LogData[caldisp_array_counter + 1];
                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                calibLog[0].calibrationTime.tm_year = year_temp;
                calibLog[0].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                calibLog[0].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                calibLog[0].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                calibLog[0].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                calibLog[0].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                calibLog[0].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                vaule_temp = LogData[caldisp_array_counter + 9];
                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                calibLog[0].calibrationValue = vaule_temp; // 1.000		

            }
            caldisp_counter++;
            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 2
            {
                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;

                year_temp = LogData[caldisp_array_counter + 1];
                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                calibLog[1].calibrationTime.tm_year = year_temp;
                calibLog[1].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                calibLog[1].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                calibLog[1].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                calibLog[1].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                calibLog[1].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                calibLog[1].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                vaule_temp = LogData[caldisp_array_counter + 9];
                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                calibLog[1].calibrationValue = vaule_temp; // 1.000

            }
            caldisp_counter++;
            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 2
            {
                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;

                year_temp = LogData[caldisp_array_counter + 1];
                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                calibLog[2].calibrationTime.tm_year = year_temp;
                calibLog[2].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                calibLog[2].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                calibLog[2].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                calibLog[2].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                calibLog[2].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                calibLog[2].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                vaule_temp = LogData[caldisp_array_counter + 9];
                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                calibLog[2].calibrationValue = vaule_temp; // 1.000

            }
            caldisp_counter++;
            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 2
            {
                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;
                year_temp = LogData[caldisp_array_counter + 1];
                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                calibLog[3].calibrationTime.tm_year = year_temp;
                calibLog[3].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                calibLog[3].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                calibLog[3].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                calibLog[3].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                calibLog[3].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                calibLog[3].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                vaule_temp = LogData[caldisp_array_counter + 9];
                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                calibLog[3].calibrationValue = vaule_temp; // 1.000

            }

            RedrawValue();
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        break;
                    case BUTTON_RIGHT:
                        break;
                    case BUTTON_UP:
                        if (cursor > 0) {
                            --cursor;
                            caldisp_counter--;
                            caldisp_counter = caldisp_counter - 3;	// ???4???? -3????
                            if (caldisp_counter < 0) {
                                caldisp_counter = 0;
                            }


                            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 1
                            {
                                if (caldisp_counter == 0) {
                                    caldisp_array_counter = caldisp_counter * 10;
                                }
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter; //
                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[0].calibrationTime.tm_year = year_temp;
                                calibLog[0].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[0].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[0].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[0].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[0].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[0].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[0].calibrationValue = vaule_temp; // 1.000

                            }
                            else {
                                calibLog[0].calibrationTime.tm_year = 0;
                                calibLog[0].calibrationTime.tm_mon = 0;
                                calibLog[0].calibrationTime.tm_mday = 0;
                                calibLog[0].calibrationTime.tm_hour = 0;
                                calibLog[0].calibrationTime.tm_min = 0;
                                calibLog[0].calibrationTime.tm_sec = 0;
                                calibLog[0].calibrationMethod = 0;
                                calibLog[0].calibrationValue = 0;
                            }
                            caldisp_counter++;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) {
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;

                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[1].calibrationTime.tm_year = year_temp;
                                calibLog[1].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[1].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[1].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[1].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[1].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[1].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[1].calibrationValue = vaule_temp; // 1.000
                                // caldisp_counter++;
                            }
                            else {
                                calibLog[1].calibrationTime.tm_year = 0;
                                calibLog[1].calibrationTime.tm_mon = 0;
                                calibLog[1].calibrationTime.tm_mday = 0;
                                calibLog[1].calibrationTime.tm_hour = 0;
                                calibLog[1].calibrationTime.tm_min = 0;
                                calibLog[1].calibrationTime.tm_sec = 0;
                                calibLog[1].calibrationMethod = 0;
                                calibLog[1].calibrationValue = 0;
                            }
                            caldisp_counter++;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) {
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;

                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[2].calibrationTime.tm_year = year_temp;
                                calibLog[2].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[2].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[2].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[2].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[2].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[2].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[2].calibrationValue = vaule_temp; // 1.000

                            }
                            else {
                                calibLog[2].calibrationTime.tm_year = 0;
                                calibLog[2].calibrationTime.tm_mon = 0;
                                calibLog[2].calibrationTime.tm_mday = 0;
                                calibLog[2].calibrationTime.tm_hour = 0;
                                calibLog[2].calibrationTime.tm_min = 0;
                                calibLog[2].calibrationTime.tm_sec = 0;
                                calibLog[2].calibrationMethod = 0;
                                calibLog[2].calibrationValue = 0;
                            }
                            caldisp_counter++;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) {
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;
                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[3].calibrationTime.tm_year = year_temp;
                                calibLog[3].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[3].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[3].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[3].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[3].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[3].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[3].calibrationValue = vaule_temp; // 1.000

                            }
                            else {
                                calibLog[3].calibrationTime.tm_year = 0;
                                calibLog[3].calibrationTime.tm_mon = 0;
                                calibLog[3].calibrationTime.tm_mday = 0;
                                calibLog[3].calibrationTime.tm_hour = 0;
                                calibLog[3].calibrationTime.tm_min = 0;
                                calibLog[3].calibrationTime.tm_sec = 0;
                                calibLog[3].calibrationMethod = 0;
                                calibLog[3].calibrationValue = 0;
                            }

                            RedrawValue();
                        }
                        break;
                    case BUTTON_DOWN:
                        if (cursor < 95) {
                            caldisp_counter++;
                            caldisp_counter = caldisp_counter - 3;	// ??? 4???? -3????

                            ++cursor;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 1
                            {
                                if (caldisp_counter == 0) {
                                    caldisp_array_counter = caldisp_counter * 10;
                                }
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter; //
                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[0].calibrationTime.tm_year = year_temp;
                                calibLog[0].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[0].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[0].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[0].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[0].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[0].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[0].calibrationValue = vaule_temp; // 1.000

                            }
                            else {
                                calibLog[0].calibrationTime.tm_year = 0;
                                calibLog[0].calibrationTime.tm_mon = 0;
                                calibLog[0].calibrationTime.tm_mday = 0;
                                calibLog[0].calibrationTime.tm_hour = 0;
                                calibLog[0].calibrationTime.tm_min = 0;
                                calibLog[0].calibrationTime.tm_sec = 0;
                                calibLog[0].calibrationMethod = 0;
                                calibLog[0].calibrationValue = 0;
                            }
                            caldisp_counter++;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) {
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;

                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[1].calibrationTime.tm_year = year_temp;
                                calibLog[1].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[1].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[1].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[1].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[1].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[1].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[1].calibrationValue = vaule_temp; // 1.000

                            }
                            else {
                                calibLog[1].calibrationTime.tm_year = 0;
                                calibLog[1].calibrationTime.tm_mon = 0;
                                calibLog[1].calibrationTime.tm_mday = 0;
                                calibLog[1].calibrationTime.tm_hour = 0;
                                calibLog[1].calibrationTime.tm_min = 0;
                                calibLog[1].calibrationTime.tm_sec = 0;
                                calibLog[1].calibrationMethod = 0;
                                calibLog[1].calibrationValue = 0;
                            }
                            caldisp_counter++;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 2
                            {
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;

                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[2].calibrationTime.tm_year = year_temp;
                                calibLog[2].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[2].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[2].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[2].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[2].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[2].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[2].calibrationValue = vaule_temp; // 1.000

                            }
                            else {
                                calibLog[2].calibrationTime.tm_year = 0;
                                calibLog[2].calibrationTime.tm_mon = 0;
                                calibLog[2].calibrationTime.tm_mday = 0;
                                calibLog[2].calibrationTime.tm_hour = 0;
                                calibLog[2].calibrationTime.tm_min = 0;
                                calibLog[2].calibrationTime.tm_sec = 0;
                                calibLog[2].calibrationMethod = 0;
                                calibLog[2].calibrationValue = 0;
                            }
                            caldisp_counter++;
                            if (caldisp_counter == LogData[caldisp_counter * 11]) // cursor + 2
                            {
                                caldisp_array_counter = (caldisp_counter * 10) + caldisp_counter;
                                year_temp = LogData[caldisp_array_counter + 1];
                                year_temp = (year_temp << 8) + LogData[caldisp_array_counter + 2];
                                calibLog[3].calibrationTime.tm_year = year_temp;
                                calibLog[3].calibrationTime.tm_mon = (int) LogData[caldisp_array_counter + 3];
                                calibLog[3].calibrationTime.tm_mday = (int) LogData[caldisp_array_counter + 4];
                                calibLog[3].calibrationTime.tm_hour = (int) LogData[caldisp_array_counter + 5];
                                calibLog[3].calibrationTime.tm_min = (int) LogData[caldisp_array_counter + 6];
                                calibLog[3].calibrationTime.tm_sec = (int) LogData[caldisp_array_counter + 7];

                                calibLog[3].calibrationMethod = (int) LogData[caldisp_array_counter + 8];
                                vaule_temp = LogData[caldisp_array_counter + 9];
                                vaule_temp = (vaule_temp << 8) + LogData[caldisp_array_counter + 10];
                                calibLog[3].calibrationValue = vaule_temp; // 1.000

                            } else {
                                calibLog[3].calibrationTime.tm_year = 0;
                                calibLog[3].calibrationTime.tm_mon = 0;
                                calibLog[3].calibrationTime.tm_mday = 0;
                                calibLog[3].calibrationTime.tm_hour = 0;
                                calibLog[3].calibrationTime.tm_min = 0;
                                calibLog[3].calibrationTime.tm_sec = 0;
                                calibLog[3].calibrationMethod = 0;
                                calibLog[3].calibrationValue = 0;
                            }
                            RedrawValue();
                        }
                        break;
                    case BUTTON_ENTER:
                        break;
                }
            }
            break;
    }
}


//==========================================

//=========================================
// ??????? ph4

//uint16_t buffer_calib_value=0;

void State_CalibBuffPH4(void) {


    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 1;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCalibPH4(cursor, WHITE);
                        //if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                        //    if (cursor == 0) cursor = 3;
                        //    else --cursor;
                        //} else cursor ^= 1;
                        DrawCalibPH4(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCalibPH4(cursor, WHITE);
                        //if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                        //    ++cursor;
                        //    cursor = cursor % 4;
                        //} else cursor ^= 1;
                        DrawCalibPH4(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.PH4_Cal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.PH4_Cal += 10;
                            } else {
                                if (tempConfigData.calibrationConfig.PH4_Cal < 9999)
                                    ++tempConfigData.calibrationConfig.PH4_Cal;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.EC_Cal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.EC_Cal += 100;
                            } else {
                                if (tempConfigData.calibrationConfig.EC_Cal < 9999)
                                    ++tempConfigData.calibrationConfig.EC_Cal;
                            }
                        }

                        DrawCalibPH4(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:

                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.PH4_Cal >= 10) {
                                    tempConfigData.calibrationConfig.PH4_Cal -= 10;
                                } 
                            } else {
                                if (tempConfigData.calibrationConfig.PH4_Cal > 0)
                                    --tempConfigData.calibrationConfig.PH4_Cal;
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.EC_Cal >= 100)
                                    tempConfigData.calibrationConfig.EC_Cal -= 100;
                                else if (tempConfigData.calibrationConfig.EC_Cal < 0)
                                    tempConfigData.calibrationConfig.EC_Cal += 100;  /* 음수->0 */
                            } else {
                                if (tempConfigData.calibrationConfig.EC_Cal > 0)
                                    --tempConfigData.calibrationConfig.EC_Cal;
                                else if (tempConfigData.calibrationConfig.EC_Cal < 0)
                                    ++tempConfigData.calibrationConfig.EC_Cal;  /* 음수->0 */
                            }
                        }

                        DrawCalibPH4(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        compareValue = 0;
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
                                Tx_S1ZeroCal = configData.calibrationConfig.PH4_Cal;

								tempConfigData.calibrationConfig.PH4_Value = data_pH;	// ***************
                                configData.calibrationConfig = tempConfigData.calibrationConfig;

                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.PH4_Cal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();

                                zero_cal_update_flag = 1;
                                Tx_S1ZeroCal = configData.calibrationConfig.PH4_Cal;
                                if (configData.adjustConfig.filterS1 != 0) {
                                    SendParmeter_Zero_Current = currentData.filterout_S1Current;
                                } else {
                                    SendParmeter_Zero_Current = currentData.S1mVolt;
                                }
                            }

                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
								tempConfigData.calibrationConfig.EC_Value = data_EC;	// ***************

                                Tx_S2ZeroCal = configData.calibrationConfig.EC_Cal;
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.EC_Cal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();
                                zero_cal_update_flag = 1;
                                Tx_S2ZeroCal = configData.calibrationConfig.S2zeroCal;
                                zero_cal_update_flag_S2 = 1;

                            }
                        }
                        else {
                            zero_cal_update_flag = 0;
                            zero_cal_update_flag_S2 = 0;
                        }


                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;


    }
}

//=========================================
// ??????? ph7

//uint16_t buffer_calib_value=0;

void State_CalibBuffPH7(void) {


    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 1;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCalibPH7(cursor, WHITE);
                        //if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                        //    if (cursor == 0) cursor = 3;
                        //    else --cursor;
                        //} else cursor ^= 1;
                        DrawCalibPH7(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCalibPH7(cursor, WHITE);
                        //if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                        //    ++cursor;
                        //    cursor = cursor % 4;
                        //} else cursor ^= 1;
                        DrawCalibPH7(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.PH7_Cal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.PH7_Cal += 10;
                            } else {
                                if (tempConfigData.calibrationConfig.PH7_Cal < 9999)
                                    ++tempConfigData.calibrationConfig.PH7_Cal;
                            }
                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.EC_Cal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.EC_Cal += 100;
                            } else {
                                if (tempConfigData.calibrationConfig.EC_Cal < 9999)
                                    ++tempConfigData.calibrationConfig.EC_Cal;
                            }
                        }

                        DrawCalibPH7(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:

                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.PH7_Cal >= 10) {
                                    tempConfigData.calibrationConfig.PH7_Cal -= 10;
                                } 
                            } else {
                                if (tempConfigData.calibrationConfig.PH7_Cal > 0)
                                    --tempConfigData.calibrationConfig.PH7_Cal;
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.EC_Cal >= 100)
                                    tempConfigData.calibrationConfig.EC_Cal -= 100;
                                else if (tempConfigData.calibrationConfig.EC_Cal < 0)
                                    tempConfigData.calibrationConfig.EC_Cal += 100;  /* 음수->0 */
                            } else {
                                if (tempConfigData.calibrationConfig.EC_Cal > 0)
                                    --tempConfigData.calibrationConfig.EC_Cal;
                                else if (tempConfigData.calibrationConfig.EC_Cal < 0)
                                    ++tempConfigData.calibrationConfig.EC_Cal;  /* 음수->0 */
                            }
                        }

                        DrawCalibPH7(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        compareValue = 0;
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
                                Tx_S1ZeroCal = configData.calibrationConfig.PH7_Cal;

								tempConfigData.calibrationConfig.PH7_Value = data_pH;	// ***************
                                configData.calibrationConfig = tempConfigData.calibrationConfig;

                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.PH7_Cal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();

                                zero_cal_update_flag = 1;
                                Tx_S1ZeroCal = configData.calibrationConfig.PH7_Cal;
                                if (configData.adjustConfig.filterS1 != 0) {
                                    SendParmeter_Zero_Current = currentData.filterout_S1Current;
                                } else {
                                    SendParmeter_Zero_Current = currentData.S1mVolt;
                                }
                            }

                        } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
                                Tx_S2ZeroCal = configData.calibrationConfig.EC_Cal;
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.EC_Cal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();
                                zero_cal_update_flag = 1;
                                Tx_S2ZeroCal = configData.calibrationConfig.EC_Cal;
                                zero_cal_update_flag_S2 = 1;

                            }
                        }
                        else {
                            zero_cal_update_flag = 0;
                            zero_cal_update_flag_S2 = 0;
                        }


                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;


    }
}


//========================
void State_CalibBuff_EC(void) {


    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 1;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawCalib_EC(cursor, WHITE);
                        //if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                        //    if (cursor == 0) cursor = 3;
                        //    else --cursor;
                        //} else cursor ^= 1;
                        DrawCalib_EC(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawCalib_EC(cursor, WHITE);
                        //if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                        //    ++cursor;
                        //    cursor = cursor % 4;
                        //} else cursor ^= 1;
                        DrawCalib_EC(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.EC_Cal + 100 <= 9999)
                                    tempConfigData.calibrationConfig.EC_Cal += 100;
                            } else {
                                if (tempConfigData.calibrationConfig.EC_Cal < 9999)
                                    ++tempConfigData.calibrationConfig.EC_Cal;
                            }
                        }

                        DrawCalib_EC(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:

                        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.calibrationConfig.EC_Cal >= 100)
                                    tempConfigData.calibrationConfig.EC_Cal -= 100;
                                else if (tempConfigData.calibrationConfig.EC_Cal < 0)
                                    tempConfigData.calibrationConfig.EC_Cal += 100;  /* 음수->0 */
                            } else {
                                if (tempConfigData.calibrationConfig.EC_Cal > 0)
                                    --tempConfigData.calibrationConfig.EC_Cal;
                                else if (tempConfigData.calibrationConfig.EC_Cal < 0)
                                    ++tempConfigData.calibrationConfig.EC_Cal;  /* 음수->0 */
                            }
                        }

                        DrawCalib_EC(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        compareValue = 0;
                        if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                            compareValue = 1;
                            if (compareValue == 1) {
								tempConfigData.calibrationConfig.EC_Value = data_EC;	// ***************

                                Tx_S2ZeroCal = configData.calibrationConfig.EC_Cal;
                                configData.calibrationConfig = tempConfigData.calibrationConfig;
                                Save_CalLog_Data(TEXT96_ZERO_CALIBRATION, configData.calibrationConfig.EC_Cal);
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                                Update_Cal_Logdata();
                                zero_cal_update_flag = 1;
                                Tx_S2ZeroCal = configData.calibrationConfig.S2zeroCal;
                                zero_cal_update_flag_S2 = 1;

                            }
                        }
                        else {
                            zero_cal_update_flag = 0;
                            zero_cal_update_flag_S2 = 0;
                        }


                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;


    }
}


//==========================================

void display_Buff_icon(void) {
    display_Calib_Buff_ph4(0);
    display_Calib_Buff_ph7(0);

    if (cursor == 0) display_Calib_Buff_ph4(1);
    else if (cursor == 1) display_Calib_Buff_ph7(1);
}

void State_CalibBuff(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();

            subState = 1;
            cursor = 0;

			display_Buff_icon();

            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
                        cursor ^= 1;
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
                        display_Buff_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}

//==========================================

//==========================================

// ???????-????????
void display_RelayRelay1_icon(void) {
    display_set7_autowash(0);
    display_set7_manwash(0);

    if (cursor == 0) display_set7_autowash(1);
    else if (cursor == 1) display_set7_manwash(1);
}

void State_ConfigRelayRelay1(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
                        cursor ^= 1;
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
                        display_RelayRelay1_icon();
                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}

//===========================================

void display_RelayRelay2_icon(void) {
    if (cursor == 0) {
        display_message3();
        display_set8_autoalarm(1);
        display_set8_noalarm(0);
    } else if (cursor == 1) {
        display_message4();
        display_set8_autoalarm(0);
        display_set8_noalarm(1);
    }
}


// ??-??????, ???????

void State_ConfigRelayRelay2(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
                        cursor ^= 1;
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
                        display_RelayRelay2_icon();

                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}


//==============================================

void display_RelayRelay3_icon(void) {
    display_set6_set(0);
    display_set6_passive(0);

    if (cursor == 0) display_set6_set(1);
    else if (cursor == 1) display_set6_passive(1);
}

void State_ConfigRelayRelay3(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
                        cursor ^= 1;
                        //DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
                        display_RelayRelay3_icon();

                        break;
                    case BUTTON_ENTER:
                        subState = 0;
                        MoveSelectedState();
                        cursor = 0;
                        break;
                }
            }
            break;
    }
}

uint16_t adjust_vauleChage_flag = 0;

void State_ConfigAdjustGradient(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            compareValue = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	                        tempConfigData.adjustConfig.gradientS1 +=100;
    	                    if (tempConfigData.adjustConfig.gradientS1 > 999) {
        	                    tempConfigData.adjustConfig.gradientS1 = 999;
            	            }
						}
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	                        tempConfigData.adjustConfig.gradientS2 +=100;
    	                    if (tempConfigData.adjustConfig.gradientS2 > 999) {
        	                    tempConfigData.adjustConfig.gradientS2 = 999;
            	            }
						}
                        DrawAdjustGradient(0, 0, YELLOW);
						break;
                    case BUTTON_RIGHT:
//                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
//                            DrawAdjustGradient(cursor, compareValue, WHITE);
//                            cursor ^= 1;
//                            DrawAdjustGradient(cursor, compareValue, YELLOW);
//                        }
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	                        tempConfigData.adjustConfig.gradientS1 -=100;
	                        if (tempConfigData.adjustConfig.gradientS1 < 0) {
	                            tempConfigData.adjustConfig.gradientS1 = 0;
	                        }
						}
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	                        tempConfigData.adjustConfig.gradientS2 -=100;
	                        if (tempConfigData.adjustConfig.gradientS2 < 0) {
	                            tempConfigData.adjustConfig.gradientS2 = 0;
	                        }
						}
                        DrawAdjustGradient(0, 0, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (tempConfigData.adjustConfig.gradientS1 < 999) {
                                ++tempConfigData.adjustConfig.gradientS1;
                                DrawAdjustGradient(0, 0, YELLOW);
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (tempConfigData.adjustConfig.gradientS2 < 999) {
                                ++tempConfigData.adjustConfig.gradientS2;
                                DrawAdjustGradient(0, 0, YELLOW);
                            }
                        }
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (tempConfigData.adjustConfig.gradientS1 > 0)
                                --tempConfigData.adjustConfig.gradientS1;
                            DrawAdjustGradient(0, 0, YELLOW);
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (tempConfigData.adjustConfig.gradientS2 > 0)
                                --tempConfigData.adjustConfig.gradientS2;
                            DrawAdjustGradient(0, 0, YELLOW);
                        }

                        break;
                    case BUTTON_ENTER:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (configData.adjustConfig.gradientS1 != tempConfigData.adjustConfig.gradientS1) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (configData.adjustConfig.gradientS2 != tempConfigData.adjustConfig.gradientS2) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        }
                        else {
                        }
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

void State_ConfigAdjustOffset(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            compareValue = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	                        tempConfigData.adjustConfig.offsetS1 +=100;
	                        if (tempConfigData.adjustConfig.offsetS1 > 999) {
	                            tempConfigData.adjustConfig.offsetS1 = 999;
	                        }
	                        DrawAdjustOffset(0, 0, YELLOW);
						}
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	                        tempConfigData.adjustConfig.offsetS2 +=100;
	                        if (tempConfigData.adjustConfig.offsetS2 > 999) {
	                            tempConfigData.adjustConfig.offsetS2 = 999;
	                        }
	                        DrawAdjustOffset(0, 0, YELLOW);
						}
						break;
                    case BUTTON_RIGHT:
//                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
//                            DrawAdjustOffset(cursor, compareValue, WHITE);
//                            cursor ^= 1;
//                            DrawAdjustOffset(cursor, compareValue, YELLOW);
//                        }
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	                        tempConfigData.adjustConfig.offsetS1 -=100;
	                        if (tempConfigData.adjustConfig.offsetS1 < -999) {
	                            tempConfigData.adjustConfig.offsetS1 = -999;
	                        }
	                        DrawAdjustOffset(0, 0, YELLOW);
						}
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	                        tempConfigData.adjustConfig.offsetS2 -=100;
	                        if (tempConfigData.adjustConfig.offsetS2 < -999) {
	                            tempConfigData.adjustConfig.offsetS2 = -999;
	                        }
	                        DrawAdjustOffset(0, 0, YELLOW);
						}
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (tempConfigData.adjustConfig.offsetS1 < 999) {
                                tempConfigData.adjustConfig.offsetS1++;
                                DrawAdjustOffset(0, 0, YELLOW);
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (tempConfigData.adjustConfig.offsetS2 < 999) {
                                tempConfigData.adjustConfig.offsetS2++;
                                DrawAdjustOffset(0, 0, YELLOW);
                            }
                        }
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (tempConfigData.adjustConfig.offsetS1 > -999) {
                                tempConfigData.adjustConfig.offsetS1--;
                                DrawAdjustOffset(0, 0, YELLOW);
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (tempConfigData.adjustConfig.offsetS2 > -999) {
                                tempConfigData.adjustConfig.offsetS2--;
                                DrawAdjustOffset(0, 0, YELLOW);
                            }
                        }
                        break;
                    case BUTTON_ENTER:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (configData.adjustConfig.offsetS1 != tempConfigData.adjustConfig.offsetS1) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
						}
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (configData.adjustConfig.offsetS2 != tempConfigData.adjustConfig.offsetS2) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        }
						else {
                            if (configData.adjustConfig.offsetS1 != tempConfigData.adjustConfig.offsetS1 || configData.adjustConfig.offsetS2 != tempConfigData.adjustConfig.offsetS2) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
						}
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

void State_ConfigAdjustFilter(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            compareValue = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            DrawAdjustFilter(cursor, compareValue, WHITE);
                            cursor ^= 1;
                            DrawAdjustFilter(cursor, compareValue, YELLOW);
                        }
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (tempConfigData.adjustConfig.filterS1 < 999) {
                                ++tempConfigData.adjustConfig.filterS1;
                                DrawAdjustFilter(0, 0, YELLOW);
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (tempConfigData.adjustConfig.filterS2 < 999) {
                                ++tempConfigData.adjustConfig.filterS2;
                                DrawAdjustFilter(0, 0, YELLOW);
                            }
                        }
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (tempConfigData.adjustConfig.filterS1 > 0) {
                                --tempConfigData.adjustConfig.filterS1;
                                DrawAdjustFilter(0, 0, YELLOW);
                            }
                        }
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (tempConfigData.adjustConfig.filterS2 > 0) {
                                --tempConfigData.adjustConfig.filterS2;
                                DrawAdjustFilter(0, 0, YELLOW);
                            }
                        }
                        break;
                    case BUTTON_ENTER:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                            if (configData.adjustConfig.filterS1 != tempConfigData.adjustConfig.filterS1) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                configData.adjustConfig.filter_update_flag = 1;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        } 
                        else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (configData.adjustConfig.filterS2 != tempConfigData.adjustConfig.filterS2) {
                                configData.adjustConfig = tempConfigData.adjustConfig;
                                configData.adjustConfig.filter_update_flag = 1;
                                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            }
                        } 
                        if (configData.adjustConfig.filterS1 == 0 || tempConfigData.adjustConfig.filterS1 == 0) {
                            configData.adjustConfig = tempConfigData.adjustConfig;
                            configData.adjustConfig.filter_update_flag = 0;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }

                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

void State_ConfigAdjustFactoryReset(void) {
    switch (subState) {
        case 0:
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        break;
                    case BUTTON_RIGHT:
                        break;
                    case BUTTON_ENTER:
                        FactoryReset();
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

//  ???? - ??????- ???? - ???????
void State_ConfigRelayRelay1AutoWash(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
			configData.relayConfig.relay1manual=0;

            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawRelay1Wash(cursor, WHITE);
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
                            cursor ^= 1;
                        else if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            if (cursor == 0)
                                cursor = 2;
                            else
                                --cursor;
                        }
                        DrawRelay1Wash(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawRelay1Wash(cursor, WHITE);
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
                            cursor ^= 1;
                        else if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            ++cursor;
                            cursor = cursor % 3;
                        }
                        DrawRelay1Wash(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.relayConfig.relay1WashCycle < 999)
                                    ++tempConfigData.relayConfig.relay1WashCycle;
                            } else {
                                if (tempConfigData.relayConfig.relay1WashTime < 999)
                                    ++tempConfigData.relayConfig.relay1WashTime;
                            }
                        } else {
                        }
                        DrawRelay1Wash(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.relayConfig.relay1WashCycle > 0)
                                    --tempConfigData.relayConfig.relay1WashCycle;
                            } else {
                                if (tempConfigData.relayConfig.relay1WashTime > 0)
                                    --tempConfigData.relayConfig.relay1WashTime;
                            }
                        } else {
                        }
                        DrawRelay1Wash(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        if (tempConfigData.relayConfig.relay1WashCycle != configData.relayConfig.relay1WashCycle || tempConfigData.relayConfig.relay1WashTime != configData.relayConfig.relay1WashTime) {
                            configData.relayConfig = tempConfigData.relayConfig;
                            wash_run_flag = 1;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        } else if (tempConfigData.relayConfig.relay1WashCycle == 0 || tempConfigData.relayConfig.relay1WashTime == 0) {
                            configData.relayConfig = tempConfigData.relayConfig;
                            wash_run_flag = 0;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }

                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}


// ????????
void State_ConfigRelayRelay1manualWASH(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 1;

            display_set11_run(0);
            display_set11_stop(1);
            break;

        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_UP:
                    case BUTTON_DOWN:
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        if (tempConfigData.relayConfig.relay1manual==1) {
	                        tempConfigData.relayConfig.relay1manual=2;

                            //DrawTextETC(200, 130, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
			                display_set11_run(1);
			                display_set11_stop(0);
							display_message1();	//RUNNing

                            //configData.relayConfig.relay1WashCycle = 0;
                            //configData.relayConfig.relay1WashTime = 0;
                            wash_run_flag = 0;
                            //Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            RELAY1_ON;
                        } else {
	                        tempConfigData.relayConfig.relay1manual=1;
                            //DrawTextETC(200, 130, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
			                display_set11_run(0);
			                display_set11_stop(1);
							display_message_STOP();	//STOP


                            //configData.relayConfig.relay1WashCycle = 0;
                            //configData.relayConfig.relay1WashTime = 0;
                            wash_run_flag = 0;
                            //Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            RELAY1_OFF;
                        }
                        break;
                    case BUTTON_ENTER:
                        if (configData.relayConfig.relay1manual != tempConfigData.relayConfig.relay1manual) {
                            configData.relayConfig.relay1manual = tempConfigData.relayConfig.relay1manual;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        subState = 0;
                        state = STATE_MAIN;

						// ??????? ???????
						if (configData.relayConfig.relay1WashCycle>0) wash_run_flag = 1;	

                        break;
                }
            }
            break;
    }
}

void State_ConfigRelayRelay2AutoAlarm(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;

            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
//            subState = 1;
            cursor = 0;

            display_autoalarm();
            display_message_STOP(); // STOP

            display_set11_run(1);
            display_set11_stop(0);


			// ??? ???????.
			{
            	configData.relayConfig.relay2AutoAlarm = 1;
				tempConfigData.relayConfig.relay2AutoAlarm =1;

                configData.relayConfig.relay2AlarmOFF = 0;
				tempConfigData.relayConfig.relay2AlarmOFF =0;

                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
            }
            subState = 0;
            state = STATE_MAIN;


            break;

    }
}

void State_ConfigRelayRelay2AlarmOFF(void) {

    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
//            subState = 1;
            cursor = 0;

            display_alarm_off();
            display_message_STOP(); // STOP


            display_set11_run(1);
            display_set11_stop(0);


			// ??? ???????.
			{
            	configData.relayConfig.relay2AutoAlarm = 0;
				tempConfigData.relayConfig.relay2AutoAlarm =0;

                configData.relayConfig.relay2AlarmOFF = 1;
				tempConfigData.relayConfig.relay2AlarmOFF =1;

                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
            }
            subState = 0;
            state = STATE_MAIN;


            break;

    }
}

void State_ConfigRelayRelay3SetOP(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
			configData.relayConfig.relay3manual=0;

            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;

            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_LEFT:
                        DrawRelay3Wash(cursor, WHITE);
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
                            cursor ^= 1;
                        else if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            if (cursor == 0)
                                cursor = 2;
                            else
                                --cursor;
                        }
                        DrawRelay3Wash(cursor, YELLOW);
                        break;
                    case BUTTON_RIGHT:
                        DrawRelay3Wash(cursor, WHITE);
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
                            cursor ^= 1;
                        else if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
                            ++cursor;
                            cursor = cursor % 3;
                        }
                        DrawRelay3Wash(cursor, YELLOW);
                        break;
                    case BUTTON_UP:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.relayConfig.relay3WashCycle < 999)
                                    ++tempConfigData.relayConfig.relay3WashCycle;
                            } else {
                                if (tempConfigData.relayConfig.relay3WashTime < 999)
                                    ++tempConfigData.relayConfig.relay3WashTime;
                            }
                        }
                        else {

                        }
                        DrawRelay3Wash(cursor, YELLOW);
                        break;
                    case BUTTON_DOWN:
                        if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                            if (cursor == 0) {
                                if (tempConfigData.relayConfig.relay3WashCycle > 0)
                                    --tempConfigData.relayConfig.relay3WashCycle;
                            } else {
                                if (tempConfigData.relayConfig.relay3WashTime > 0)
                                    --tempConfigData.relayConfig.relay3WashTime;
                            }
                        }
                        else {

                        }
                        DrawRelay3Wash(cursor, YELLOW);
                        break;
                    case BUTTON_ENTER:
                        if (tempConfigData.relayConfig.relay3WashCycle != configData.relayConfig.relay3WashCycle || tempConfigData.relayConfig.relay3WashTime != configData.relayConfig.relay3WashTime) {
                            configData.relayConfig = tempConfigData.relayConfig;
                            wash_run_flag3 = 1;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        } else if (tempConfigData.relayConfig.relay3WashCycle == 0 || tempConfigData.relayConfig.relay3WashTime == 0) {
                            configData.relayConfig = tempConfigData.relayConfig;
                            wash_run_flag3 = 0;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }

                        subState = 0;
                        state = STATE_MAIN;
                        break;
                }
            }
            break;
    }
}

void State_ConfigRelayRelay3manualOP(void) {
    switch (subState) {
        case 0:
            tempConfigData = configData;
            RedrawTitle();
            RedrawBottomArea();
            RedrawViewArea();
            subState = 1;
            cursor = 0;
            break;
        case 1:
            if (flag10ms & FLAG10MS_BUTTON) {
                flag10ms &= ~FLAG10MS_BUTTON;
                switch (CheckButton()) {
                    case BUTTON_HOME:
                        subState = 0;
                        state = STATE_MAIN;
                        break;
                    case BUTTON_BACK:
                        subState = 0;
                        RedrawMainView();
                        MoveBackState();
                        break;
                    case BUTTON_UP:
                    case BUTTON_DOWN:
                        break;
                    case BUTTON_LEFT:
                    case BUTTON_RIGHT:
                        if ((tempConfigData.relayConfig.relay3manual) == 1) {
	                        tempConfigData.relayConfig.relay3manual =2;
                            //DrawTextETC(200, 130, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
			                display_set11_run(1);
			                display_set11_stop(0);

                            display_message2(); //running...

                            //configData.relayConfig.relay3WashCycle = 0;
                            //configData.relayConfig.relay3WashTime = 0;
                            wash_run_flag3 = 0;
                            //Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            RELAY3_ON;
                        } else {
                            tempConfigData.relayConfig.relay3manual =1;
                            //DrawTextETC(200, 130, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
                            TFT_Fill(122, 126, 122 + 250, 126 + 64, WHITE);
			                display_set11_run(0);
			                display_set11_stop(1);

                            display_message_STOP(); //stop

                            //configData.relayConfig.relay3WashCycle = 0;
                            //configData.relayConfig.relay3WashTime = 0;
                            wash_run_flag3 = 0;
                            //Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                            RELAY3_OFF;
                        }
                        break;
                    case BUTTON_ENTER:
                        if (configData.relayConfig.relay3manual != tempConfigData.relayConfig.relay3manual) {
                            configData.relayConfig.relay3manual = tempConfigData.relayConfig.relay3manual;
                            Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
                        }
                        subState = 0;
                        state = STATE_MAIN;

						// ????3 ???????
						if (configData.relayConfig.relay3WashCycle>0) wash_run_flag3 = 1;	

                        break;
                }
            }
            break;
    }
}

void MoveBackState(void) {
    state = (state >> 4)&0xFFFFFFFF;
}

void MoveSelectedState(void) {
    state = (state << 4)&0xFFFFFFFF;
    state |= cursor + 1;
}

void FactoryReset(void) {
    configData.outputConfig.output4mA = 0;
    configData.outputConfig.output4mA2 = 0;

    configData.outputConfig.output20mA = 2000;
    configData.outputConfig.output20mA2 = 10000;

    configData.outputConfig.outputReal4mA =  DEFAULT_4MA;
    configData.outputConfig.outputReal20mA = DEFAULT_20MA;

    configData.outputConfig.outputReal4mA2 =  DEFAULT_4MA;
    configData.outputConfig.outputReal20mA2 = DEFAULT_20MA2;

    configData.modbusConfig.mode = 0;
    configData.modbusConfig.baudrate = 0;
    configData.modbusConfig.modbusSlaveAddr = 0;
    configData.modbusConfig.databit = 0;
    configData.modbusConfig.modbusSensor1Addr = 2;   /* pH 센서 기본 Slave ID */
    configData.modbusConfig.modbusSensor2Addr = 4;   /* EC 센서 기본 Slave ID */

    configData.relayConfig.relay1WashCycle = 0;
    configData.relayConfig.relay1WashTime = 0;
    configData.relayConfig.relay1manual = 0;
    configData.relayConfig.relay2AutoAlarm = 0;
    configData.relayConfig.relay2WashTime = 0;
    configData.relayConfig.relay2AlarmOFF = 0;
    configData.relayConfig.relay3WashCycle = 0;
    configData.relayConfig.relay3WashTime = 0;
    configData.relayConfig.relay3manual = 0;

    configData.adjustConfig.gradientS1 = 100;
    configData.adjustConfig.gradientS2 = 100;
    configData.adjustConfig.offsetS1 = 0;
    configData.adjustConfig.offsetS2 = 0;
    configData.adjustConfig.filterS1 = 15;
    configData.adjustConfig.filterS2 = 15;

    configData.calibrationConfig.S1zeroCal = 0;


    configData.calibrationConfig.PH4_Cal = 400;
    configData.calibrationConfig.PH4_Value = 400;
    configData.calibrationConfig.PH7_Cal= 700;
    configData.calibrationConfig.PH7_Value= 700;

    configData.calibrationConfig.EC_Cal = 0;
    configData.calibrationConfig.EC_Value = 0;


    configData.calibrationConfig.PH_Span_Cal = 1;
    configData.calibrationConfig.PH_Span_Value = 1;

    configData.calibrationConfig.EC_Span_Cal = 14130;
    configData.calibrationConfig.EC_Span_Value = 14130;

    configData.calibrationConfig.TEMP_Span_Cal1 = 1;
    configData.calibrationConfig.TEMP_Span_Value1 = 1;

    configData.calibrationConfig.TEMP_Span_Cal2 = 1;
    configData.calibrationConfig.TEMP_Span_Value2 = 1;

    configData.calibrationConfig.TEMP_Span_Cal3 = 1;
    configData.calibrationConfig.TEMP_Span_Value3 = 1;

    configData.calibrationConfig.TEMP_Span_Cal4 = 1;
    configData.calibrationConfig.TEMP_Span_Value4 = 1;


    configData.calibrationConfig.S2zeroCal = 0;
    configData.calibrationConfig.S1manualCal = 0;
    configData.calibrationConfig.S2manualCal = 0;
    configData.calibrationConfig.S1Value = 0;
    configData.calibrationConfig.S2StdValue = 0;
    configData.calibrationConfig.errorRange = 0;
    configData.calibrationConfig.S2cycle = 0;


    configData.alarmConfig.highLimit = 0;
    configData.alarmConfig.lowLimit = 0;
    configData.alarmConfig.zeroAlarm = 0;

    configData.alarmConfig.highLimit2 = 0;
    configData.alarmConfig.lowLimit2 = 0;
    configData.alarmConfig.zeroAlarm2 = 0;
    Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*) & configData, sizeof (ConfigSet));
    Clear_EEprom_Data();
}


#define DAY_X 28	//10
#define DAY_Y 233
#define TIME_X 193	//179
#define TIME_Y 233


void RedrawTime(void) {
    currentTime = Time_GetCalendarTime();
    if (currentTime.tm_sec != prevTime.tm_sec) {


   	    sprintf(strBuffer, "%04d-%02d-%02d", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday);
		if (day_up>0) {
//    	    sprintf(strBuffer, "%04d-%02d-%02d", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday);
	        Draw_Back_Oval_140x42(20, 220);
//        	DrawSmallNumber(DAY_X, DAY_Y, strBuffer, WHITE);
			day_up--;
		}
       	DrawSmallNumber(DAY_X, DAY_Y, strBuffer, WHITE);
		if (currentTime.tm_mday != prevTime.tm_mday) day_up=2;


        sprintf(strBuffer, "%02d:%02d:%02d", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

		if (time_up>0) {
			Draw_Back_Oval_140x42(172, 220);
			time_up--;
		}
		if (currentTime.tm_min != prevTime.tm_min) time_up=2;

		//TFT_Fill(269,228, 269+35,228+28, WHITE);	 // ????? ???.
        DrawSmallNumber(TIME_X, TIME_Y, strBuffer, WHITE);
    }


    prevTime = currentTime;
}

void ClearstrBuffer(void) {
    int i;

    for (i = 0; i < 35; i++) {
        strBuffer[i] = 0x00;
    }
}

