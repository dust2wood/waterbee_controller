#include "Interrupt.h"


uint32_t flag10ms = 0;


uint16_t rx1Buffer[32] = {0,};
uint16_t rx2Buffer[32] = {0,};
uint16_t rx3Buffer[32] = {0,};
uint16_t tx1Buffer[15] = {0,};
uint16_t tx2Buffer[32] = {0,};
uint16_t tx3Buffer[32] = {0,};

uint16_t tx1Size = 0, tx1Count = 0, rx1Size = 0;
uint16_t tx2Size = 0, tx2Count = 0, rx2Size = 0;
uint16_t tx3Size = 0, tx3Count = 0, rx3Size = 0;

uint32_t flag_wash1_time_counter = 0;
uint32_t flag_wash2_time_counter = 0;
uint32_t flag_wash3_time_counter = 0;

uint32_t flag_wash1_relay_time_counter = 0;
uint32_t flag_wash2_relay_time_counter = 0;
uint32_t flag_wash3_relay_time_counter = 0;

uint32_t zero_Alarm_Time_couter = 0;
uint32_t zero_Alarm_Time_couter2 = 0;

uint32_t MicroSdCard_Time_Coutner = 0;
uint32_t MicrosdCard_Run_flag = 0;


extern uint16_t rx2_time_count, rx3_time_count;

void TIM2_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 

	if( wash_run_flag == 1) 	{  flag_wash1_time_counter++; }
  	else { flag_wash1_time_counter = 0;   }

	if(Relay1_run_on_flag == 1) { flag_wash1_relay_time_counter++;  }

	if(Alarm_OP_flag == 0 && configData.alarmConfig.zeroAlarm != 0) { zero_Alarm_Time_couter++; }
	else { zero_Alarm_Time_couter = 0; }

	if(Alarm_OP_flag == 0 && configData.alarmConfig.zeroAlarm2 != 0) { zero_Alarm_Time_couter2++; }
	else { zero_Alarm_Time_couter2 = 0; }

 	//-------------------------------------------------------------------------------------//
  	if( wash_run_flag3 == 1) 	{  flag_wash3_time_counter++; }
  	else { flag_wash3_time_counter = 0;   }

	if(Relay3_run_on_flag == 1) { flag_wash3_relay_time_counter++;  }
	//---------------------------------------------------------------------------------------//

	if( MicrosdCard_Run_flag == 0 && sd_dec_flag == 1)
	{
     	MicroSdCard_Time_Coutner++;
//		if(MicroSdCard_Time_Coutner == 1350)
		if(MicroSdCard_Time_Coutner == 300)
	    {
			MicroSdCard_Time_Coutner = 0;
	      	MicrosdCard_Run_flag = 1;		 
      	}
  	}

	if (rx2_time_count<1000) rx2_time_count++;
	if (rx3_time_count<1000) rx3_time_count++;

}

uint16_t Counter10msec = 0;
uint16_t Comm_time_flag = 0;

uint16_t Timer10msec, TimerSec, TimerMin;

extern uint32_t ButtonTimer;


// trand 하기위한 변수들 
extern uint16_t trand_time1, trand_time6, trand_time12, trand_time24, trand_time168, trand_time336;

// Trand 는 60분(1분단위), 6시간(6분단위), 12시간(12분단위), 24시간(24분단위), 168시간=7일(168분단위), 336시간=14일(336분단위)
#define trand_select_time_NO 6		// 시간범위 선택하는거
extern uint16_t trand_select_time;		// 시간범위 선택하는거


extern int16_t trandData[trand_select_time_NO][61];
extern int16_t trandData2[trand_select_time_NO][61];
extern uint8_t trandData_TIME[trand_select_time_NO][61][6];

extern char flag_write_trand1, flag_write_trand6, flag_write_trand12, flag_write_trand24, flag_write_trand168, flag_write_trand336;

extern unsigned char	Sensor1_OK_TIME;
extern unsigned char	Sensor2_OK_TIME;
extern unsigned char	Sensor3_OK_TIME;
extern unsigned char	Sensor4_OK_TIME;
extern uint16_t	  Relay2_off_time, Relay2_off_time2;


void data_rotate(int n)
{
	int i,j;

	for (i=0;i<60;i++)	{
		trandData[n][i] = trandData[n][i+1];
		trandData2[n][i] = trandData2[n][i+1];
		for (j=0;j<6;j++)
			trandData_TIME[n][i][j] = trandData_TIME[n][i+1][j];
	}

//    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
		trandData[n][60] = currentData.S1PPM;
//	}
//    else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
		trandData2[n][60] = currentData.S2PPM;
//	}
	trandData_TIME[n][60][0] = currentTime.tm_year-2000;
	trandData_TIME[n][60][1] = currentTime.tm_mon+1;
	trandData_TIME[n][60][2] = currentTime.tm_mday;
	trandData_TIME[n][60][3] = currentTime.tm_hour;
	trandData_TIME[n][60][4] = currentTime.tm_min;
	trandData_TIME[n][60][5] = currentTime.tm_sec;
}

void CheckTrandData(void)
{
	char i,j;

	//trand_time1++;

	trand_time6++;
	trand_time12++;
	trand_time24++;
	trand_time168++;
	trand_time336++;


	// 0번
	// 대이터 회전해야함
	data_rotate(0);
	flag_write_trand1=1;


	// 1번 
	if (trand_time6>=6) { 
	 	trand_time6=0; 

		// 대이터 회전해야함
		data_rotate(1);
		flag_write_trand6=1;
	}	

	// 2번 
	if (trand_time12>=12) { 
	 	trand_time12=0; 

		// 대이터 회전해야함
		data_rotate(2);
		flag_write_trand12=1;
	}	

	// 3번 
	if (trand_time24>=24) { 
	 	trand_time24=0; 
		// 대이터 회전해야함
		data_rotate(3);

		flag_write_trand24=1;
	}	

	// 4번 
	if (trand_time168>=168) { 
	 	trand_time168=0; 
		// 대이터 회전해야함
		data_rotate(4);
		flag_write_trand168=1;
	}	


	// 5번 
	if (trand_time336>=336) { 
	 	trand_time336=0; 
		// 대이터 회전해야함
		data_rotate(5);
		flag_write_trand336=1;
	}	

}


void TIM3_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
	flag10ms = 0xFFFFFFFF;	

#ifdef BUZ_ENABLE
	if (buz_time>0) { buz_time--;	BUZ_ON;	}
	else 			  				BUZ_OFF;
#else
	BUZ_OFF;
#endif


	Timer10msec++;

	// 1초마다...
	if (Timer10msec>100) {
		TimerSec++;

		if (Sensor1_OK_TIME>0) Sensor1_OK_TIME--;
		if (Sensor1_OK_TIME==0) {
			Sensor_State1 = SENSOR_ERROR3;
		}

		if (Sensor2_OK_TIME>0) Sensor2_OK_TIME--;
		if (Sensor2_OK_TIME==0) {
			Sensor_State2 = SENSOR_ERROR3;
		}

		if (Sensor3_OK_TIME>0) Sensor3_OK_TIME--;
		if (Sensor3_OK_TIME==0) {
			Sensor_State3 = SENSOR_ERROR3;
		}

		if (Sensor4_OK_TIME>0) Sensor4_OK_TIME--;
		if (Sensor4_OK_TIME==0) {
			Sensor_State4 = SENSOR_ERROR3;
		}

		if (Relay2_off_time>0) Relay2_off_time--;
		if (Relay2_off_time2>0) Relay2_off_time2--;


		// 1분마다 
		if (TimerSec>=60) {
			TimerSec=0;
			TimerMin++;
			if (TimerMin>=60) {
				TimerMin=0;
			}
			CheckTrandData();
		}
		Timer10msec=0;
	}


	if(Comm_time_flag == 1)
	{
      Counter10msec++;
  	}

	if (ButtonTimer>0) ButtonTimer--;
}

void TIM4_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
}
void TIM5_IRQHandler(void)  
{
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update); 
}


//=============================================

extern uint32_t REqParmeterPPM;
extern uint32_t REqParmeterNTU;
extern uint32_t REqParmeterCurrent;

extern uint32_t REqParmeterZero_PPM;
extern uint32_t REqParmeterZero_Current;

extern uint32_t SendParmeter_Current;
extern uint32_t SendParmeter_Zero_Current;

extern uint32_t Couter_1 ;
extern uint32_t Couter_2 ;
extern uint32_t Couter_3 ;
extern uint32_t Couter_4 ;

extern uint32_t ReqParmeterTemp;

extern uint16_t ErrorCodeRecive;
extern uint16_t ComCouter;
extern uint16_t ComCouter2;

extern char WORKING_TIMER;
extern uint32_t ReqParmeterZero_NTU;


// 센서 에러 표시하는 센서값의 하한값, 상한값
// 에러 0~6.6 (660), 최대값 327.30 (32730) 
#define S1_ERROR_LOW	660
#define S1_ERROR_HIGH	32730


#define S1_PH_ERROR_LOW		0
#define S1_PH_ERROR_HIGH	1400


#define S2_EC_ERROR_LOW		0
#define S2_EC_ERROR_HIGH	200000


// NTU 탁도 
// 센서이상 0~350.0   400.0~450.0  4500.0~
// 램프이상 350.0 ~ 400.0

#define S2_ERROR_LOW	0
#define S2_ERROR_HIGH	 1800
#define S2_ERROR_MAX_HIGH	45000

#define S2_ERROR_LAMP_LOW	800
#define S2_ERROR_LAMP_HIGH	1200


// 새로정해야 함
//PH
#define S3_ERROR_LOW	0
#define S3_ERROR_HIGH	4500
//EC
#define S4_ERROR_LOW	0
#define S4_ERROR_HIGH	4500


extern int16_t data_TEMP, SET_data_TEMP;

void USART1_IRQHandler(void)
{
	uint8_t  data;
	uint32_t data32;
	uint8_t	 sensor_no;
	uint32_t sensor;
	int32_t  temp;
	int32_t  e, f;


	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET) 
	{
		USART1->SR &= USART1->SR&(~0x40);

		if(tx1Count < tx1Size)
			USART1->DR = tx1Buffer[tx1Count++];
		else
		{
			USART1->CR1 &= ~0x40;		
		}
	}
	else
	{
		USART1->SR = (USART1->SR)&(~((USART1->SR)&0X20));
		data = USART1->DR;
		if (data=='$') rx1Size = 0;

		rx1Buffer[rx1Size] = data;
		++rx1Size;
		rx1HandlerCount  = 0;

		// SET A,B,C,D,T 에 대한 응답
		if (data==0x0a && rx1Size==12) {
			rx1Size=0;
			if (rx1Buffer[0]=='$' && rx1Buffer[1]=='R' && rx1Buffer[2]=='E' && rx1Buffer[3]=='S' && rx1Buffer[5]==',')
			{
				data32 =  (rx1Buffer[6]-0x30)*10000 + (rx1Buffer[7]-0x30)*1000 + (rx1Buffer[8]-0x30)*100 + (rx1Buffer[9]-0x30)*10 + (rx1Buffer[10]-0x30);

				if (rx1Buffer[4]=='D') 
				{
					if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                    REqParmeterPPM = data32;
	                    if (REqParmeterPPM == Tx_S1manualCal && manual_cal_flag == 1) {
	                        req_ppm_com_flag = 1;
	                    }
	                    Couter_1++;
					}
					else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
		              	REqParmeterNTU = data32;
		              	if (REqParmeterNTU == Tx_S2manualCal && manual_cal_flag == 1) {
		                  	req_ppm_com_flag = 1;
		              	}
					}

				}

				else if (rx1Buffer[4]=='C') 
				{
                    REqParmeterCurrent = data32;
                    if (REqParmeterCurrent == SendParmeter_Current && manual_cal_flag == 1) {
                        req_current_com_flag = 1;

                    }
                    Couter_2++;
				}
				else if (rx1Buffer[4]=='T') 
				{
                    ReqParmeterTemp = data32;
                    if (ReqParmeterTemp == Tx_Adj_TempertureVaule && manual_cal_temp_flag == 1) {
                        req_temp_com_flag = 1;

                    }
                    Couter_3++;
				}
				else if (rx1Buffer[4]=='B') 
				{	
					// - 일때 처리해야하는데 지금은 안됨...프로토콜이 안맞음
					if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
						REqParmeterZero_PPM = data32;
	                    if (REqParmeterZero_PPM == Tx_S1ZeroCal && zero_cal_update_flag == 1) {
	                        req_zero_ppm_com_flag = 1;
	                    }
					}
					else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
						ReqParmeterZero_NTU = data32;
	                    if (ReqParmeterZero_NTU == Tx_S2ZeroCal && zero_cal_update_flag == 1) {
	                        req_zero_ppm_com_flag = 1;
	                    }
					}
                    Couter_4++;
				}
				else if (rx1Buffer[4]=='A') 
				{
					if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                    REqParmeterZero_Current = data32;
	                    if (REqParmeterZero_Current == SendParmeter_Zero_Current && req_zero_ppm_com_flag == 1) {
	                        req_zero_current_com_flag = 1;
	                    }
					}
					else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
			             ReqParmeterZero_NTU = data32;
			             if (ReqParmeterZero_NTU == Tx_S2ZeroCal && zero_cal_update_flag == 1) {
			                 req_zero_ppm_com_flag = 1;
			             }
					}
				}
				else if (rx1Buffer[4]=='E') 
				{
                    ErrorCodeRecive = data32;
                }
			}
		}
		// 센서값 읽어오는거
		else if (data==0x0a && rx1Size==24) {
			rx1Size=0;
            if (rx1Buffer[0] == '$' && rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'V' && rx1Buffer[5] == ',' && manual_cal_flag == 0 && manual_cal_temp_flag == 0 && zero_cal_update_flag == 0) 
			{
				{
                    //DrawTextsize120(MEASURE_X2, MEASURE_Y, TEXT120_MEASURING, DRAW_IMAGE_ENABLE);
//					if (WORKING_TIMER==0) { DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE); WORKING_TIMER=1;}
//					else 				  { DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE); WORKING_TIMER=0;}

					// 0=no comm, 1=DISPLAY, 2=no DISPLAY
					if (WORKING_TIMER==0) 		{ WORKING_TIMER=1;}
					else if (WORKING_TIMER==1) 	{ WORKING_TIMER=2;}
					else 				  		{ WORKING_TIMER=1;}

                }
                if (rx1Buffer[6] != 0x2D) {
					if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                    currentData.comm_S1PPM = (rx1Buffer[6] - 0x30) * 10000 + (rx1Buffer[7] - 0x30) * 1000 + (rx1Buffer[8] - 0x30) * 100 + (rx1Buffer[9] - 0x30) * 10 + (rx1Buffer[10] - 0x30);
						Sensor1_OK_TIME=10;	// 10=10sec
					}
					else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
						currentData.comm_S2NTU = (rx1Buffer[6] - 0x30) * 10000 + (rx1Buffer[7] - 0x30) * 1000 + (rx1Buffer[8] - 0x30) * 100 + (rx1Buffer[9] - 0x30) * 10 + (rx1Buffer[10] - 0x30);
						Sensor2_OK_TIME=10;	// 10=10sec
					}
                }
                //ComCouter++;
                ComCouter=0;
                ComCouter2=0;

                if (rx1Buffer[11] == ',') {
					if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                    currentData.S1mVolt = (rx1Buffer[12] - 0x30) * 10000 + (rx1Buffer[13] - 0x30) * 1000 + (rx1Buffer[14] - 0x30) * 100 + (rx1Buffer[15] - 0x30) * 10 + (rx1Buffer[16] - 0x30);
    	                currentData.S1mV = currentData.S1mVolt;
					}
					else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
	                    currentData.S2mVolt = (rx1Buffer[12] - 0x30) * 10000 + (rx1Buffer[13] - 0x30) * 1000 + (rx1Buffer[14] - 0x30) * 100 + (rx1Buffer[15] - 0x30) * 10 + (rx1Buffer[16] - 0x30);
    	                currentData.S2mV = currentData.S2mVolt;
					}
                }

//                S1PPm_Filter_OUT_function();
//                S1PPm_Data_offset_function();
                //RedrawValue();

				// 1234 = 12.34도, 최대 9999 = 99.99도, 영하는 2660 = -26.6 도
                if (rx1Buffer[17] == ',') {
					if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
	                    currentData.temperature = (rx1Buffer[19] - 0x30) * 1000 + (rx1Buffer[20] - 0x30) * 100 + (rx1Buffer[21] - 0x30) * 10 + (rx1Buffer[22] - 0x30);
						if (rx1Buffer[18]=='-') currentData.temperature*=(-1);	// 영하
					}
					else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
	                    currentData.temperature1 = (rx1Buffer[19] - 0x30) * 1000 + (rx1Buffer[20] - 0x30) * 100 + (rx1Buffer[21] - 0x30) * 10 + (rx1Buffer[22] - 0x30);
						if (rx1Buffer[18]=='-') currentData.temperature*=(-1);	// 영하
					}


                }
                comm_run_flag = 2;


#ifdef  SENSOR_PH_EC
				// 센서값이 너무작거나 너무 크면, 센서점검 에러 표시함
				if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {


					// value 가 0과, 14.0 이상이면 에러로 하자 20241118
					if (currentData.S1PPM<=S1_PH_ERROR_LOW || currentData.S1PPM>=S1_PH_ERROR_HIGH) {
//					if (currentData.S1mV<S1_ERROR_LOW || currentData.S1mV>S1_ERROR_HIGH) {
						Sensor_State1=SENSOR_ERROR1;
					}
					else { //if (Sensor_State!=SENSOR_ERROR3)	
						Sensor_State1=SENSOR_OK;
					}
					Sensor1_OK_TIME=10;	// 10=10sec

				}
				else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
					// 센서이상 
					if (currentData.S2mV<S2_EC_ERROR_LOW || currentData.S2mV>S2_EC_ERROR_HIGH) {
						Sensor_State2=SENSOR_ERROR1;
					}
					else {//if (Sensor_State!=SENSOR_ERROR3)	
						Sensor_State2=SENSOR_OK;
					}
					Sensor2_OK_TIME=10;	// 10=10sec

				}
				ComCouter  = 0;
   				ComCouter2 = 0;
#endif

            } 		   

			// 20240325 통신 추가  $RES1 ~ $RES4
            else if (rx1Buffer[0] == '$' && rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[5] == ',' && manual_cal_flag == 0 && manual_cal_temp_flag == 0 && zero_cal_update_flag == 0) 
			{
				if (rx1Buffer[4]>='1' && rx1Buffer[4]<='4') 
				{
					sensor_no = rx1Buffer[4]-0x30;
					{
	                    //DrawTextsize120(MEASURE_X2, MEASURE_Y, TEXT120_MEASURING, DRAW_IMAGE_ENABLE);
	//					if (WORKING_TIMER==0) { DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE); WORKING_TIMER=1;}
	//					else 				  { DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE); WORKING_TIMER=0;}

						// 0=no comm, 1=DISPLAY, 2=no DISPLAY
						if (WORKING_TIMER==0) 		{ WORKING_TIMER=1;}
						else if (WORKING_TIMER==1) 	{ WORKING_TIMER=2;}
						else 				  		{ WORKING_TIMER=1;}

	                }
	                if (rx1Buffer[6] != 0x2D) {
	                    sensor  = (rx1Buffer[6] - 0x30) * 10000 + (rx1Buffer[7] - 0x30) * 1000 + (rx1Buffer[8] - 0x30) * 100 + (rx1Buffer[9] - 0x30) * 10 + (rx1Buffer[10] - 0x30);

						sensor = S1PPm_Filter_OUT_function(sensor_no-1, sensor);

#ifdef CH2
	#ifndef  SENSOR_PH_EC
						if 		(sensor_no==1) currentData.comm_S1PPM	= sensor;
						else if (sensor_no==2) currentData.comm_S2NTU 	= sensor;
	#else
						if 		(sensor_no==3) currentData.comm_S1PPM 	= sensor;	// PH
						else if (sensor_no==4) currentData.comm_S2NTU 	= sensor;	// EC
	#endif
#else	// CH4
						if 		(sensor_no==1) currentData.comm_S1PPM	= sensor;
						else if (sensor_no==2) currentData.comm_S2NTU 	= sensor;
						else if (sensor_no==3) currentData.comm_S3PH 	= sensor;	// PH
						else if (sensor_no==4) currentData.comm_S4EC 	= sensor;	// EC
#endif
	                }
	                //ComCouter++;
	                ComCouter=0;
	                ComCouter2=0;

	                if (rx1Buffer[11] == ',') {
	                    sensor = (rx1Buffer[12] - 0x30) * 10000 + (rx1Buffer[13] - 0x30) * 1000 + (rx1Buffer[14] - 0x30) * 100 + (rx1Buffer[15] - 0x30) * 10 + (rx1Buffer[16] - 0x30);
#ifdef CH2
	#ifndef  SENSOR_PH_EC
						if 		(sensor_no==1) { currentData.S1mVolt = sensor; currentData.S1mV = sensor;}
						else if (sensor_no==2) { currentData.S2mVolt = sensor; currentData.S2mV = sensor;}
	#else
						if 		(sensor_no==3) { currentData.S1mVolt = sensor; currentData.S1mV = sensor;}
						else if (sensor_no==4) { currentData.S2mVolt = sensor; currentData.S2mV = sensor;}
	#endif
#else	// CH4
						if 		(sensor_no==1) { currentData.S1mVolt = sensor; currentData.S1mV = sensor;}
						else if (sensor_no==2) { currentData.S2mVolt = sensor; currentData.S2mV = sensor;}
						else if (sensor_no==3) { currentData.S3mVolt = sensor; currentData.S3mV = sensor;}
						else if (sensor_no==4) { currentData.S4mVolt = sensor; currentData.S4mV = sensor;}
#endif

	                }

//	                S1PPm_Filter_OUT_function();
					S1PPm_Data_offset_function();
	                //RedrawValue();

					// 1234 = 12.34도, 최대 9999 = 99.99도, 영하는 2660 = -26.6 도
	                if (rx1Buffer[17] == ',') {
	                    data_TEMP = (rx1Buffer[19] - 0x30) * 1000 + (rx1Buffer[20] - 0x30) * 100 + (rx1Buffer[21] - 0x30) * 10 + (rx1Buffer[22] - 0x30);
						if (rx1Buffer[18]=='-') data_TEMP*=(-1);	// 영하


						//=====================
						// 온도 데이터 받아서 게산하기
						//=====================
						// f:교정값, e:교정시 측정값, e0:지금 측정값 
						// y=f/e * e0; 	
#ifdef CH2
	#ifndef  SENSOR_PH_EC	   
						if 		(sensor_no==1) {
							f = configData.calibrationConfig.TEMP_Span_Cal1;
							e = configData.calibrationConfig.TEMP_Span_Value1;
							temp = data_TEMP*f/e;
							currentData.temperature = temp;
							if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
								SET_data_TEMP = data_TEMP;
						}

						else if (sensor_no==2) {
							f = configData.calibrationConfig.TEMP_Span_Cal2;
							e = configData.calibrationConfig.TEMP_Span_Value2;
							temp = data_TEMP*f/e;
							currentData.temperature1 = temp;
							if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {}
							else  SET_data_TEMP = data_TEMP;
						}
	#else
						if 		(sensor_no==3) {
							f = configData.calibrationConfig.TEMP_Span_Cal1;
							e = configData.calibrationConfig.TEMP_Span_Value1;
							temp = data_TEMP*f/e;
							currentData.temperature  = temp;
							if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
								SET_data_TEMP = data_TEMP;
						}
						else if (sensor_no==4) {
							f = configData.calibrationConfig.TEMP_Span_Cal2;
							e = configData.calibrationConfig.TEMP_Span_Value2;
							temp = data_TEMP*f/e;
							currentData.temperature1 = temp;
							if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {}
							else  SET_data_TEMP = data_TEMP;
						}


	#endif
#else	// CH4
						if 		(sensor_no==1) {
							f = configData.calibrationConfig.TEMP_Span_Cal1;
							e = configData.calibrationConfig.TEMP_Span_Value1;
							temp = data_TEMP*f/e;
							currentData.temperature = temp;
							SET_data_TEMP = data_TEMP;
						}
						else if (sensor_no==2) {
							f = configData.calibrationConfig.TEMP_Span_Cal2;
							e = configData.calibrationConfig.TEMP_Span_Value2;
							temp = data_TEMP*f/e;
							currentData.temperature1 = temp;
							SET_data_TEMP = data_TEMP;
						}
						else if (sensor_no==3) {
							f = configData.calibrationConfig.TEMP_Span_Cal3;
							e = configData.calibrationConfig.TEMP_Span_Value3;
							temp = data_TEMP*f/e;
							currentData.temperature3 = temp;
							SET_data_TEMP = data_TEMP;
						}
						else if (sensor_no==4) {
							f = configData.calibrationConfig.TEMP_Span_Cal4;
							e = configData.calibrationConfig.TEMP_Span_Value4;
							temp = data_TEMP*f/e;
							currentData.temperature4 = temp;
							SET_data_TEMP = data_TEMP;
						}
#endif
	                }
	                comm_run_flag = 2;


#ifndef  SENSOR_PH_EC	
					// 센서값이 너무작거나 너무 크면, 센서점검 에러 표시함
					if (sensor_no==1) {


						// value 가 0과, 14.0 이상이면 에러로 하자 20241118 여기아님, 위에있음
						if (currentData.S1mV<4.0 || currentData.S1mV>=25000) {
							Sensor_State1=SENSOR_ERROR1;
						}
						else { //if (Sensor_State!=SENSOR_ERROR3)	
							Sensor_State1=SENSOR_OK;
						}
						Sensor1_OK_TIME=10;	// 10=10sec 

					}
					else if (sensor_no==2) {

						// 센서이상 
						if (currentData.S2mV<S2_ERROR_LAMP_LOW) {
							Sensor_State2=SENSOR_ERROR1;
						}
						// 램프이상
						else if (currentData.S2mV<S2_ERROR_LAMP_HIGH) {
							Sensor_State2=SENSOR_ERROR2;
						}
						// 센서이상 
						else if (currentData.S2mV<S2_ERROR_HIGH) {
							Sensor_State2=SENSOR_ERROR1;
						}
						// 센서이상 MAX
						else if (currentData.S2mV>S2_ERROR_MAX_HIGH) {
							Sensor_State2=SENSOR_ERROR1;
						}
						else {//if (Sensor_State!=SENSOR_ERROR3)	
							Sensor_State2=SENSOR_OK;
						}
						Sensor2_OK_TIME=10;	// 10=10sec


					}

#else
					// 센서값이 너무작거나 너무 크면, 센서점검 에러 표시함
					if (sensor_no==3) {
						if (currentData.S1PPM<=S1_PH_ERROR_LOW || currentData.S1PPM>=S1_PH_ERROR_HIGH) {
						//if (currentData.comm_S1PPM<S3_ERROR_LOW || currentData.comm_S1PPM>S3_ERROR_HIGH) {
							Sensor_State1=SENSOR_ERROR1;
						}
						else { //if (Sensor_State!=SENSOR_ERROR3)	
							Sensor_State1=SENSOR_OK;
						}
						Sensor1_OK_TIME=10;	// 10=10sec

					}
					else if (sensor_no==4) {
						if (currentData.S2PPM<S2_EC_ERROR_LOW || currentData.S2PPM>=S2_EC_ERROR_HIGH) {
						//if (currentData.comm_S2NTU<S4_ERROR_LOW || currentData.comm_S2NTU>S4_ERROR_HIGH) {
							Sensor_State2=SENSOR_ERROR1;
						}
						else { //if (Sensor_State!=SENSOR_ERROR3)	
							Sensor_State2=SENSOR_OK;
						}
						Sensor2_OK_TIME=10;	// 10=10sec
					}

#endif

					ComCouter  = 0;
	   				ComCouter2 = 0;
				}	
            } 
		}

		if(rx1Size == 32)	rx1Size = 0;
	}
}

void USART2_IRQHandler(void)
{
		if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) 
	{
		USART2->SR &= USART2->SR&(~0x40);

		if(tx2Count < tx2Size)
			USART2->DR = tx2Buffer[tx2Count++];
		else
		{
			USART2->CR1 &= ~0x40;		
		}
	}
	else
	{
		USART2->SR = (USART2->SR)&(~((USART2->SR)&0X20));
		rx2Buffer[rx2Size] = USART2->DR;
		++rx2Size;
		rx2HandlerCount  = 0;
		if(rx2Size == 32)
			rx2Size = 0;
	}
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_TC) != RESET) 
	{
		USART3->SR &= USART3->SR&(~0x40);

		if(tx3Count < tx3Size)
			USART3->DR = tx3Buffer[tx3Count++];
		else
		{
			USART3->CR1 &= ~0x40;		
			com485State = 3;
			/* RE/DE: TC(Transmission Complete) 직후 수신 모드 전환 - 단일 트랜시버에서 타이밍 충돌 방지 */
			RS485_DRIVE_LOW;
		}
	}
	else
	{
		USART3->SR = (USART3->SR)&(~((USART3->SR)&0X20));
		rx3Buffer[rx3Size] = USART3->DR;

		if (rx3Size==0 && rx3Buffer[rx3Size]==0) ;	// 처음 데이터가 0이면 안받음...
		else rx3Size++;

		rx3HandlerCount  = 0;
		if(rx3Size == 32)
			rx3Size = 0;
	}
}


