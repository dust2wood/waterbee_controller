#include "Main.h"

const uint32_t Device_Selector = SENSOR_1_MODE;
char strBuffer[35];

uint32_t state = 0, subState = 0;
uint16_t adcBuffer[ADCBUFFER_SIZE] = {0,};
uint16_t testImageCount = 0;
struct tm prevTime, currentTime, tempConfigTime;

uint32_t cursor = 0, cursorMax = 0, cursorWidth = 0;

DataSet currentData;
ConfigSet configData, tempConfigData; 

uint32_t adjdefault4mA = 600;
uint32_t adjdefault20mA = 5500;
uint32_t campare_4mA = 0;
uint32_t campare_20mA = 0;

uint16_t adj_run_flag = 0;

uint32_t compareValue = 0;
int32_t compareSignValue = 0;
int32_t compareSigntemp = 0;

uint16_t wash_run_flag  = 0;
uint16_t Relay1_run_on_flag = 0;

uint16_t wash_run_flag2  = 0;
uint16_t Relay2_run_on_flag = 0;

uint16_t wash_run_flag3  = 0;
uint16_t Relay3_run_on_flag = 0;

uint16_t sd_flag  = 0;
uint16_t Alarm_OP_flag = 0;

uint32_t WashCycleVaule1  = 0;
uint32_t Run_ON_RelayVaule1 = 0;

uint32_t WashCycleVaule3  = 0;
uint32_t Run_ON_RelayVaule3 = 0;

CalibrationLogValue calibLog[4];

uint32_t  trendValue[2][61]={{0,},{0,}};
uint32_t  Mesure_Data_Value[3][61]={{0,},{0,},{0,}};
uint32_t  Filter_Data_Value[2][100]={{0,},{0,}};

uint32_t PPmSD_card_Data [60] = {0,};
uint32_t TEMP_card_Data [60]  = {0,};

unsigned char LogData [111] = {0,};
uint16_t LogDataCounter = 0;

int temp_year_mask = 0;
int32_t temp_vaule_mask = 0;

uint32_t testvaule  = 1000;

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

uint16_t tp_counter= 0;
uint16_t tx_trans_flag = 0;


extern uint16_t Counter10msec;
extern uint16_t Comm_time_flag;

int main(void)
{
	Initialize();
	SysIND_LED();
	SystemSetting();
	bufferData_reset_function();
  	Update_Cal_Logdata();	
	configData.adjustConfig.filter_update_flag = 1;

	while (1)
	{		
		// STATE, LCD OUT
		StateHandler();		

		// SENSOR INPUT
		SensorComHandler();				

		// 4-20mA ???
		Update_CurrentTrans ();

		// RELAY1 ON/OFF
		Wash1_Run_Handler();	

		// RELAY3 ON/OFF
		Set_Relay_OP_Run_Handler();		

		// WARNING
		Warning_Process_function();

		// 232		
		if(configData.modbusConfig.mode == 0 && currentData.holdState ==0) {
	       Modbus232Handler();
    	}
		// 485 
		else if(configData.modbusConfig.mode == 1 && currentData.holdState ==0) {		
			Modbus485Handler();				
		}
			
		// CALIBRATION. 보정 매뉴				
		if(manual_cal_flag == 1) 
		{ 
    	   	if((currentData.Device_Selector_Mode&SENSOR_1_MODE) && (req_ppm_com_flag == 0))	{			
	           SendCalData(Tx_S1manualCal);      	
			}
			else if(currentData.Device_Selector_Mode&SENSOR_2_MODE && tx_trans_flag ==0)	{
			   SendCalData(Tx_S2manualCal);  
				 tx_trans_flag = 1; 
	       	}
			if(state == 0 ){ DrawTextsize96(40, 70, TEXT96_MANUAL_CALIBRATION, DRAW_IMAGE_ENABLE); }
		       Deley_10msec(10000);	
			 
			if((currentData.Device_Selector_Mode&SENSOR_1_MODE) && (req_ppm_com_flag == 1))	{			
				if(req_ppm_com_flag == 1 && req_current_com_flag == 0)  
			  	{          	 
           			SendCalDataCurrent(SendParmeter_Current);
	         	}
			 	Deley_10msec(10000);	
			  	if(req_current_com_flag == 1 && req_ppm_com_flag == 1)
			 	{
         			manual_cal_flag = 0; 
				 	req_ppm_com_flag = 0;
				 	req_current_com_flag = 0;
       			}		
			}
		  	else if (currentData.Device_Selector_Mode&SENSOR_2_MODE){
			 	if( req_ppm_com_flag == 1)
			 	{
					tx_trans_flag = 0;
         			manual_cal_flag = 0; 
				 	req_ppm_com_flag = 0;
				 	req_current_com_flag = 0;
       			}		
		 	}			 
    	}
		
		// TEMP. CAL 온도 보정
		if (manual_cal_temp_flag == 1) 
		{			  
			SendCalDataTemp(Tx_Adj_TempertureVaule);
			if(state == 0 ){ DrawTextsize96(40, 70, TEXT96_TEMP_CALIBRATION, DRAW_IMAGE_ENABLE); }
			Deley_10msec(10000);			
			if(req_temp_com_flag == 1)
			{					 
				manual_cal_temp_flag = 0; 
				req_temp_com_flag = 0;
			}
    	}
		
		// ZERO CAL   영점 조정
		if(zero_cal_update_flag == 1 && manual_cal_flag == 0) 
		{
			if((currentData.Device_Selector_Mode&SENSOR_1_MODE) && (req_zero_ppm_com_flag == 0))	{			
				SendCalZeroData(Tx_S1ZeroCal);      	
			}
			else if((currentData.Device_Selector_Mode&SENSOR_2_MODE) && (req_zero_ppm_com_flag == 0) && (tx_trans_flag ==0))	{			
				SendCalZeroData(Tx_S2ZeroCal);  
				tx_trans_flag = 1;          
			}
		
			if(state == 0 ){ DrawTextsize96(40, 70, TEXT96_ZERO_CALIBRATION, DRAW_IMAGE_ENABLE); }
			Deley_10msec(10000);	
			
			if(currentData.Device_Selector_Mode&SENSOR_1_MODE) {
				if(req_zero_ppm_com_flag == 1 && req_zero_current_com_flag == 0) 
				{           	 
					SendCalZeroDataCurrent(SendParmeter_Zero_Current);
				}
				Deley_10msec(10000);
				if(req_zero_ppm_com_flag == 1 && req_zero_current_com_flag == 1)
				{
					zero_cal_update_flag = 0;
					req_zero_current_com_flag = 0;
					req_zero_ppm_com_flag = 0;
				}			 			 
			}
			else if (currentData.Device_Selector_Mode&SENSOR_2_MODE) {
				if(req_zero_ppm_com_flag == 1)
				{
					zero_cal_update_flag = 0;
					req_zero_current_com_flag = 0;
					req_zero_ppm_com_flag = 0;
					tx_trans_flag = 0;
				}			 	
			}
	    }
		
		// WRITE SD
   		if(MicrosdCard_Run_flag == 1 && sd_dec_flag == 1)
		{
			MicroSD_Write();
       		MicrosdCard_Run_flag = 0;
    	}

  	}//while

}

//=======================================================

uint16_t delay_temp;


void Deley_10msec (uint16_t Time)
{
	int i = 0;
	 for (i = 0 ; i< Time ; i++)
	{
		 delay_temp = 1;
  }
}

void SysIND_LED (void)
{	
	 if( ReadJumper == 1)
	 {
         INDLED_ON; 	   
		     currentData.Device_Selector_Mode = SENSOR_1_MODE;		    
   }
	 else if (  ReadJumper == 0 )
	 {
		      INDLED_OFF;   
          currentData.Device_Selector_Mode = SENSOR_2_MODE;	
   }  	
}

void ReadSenosr2ADC (void)
{		    	
	 adcBuffer[0] =  ADC_GetConversionValue(ADC1);
}


void Update_Cal_Logdata (void)
{
	uint16_t i;
	
	EEpromToMpu(0,111);	
	
	for (i= 0 ; i < 111 ; i++)
	   {
	    LogData[i] = EEpromdata.ReadEEpromData_Buffer[i];                  
     }		 

     caldisp_counter = 0;
     caldisp_array_counter = 0;		 
}

void bufferData_reset_function (void)
{
	int i = 0;
	int j = 0;
	for(i = 0 ; i < 2 ; i++)
	{
		for (j = 0; j < 60 ; j++)
		{       
       trendValue[i][j]=0x00;
       Mesure_Data_Value[i][j]= 0xFFFF;
    }
  }
	
	 for (j = 0 ; j<60 ; j++)
	 {
       PPmSD_card_Data [j] = 0x00;
  }
}

uint32_t warning_counter = 0;
uint32_t warning_counter2 = 0;
uint16_t zero_Alarm_sec = 0;
uint16_t zero_Alarm_min = 0;
uint16_t zero_error_flag = 0;

void Warning_Process_function(void)
{
   if(Alarm_OP_flag == 0 && configData.relayConfig.relay2AlarmOFF == 0)
	 {
		  if(currentData.Device_Selector_Mode&SENSOR_1_MODE ) {
         if(currentData.S1PPM > (configData.alarmConfig.highLimit*0.1)) {warning_counter++;}
			   else { if(warning_counter != 0) warning_counter--;}
			   if(currentData.S1PPM < (configData.alarmConfig.lowLimit*0.1))  {warning_counter2++;}
			   else { if(warning_counter2 != 0) warning_counter2--;}	
		}
		  else if(currentData.Device_Selector_Mode&SENSOR_2_MODE ) {
         if(currentData.S2PPM > (configData.alarmConfig.highLimit)) {warning_counter++;}
			   else { if(warning_counter != 0) warning_counter--;}
			   if(currentData.S2PPM < (configData.alarmConfig.lowLimit))  {warning_counter2++;}
		     else { if(warning_counter2 != 0) warning_counter2--;}	
    }
		
			
			if( currentData.S1PPM == 0 )
			{   //50msec
          if(zero_Alarm_Time_couter == 20) 	{ zero_Alarm_sec++;}
          if(	zero_Alarm_sec == 60) { zero_Alarm_min ++; zero_Alarm_sec = 0; zero_Alarm_Time_couter = 0; }
  				if(configData.alarmConfig.zeroAlarm == zero_Alarm_min)
					{
             zero_error_flag = 1;
          }						
      }
		  else {
           zero_Alarm_Time_couter = 0; zero_Alarm_sec = 0;  zero_Alarm_min = 0; zero_error_flag = 0;
      }
			
			if( warning_counter != 0 || warning_counter2 != 0 || zero_error_flag == 1)
			{
				    if(state == 0){ DrawIcon(ICON_CAUTION, DRAW_IMAGE_ENABLE);  
						}
						else { DrawIcon(ICON_CAUTION, DRAW_IMAGE_DISABLE);                              
            }
						if(configData.relayConfig.relay2AutoAlarm == 1)
						{
                    RELAY2_ON;
            }
      }
			else {if(state == 0){ DrawIcon(ICON_CAUTION, DRAW_IMAGE_DISABLE);} 
			         if( configData.relayConfig.relay2AutoAlarm == 1)
						 {
                    RELAY2_OFF;
             }
      }			
   }
	 else if(Alarm_OP_flag == 0 && configData.relayConfig.relay2AlarmOFF == 1){		 
		  RELAY2_OFF;
		  DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
		  configData.relayConfig.relay2AutoAlarm = 0;
		  warning_counter = 0;
		  warning_counter2 = 0;
		  zero_error_flag = 0;
		  Alarm_OP_flag = 1; 
  }
	else
	{
      RELAY2_OFF;
		  DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
		  warning_counter = 0;
		  warning_counter2 = 0;
		  zero_error_flag = 0;
		  Alarm_OP_flag = 1; 
  }
}



void Wash1_Run_Handler (void)
{
   WashCycleVaule1 = (configData.relayConfig.relay1WashCycle*60)/0.05;  
   Run_ON_RelayVaule1 = (configData.relayConfig.relay1WashTime*60)/0.05; 
	
	 if((WashCycleVaule1 < flag_wash1_time_counter) && (wash_run_flag == 1 ))
	 {
      flag_wash1_time_counter = 0;
		  wash_run_flag = 0;
		  Relay1_run_on_flag = 1;
   }
	 else{

   }
	 
	 if((Run_ON_RelayVaule1 > flag_wash1_relay_time_counter) && (Relay1_run_on_flag == 1))
	 {
		 if(state == 0){
			 if(currentData.Device_Selector_Mode&SENSOR_1_MODE ) { DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_ENABLE);}
			 else if (currentData.Device_Selector_Mode&SENSOR_2_MODE) {DrawTextsize120(20, 140, TEXT120_WASHING, DRAW_IMAGE_ENABLE);}
		 }
		 else{
			RedrawValue(); 
     }
      RELAY1_ON;
   }
	 else if((Run_ON_RelayVaule1 < flag_wash1_relay_time_counter) && (Relay1_run_on_flag == 1)){	
		  
		 if(state == 0){
		  if(currentData.Device_Selector_Mode&SENSOR_1_MODE ) { DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_DISABLE);}
			 else if (currentData.Device_Selector_Mode&SENSOR_2_MODE) {DrawTextsize120(20, 140, TEXT120_WASHING, DRAW_IMAGE_DISABLE);}
		 }
		 else{
			RedrawValue(); 
     }
		  
		  RedrawValue();
      RELAY1_OFF;
		  Relay1_run_on_flag = 0;
		  flag_wash1_relay_time_counter = 0;
		  wash_run_flag = 1;
		 
   }
	 
}

void Set_Relay_OP_Run_Handler (void)
{
   WashCycleVaule3 = (configData.relayConfig.relay3WashCycle*60)/0.05;  
   Run_ON_RelayVaule3 = (configData.relayConfig.relay3WashTime*60)/0.05; 
	
	 if((WashCycleVaule3 < flag_wash3_time_counter) && (wash_run_flag3 == 1 ))
	 {
      flag_wash3_time_counter = 0;
		  wash_run_flag3 = 0;
		  Relay3_run_on_flag = 1;
   }
	 else{

   }
	 
	 if((Run_ON_RelayVaule3 > flag_wash3_relay_time_counter) && (Relay3_run_on_flag == 1))
	 {
		 if(state == 0){
		 // DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_ENABLE);
		 }
		 else{
			RedrawValue(); 
     }
      RELAY3_ON;
   }
	 else if((Run_ON_RelayVaule3 < flag_wash3_relay_time_counter) && (Relay3_run_on_flag == 1)){	
		  
		 if(state == 0){
		  //DrawTextsize120(40, 140, TEXT120_WASHING, DRAW_IMAGE_DISABLE);
		 }
		 else{
			RedrawValue(); 
     }
		  
		  RedrawValue();
      RELAY3_OFF;
		  Relay3_run_on_flag = 0;
		  flag_wash3_relay_time_counter = 0;
		  wash_run_flag3 = 1;
		 
   }
	 
}


void SystemSetting(void)
{
	configData = *((ConfigSet*)(SAVEADDR_CONFIG_BASE));
	
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE ) {configData.outputConfig.output20mA = 2000;
     Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));}
	else if( currentData.Device_Selector_Mode == SENSOR_2_MODE) {configData.outputConfig.output20mA = 20000;
     Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
    }
	if(configData.outputConfig.output4mA == 0xFFFFFFFF)
		FactoryReset();
	
	switch(configData.modbusConfig.baudrate)
	{
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
	
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
    DrawTextsize96(5, 5, TEXT96_CI, DRAW_IMAGE_ENABLE);	}
	else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
	  DrawTextsize96(5, 5, TEXT96_CI3, DRAW_IMAGE_ENABLE);
	}	

	DrawUnit(300, 5, UNIT_RX, DRAW_IMAGE_ENABLE);
	DrawUnit(335, 5, UNIT_TX, DRAW_IMAGE_ENABLE);
	TFT_Fill(0, 41, 480, 43, WHITE);	
	
	currentTime = Time_GetCalendarTime();
	state = STATE_MAIN;
}

void SD_Detect_Draw (void)
{	  
	 sd_dec_flag = SD_Detect();	
	 if(sd_dec_flag == 0x00) { DrawIcon(ICON_SDCARD, DRAW_IMAGE_DISABLE);}
	 else if (sd_dec_flag == 0x01) { DrawIcon(ICON_SDCARD, DRAW_IMAGE_ENABLE);}
}



void Alarm_Icon_Draw (void)   
{
  if(configData.alarmConfig.highLimit == 0 && configData.alarmConfig.lowLimit==0 && configData.alarmConfig.zeroAlarm == 0 && configData.relayConfig.relay2AutoAlarm == 0 )
	{
     DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
		 warning_counter = 0;
		 warning_counter2 = 0;
		 zero_error_flag = 0;
		 Alarm_OP_flag = 1;
  }
	
 if((configData.relayConfig.relay2AutoAlarm == 1 )&&(configData.alarmConfig.highLimit != 0 || configData.alarmConfig.lowLimit !=0 || configData.alarmConfig.zeroAlarm != 0))
	{		
      DrawIcon(ICON_BELL, DRAW_IMAGE_ENABLE);
		  Alarm_OP_flag = 0;
		
  }
	else{
     DrawIcon(ICON_BELL, DRAW_IMAGE_DISABLE);
		 warning_counter = 0;
		 warning_counter2 = 0;
		 zero_error_flag = 0;
		 Alarm_OP_flag = 1; 
  }
}


//====================================
// STATE, LCD OUT
//====================================
void StateHandler(void)
{
	SD_Detect_Draw();
	Alarm_Icon_Draw();
	
	switch(state)
	{
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
		State_ConfigOutput();
	break;
	case STATE_CONFIG_COMM:
		State_ConfigComm();
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
	case STATE_CALIB_ZERO:
		State_CalibZero();
	break;
	case STATE_CALIB_MANUAL:
		State_CalibManual();
	break;
	case STATE_CALIB_AUTO:  
		State_CalibAuto();
	break;
	case STATE_CALIB_S2_CYCLE:
		State_CalibS2Cycle();
	break;
	case STATE_CALIB_LOG:
		State_CalibLog();
	break;
	
	case STATE_CONFIG_RELAY_RELAY1:
		State_ConfigRelayRelay1();
	break;
	case STATE_CONFIG_RELAY_RELAY2:
		State_ConfigRelayRelay2();
	break;
	case STATE_CONFIG_RELAY_RELAY3:
		State_ConfigRelayRelay3();
	break;
	case STATE_CONFIG_ADJUST_GRADIENT:
		State_ConfigAdjustGradient();
	break;
	case STATE_CONFIG_ADJUST_OFFSET:
		State_ConfigAdjustOffset();
	break;
	case STATE_CONFIG_ADJUST_FILTER:
		State_ConfigAdjustFilter();
	break;
	case STATE_CONFIG_ADJUST_FACTORYRESET:
		State_ConfigAdjustFactoryReset();
	break;
	
	case STATE_CONFIG_RELAY_RELAY1_WASH:  
		State_ConfigRelayRelay1Wash();
	break;
	case STATE_CONFIG_RELAY_RELAY1_ALARM:
		State_ConfigRelayRelay1manualOP();
	break;
	case STATE_CONFIG_RELAY_RELAY2_WASH:  
           State_ConfigRelayRelay2AutoAlarm();
	break;
	case STATE_CONFIG_RELAY_RELAY2_ALARM: 
         	State_ConfigRelayRelay2AlarmOFF();
	break;
	case STATE_CONFIG_RELAY_RELAY3_WASH: 
		State_ConfigRelayRelay3SetOP();
	break;
	case STATE_CONFIG_RELAY_RELAY3_ALARM: 
		State_ConfigRelayRelay3manualOP();
	break;	
	}
}



//====================================
// STATE NAIN,
//====================================
void State_Main(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
	break;
	case 1:
		RedrawValue();
		subState = 2;
	break;
	case 2:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			if(CheckButton()==BUTTON_MENU)
			{
				subState = 0;
				cursor = 0;
				MoveSelectedState();
			}
			else
				RedrawTime();
		}
	break;
	}
}


void State_Menu(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
			case BUTTON_HOME:
			case BUTTON_BACK:
				subState = 0;
				state = STATE_MAIN;
			break;
			case BUTTON_LEFT:
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
				if(cursor == 0)
					cursor = 4;
				else
					--cursor;
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
				++cursor;
				cursor = cursor % 5;
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
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




void State_Config(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
			case BUTTON_HOME:
				subState = 0;
				state = STATE_MAIN;
			break;
			case BUTTON_BACK:
				subState = 0;
				MoveBackState();
			break;
			case BUTTON_LEFT:
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
				if(cursor == 0)
					cursor = 4;
				else
					--cursor;
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, BLACK);
				++cursor;
				cursor = cursor % 5;
				DrawLineRectangle(0+(cursor*96), 238, 96+(cursor*96), 272, YELLOW);
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
void State_Calib(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
		if(currentData.Device_Selector_Mode==SENSOR_1_MODE || currentData.Device_Selector_Mode==SENSOR_2_MODE)
		{
			cursorMax = 4;
			cursorWidth = 120;
		}
		else
		{
			cursorMax = 5;
			cursorWidth = 96;
		}
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
			case BUTTON_HOME:
				subState = 0;
				state = STATE_MAIN;
			break;
			case BUTTON_BACK:
				subState = 0;
				MoveBackState();
			break;
			case BUTTON_LEFT:
				DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, BLACK);
				if(cursor == 0)
					cursor = cursorMax - 1;
				else
					--cursor;
				DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, BLACK);
				++cursor;
				cursor = cursor % cursorMax;
				DrawLineRectangle(0+(cursor*cursorWidth), 238, cursorWidth+(cursor*cursorWidth), 272, YELLOW);
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

void Trend_Data_move (void)
{
	int i = 0;
	if(comm_run_flag != 1)
	{		
		 comm_run_flag = 7; 
		if(Mesure_Data_Value[0][59] != 0xFFFF)
		{
		 for(i =0 ; i < 60 ; i++)
		 {
       trendValue[0][i]= Mesure_Data_Value[0][i];
     }     
	  }
		comm_run_flag = 0;
  }  
}

void State_Trend(void) 
{
	switch(subState)
	{
	case 0:
		Trend_Data_move();
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
		compareValue = 0;
		RedrawValue();
		DrawGraph(cursor);
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(cursor == 0)
				{
					ClearGraphArea();
					cursor = 60;
					TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, RED);
				}
				else
				{
					TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, BLACK);
					--cursor;
					TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, RED);
				}
				DrawGraph(cursor);
				DrawGraphText(cursor);
			break;
			case BUTTON_RIGHT:
				if(cursor == 60)
				{
					ClearGraphArea();
					cursor = 0;
					TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, RED);
				}
				else
				{
					TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, BLACK);
					++cursor;
					TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, RED);
				}
				DrawGraph(cursor);
				DrawGraphText(cursor);
			break;
			case BUTTON_UP:
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					compareValue ^= 1;
					ClearGraphArea();
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
void State_Alarm(void) 
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(cursor == 0)					cursor = 4;
				else					--cursor;
				DrawAlarmConfig(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawAlarmConfig(cursor, WHITE);
				++cursor;	cursor = cursor % 5;
				DrawAlarmConfig(cursor, YELLOW);
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode&SENSOR_1_MODE )
       	{
				switch(cursor)
				{
				case 0:
					if(tempConfigData.alarmConfig.highLimit + 100 <= 9999)
						tempConfigData.alarmConfig.highLimit += 100;
				break;
				case 1:
					if(tempConfigData.alarmConfig.highLimit  < 9999)
						++tempConfigData.alarmConfig.highLimit;
				break;
				case 2:
					if(tempConfigData.alarmConfig.lowLimit + 100 <= 9999)
						tempConfigData.alarmConfig.lowLimit += 100;
				break;
				case 3:
					if(tempConfigData.alarmConfig.lowLimit  < 9999)
						++tempConfigData.alarmConfig.lowLimit;
				break;
				case 4:
					if(tempConfigData.alarmConfig.zeroAlarm < 999)
						++tempConfigData.alarmConfig.zeroAlarm;
				break;
				}
		  }
			else if (currentData.Device_Selector_Mode&SENSOR_2_MODE )
			{
         switch(cursor)
				{
				case 0:
					if(tempConfigData.alarmConfig.highLimit + 100 <= 99999)
						tempConfigData.alarmConfig.highLimit += 100;
				break;
				case 1:
					if(tempConfigData.alarmConfig.highLimit  < 99999)
						++tempConfigData.alarmConfig.highLimit;
				break;
				case 2:
					if(tempConfigData.alarmConfig.lowLimit + 100 <= 99999)
						tempConfigData.alarmConfig.lowLimit += 100;
				break;
				case 3:
					if(tempConfigData.alarmConfig.lowLimit  < 99999)
						++tempConfigData.alarmConfig.lowLimit;
				break;
				case 4:
					if(tempConfigData.alarmConfig.zeroAlarm < 999)
						++tempConfigData.alarmConfig.zeroAlarm;
				break;
				}
      }
			DrawAlarmConfig(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode&SENSOR_1_MODE )
       	{
				switch(cursor)
				{
				case 0:
					if(tempConfigData.alarmConfig.highLimit >= 100)
						tempConfigData.alarmConfig.highLimit -= 100;
				break;
				case 1:
					if(tempConfigData.alarmConfig.highLimit  > 0)
						--tempConfigData.alarmConfig.highLimit;
				break;
				case 2:
					if(tempConfigData.alarmConfig.lowLimit >= 100)
						tempConfigData.alarmConfig.lowLimit -= 100;
				break;
				case 3:
					if(tempConfigData.alarmConfig.lowLimit  > 0)
						--tempConfigData.alarmConfig.lowLimit;
				break;
				case 4:
					if(tempConfigData.alarmConfig.zeroAlarm > 0)
						--tempConfigData.alarmConfig.zeroAlarm;
				break;
				}
       }
			 	if(currentData.Device_Selector_Mode&SENSOR_2_MODE )
       	{
					switch(cursor)
				{
				case 0:
					if(tempConfigData.alarmConfig.highLimit >= 100)
						tempConfigData.alarmConfig.highLimit -= 100;
				break;
				case 1:
					if(tempConfigData.alarmConfig.highLimit  > 0)
						--tempConfigData.alarmConfig.highLimit;
				break;
				case 2:
					if(tempConfigData.alarmConfig.lowLimit >= 100)
						tempConfigData.alarmConfig.lowLimit -= 100;
				break;
				case 3:
					if(tempConfigData.alarmConfig.lowLimit  > 0)
						--tempConfigData.alarmConfig.lowLimit;
				break;
				case 4:
					if(tempConfigData.alarmConfig.zeroAlarm > 0)
						--tempConfigData.alarmConfig.zeroAlarm;
				break;
				}
			}
				DrawAlarmConfig(cursor, YELLOW);
			break;
			case BUTTON_ENTER:
				if(configData.alarmConfig.highLimit != tempConfigData.alarmConfig.highLimit || configData.alarmConfig.lowLimit != tempConfigData.alarmConfig.lowLimit
				||configData.alarmConfig.zeroAlarm != tempConfigData.alarmConfig.zeroAlarm)
				{
					configData.alarmConfig = tempConfigData.alarmConfig;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_Diagnostic(void)  
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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


void State_ConfigOutput(void)
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
	  output_temp4 = (int32_t)tempConfigData.outputConfig.outputReal4mA - (int32_t)adjdefault4mA;
	  output_temp20 = (int32_t)tempConfigData.outputConfig.outputReal20mA - (int32_t)adjdefault20mA;
	  if(output_temp4 < 0 )
		{
        tempConfigData.outputConfig.outputReal4mA = 0;
    }
		else 
		{
          tempConfigData.outputConfig.outputReal4mA = (uint32_t)output_temp4;
    }
	  if(output_temp20 < 0 )
		{
        tempConfigData.outputConfig.outputReal20mA = 0;
    }
		else 
		{
          tempConfigData.outputConfig.outputReal20mA = (uint32_t)output_temp20;
    }
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
		compareValue = currentData.holdState;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(cursor == 0)					cursor = 8;
				else					--cursor;
				DrawOutputConfig(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawOutputConfig(cursor, WHITE);
				++cursor;			cursor = cursor % 9;
				DrawOutputConfig(cursor, YELLOW);
			break;
			case BUTTON_UP:
				switch(cursor)
				{
				case 0:
					
				break;
				case 1:
					
				break;
				case 2:
					
				break;
				case 3:
					
				break;
				case 4:
					   output4ma_flag  = 1;
				     output20ma_flag = 0;
					if(tempConfigData.outputConfig.outputReal4mA +100 <= 99999)
						tempConfigData.outputConfig.outputReal4mA += 100;
				break;
				case 5:
					   output4ma_flag  = 1;
				     output20ma_flag = 0;
					if(tempConfigData.outputConfig.outputReal4mA  < 99999)
						++tempConfigData.outputConfig.outputReal4mA;
				break;
				case 6:
					   output4ma_flag  = 0;
				     output20ma_flag = 1;
					if(tempConfigData.outputConfig.outputReal20mA +100 <= 99999)
						tempConfigData.outputConfig.outputReal20mA += 100;
				break;
				case 7:
					   output4ma_flag  = 0;
				     output20ma_flag = 1;
					if(tempConfigData.outputConfig.outputReal20mA  < 99999)
						++tempConfigData.outputConfig.outputReal20mA;
				break;
				case 8:
					compareValue ^= 1;
				break;
				}
				DrawOutputConfig(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				switch(cursor)
				{
				case 0:
					
				break;
				case 1:
					
				break;
				case 2:
					
				break;
				case 3:
				
				break;
				case 4:
					   output4ma_flag  = 1;
				     output20ma_flag = 0;
					if(tempConfigData.outputConfig.outputReal4mA >= 100)
						tempConfigData.outputConfig.outputReal4mA -= 100;
				break;
				case 5:
					   output4ma_flag  = 1;
				     output20ma_flag = 0;
					if(tempConfigData.outputConfig.outputReal4mA  > 0)
						--tempConfigData.outputConfig.outputReal4mA;
				break;
				case 6:
					   output4ma_flag  = 0;
				     output20ma_flag = 1;
					if(tempConfigData.outputConfig.outputReal20mA >= 100)
						tempConfigData.outputConfig.outputReal20mA -= 100;
				break;
				case 7:
					   output4ma_flag  = 0;
				     output20ma_flag = 1;
					if(tempConfigData.outputConfig.outputReal20mA  > 0)
						--tempConfigData.outputConfig.outputReal20mA;
				break;
				case 8:
					compareValue ^= 1;
				break;
				}
				DrawOutputConfig(cursor, YELLOW);
			break;
			case BUTTON_ENTER:
				if(configData.outputConfig.output4mA != tempConfigData.outputConfig.output4mA || configData.outputConfig.output20mA != tempConfigData.outputConfig.output20mA
				||configData.outputConfig.outputReal4mA != tempConfigData.outputConfig.outputReal4mA||configData.outputConfig.outputReal20mA != tempConfigData.outputConfig.outputReal20mA)
				{
					
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				currentData.holdState = compareValue;  
				subState = 0;
				state = STATE_MAIN;
				output4ma_flag  = 0;
				output20ma_flag = 0;
				TIM8_Chage_Duty_Channel(1,0); 
				
			break;
			}
			adj_Current_Cal();
		}
	break;
	}
}

void adj_Current_Cal (void)
{
  if(output4ma_flag  == 1 && output20ma_flag == 0)				     
	{		  
		  if(campare_4mA != tempConfigData.outputConfig.outputReal4mA)
			{
        configData.outputConfig.outputReal4mA = adjdefault4mA+tempConfigData.outputConfig.outputReal4mA;
      }      
		  TIM8_Chage_Duty_Channel(1,configData.outputConfig.outputReal4mA);
			campare_4mA = configData.outputConfig.outputReal4mA;
  }
	else if(output4ma_flag  == 0 && output20ma_flag == 1)
	{
      if(campare_20mA != tempConfigData.outputConfig.outputReal20mA)
			{
        configData.outputConfig.outputReal20mA = adjdefault20mA+tempConfigData.outputConfig.outputReal20mA;
      }      
		  TIM8_Chage_Duty_Channel(1,configData.outputConfig.outputReal20mA);
			campare_20mA = configData.outputConfig.outputReal20mA;
  }	
	else {
         campare_4mA = 0;
         campare_20mA = 0;
  }	
}

void State_ConfigComm(void)   
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(cursor==0)
					cursor = 3;
				else
					--cursor;
				DrawCommConfig(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawCommConfig(cursor, WHITE);
				++cursor;		cursor = cursor % 4;
				DrawCommConfig(cursor, YELLOW);
			break;
			case BUTTON_UP:
				switch(cursor)
				{
				case 0:
					tempConfigData.modbusConfig.mode ^= 1;  
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
				switch(cursor)
				{
				case 0:
					tempConfigData.modbusConfig.mode ^= 1;
				break;
				case 1:
					if(tempConfigData.modbusConfig.baudrate == 0)
						tempConfigData.modbusConfig.baudrate = 3;
					else
						--tempConfigData.modbusConfig.baudrate;
				break;
				case 2:
					if(tempConfigData.modbusConfig.modbusSlaveAddr == 0)
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
				if(configData.modbusConfig.mode != tempConfigData.modbusConfig.mode || configData.modbusConfig.baudrate != tempConfigData.modbusConfig.baudrate
				||configData.modbusConfig.modbusSlaveAddr != tempConfigData.modbusConfig.modbusSlaveAddr||configData.modbusConfig.databit != tempConfigData.modbusConfig.databit)
				{
					configData.modbusConfig = tempConfigData.modbusConfig;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				  
					switch(configData.modbusConfig.baudrate)
					{
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
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_ConfigRelay(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
			case BUTTON_HOME:
				subState = 0;
				state = STATE_MAIN;
			break;
			case BUTTON_BACK:
				subState = 0;
				MoveBackState();
			break;
			case BUTTON_LEFT:
				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, BLACK);
				if(cursor == 0)					cursor = 2;
				else					--cursor;
				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, BLACK);
				++cursor;				cursor = cursor % 3;
				DrawLineRectangle(0+(cursor*160), 238, 160+(cursor*160), 272, YELLOW);
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
void State_ConfigTime(void)
{
	switch(subState)
	{
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
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(cursor == 0)					cursor = 5;
				else					--cursor;
				DrawTimeConfig(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawTimeConfig(cursor, WHITE);
				++cursor;		cursor = cursor % 6;
				DrawTimeConfig(cursor, YELLOW);
			break;
			case BUTTON_UP:
				switch(cursor)
				{
				case 0:
					if(tempConfigTime.tm_year < 2099)
						++tempConfigTime.tm_year;
				break;
				case 1:
					if(tempConfigTime.tm_mon == 11)						tempConfigTime.tm_mon = 0;
					else						++tempConfigTime.tm_mon;
				break;
				case 2:
					if(tempConfigTime.tm_mday == 31)						tempConfigTime.tm_mday = 1;
					else						++tempConfigTime.tm_mday;
				break;
				case 3:
					if(tempConfigTime.tm_hour == 23)						tempConfigTime.tm_hour = 0;
					else						++tempConfigTime.tm_hour;
				break;
				case 4:
					if(tempConfigTime.tm_min == 59)						tempConfigTime.tm_min = 0;
					else						++tempConfigTime.tm_min;
				break;
				case 5:
					if(tempConfigTime.tm_sec == 59)						tempConfigTime.tm_sec = 0;
					else						++tempConfigTime.tm_sec;
				break;
				}
				DrawTimeConfig(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				switch(cursor)
				{
				case 0:
					if(tempConfigTime.tm_year > 1970)
						--tempConfigTime.tm_year;
				break;
				case 1:
					if(tempConfigTime.tm_mon == 0)						tempConfigTime.tm_mon = 11;
					else						--tempConfigTime.tm_mon;
				break;
				case 2:
					if(tempConfigTime.tm_mday == 1)						tempConfigTime.tm_mday = 31;
					else						--tempConfigTime.tm_mday;
				break;
				case 3:
					if(tempConfigTime.tm_hour == 0)						tempConfigTime.tm_hour = 23;
					else						--tempConfigTime.tm_hour;
				break;
				case 4:
					if(tempConfigTime.tm_min == 0)						tempConfigTime.tm_min = 59;
					else						--tempConfigTime.tm_min;
				break;
				case 5:
					if(tempConfigTime.tm_sec == 0)						tempConfigTime.tm_sec = 59;
					else						--tempConfigTime.tm_sec;
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
void State_ConfigAdjust(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
				if(cursor == 0)
					cursor = 3;
				else
					--cursor;
				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, BLACK);
				++cursor;
				cursor = cursor % 4;
				DrawLineRectangle(0+(cursor*120), 238, 120+(cursor*120), 272, YELLOW);
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

void Save_CalLog_Data(uint16_t Type , int32_t calvaule)  
{  
	    uint16_t address_start = 0;
	    uint16_t upcounter = 0;
	
					LogDataCounter = LogData[110]; 					
					currentTime = Time_GetCalendarTime();
					
					if(LogDataCounter >= 10 )
					{
						 LogDataCounter = 0;
						 LogData[110]= LogDataCounter+1;
						 address_start = LogDataCounter;
          }				
					
					if(LogDataCounter != 0 )
					{
						 LogData[110]= LogDataCounter+1;
						 upcounter = LogDataCounter;
						 LogDataCounter = LogDataCounter * 10;
             LogDataCounter = LogDataCounter+upcounter;
						 address_start = LogDataCounter;
          }
					else {
             LogData[110]= LogDataCounter+1;
						 address_start = LogDataCounter;
						 
          }
					
          LogData[LogDataCounter++] = LogData[110] - 1;
					EEpromdata.WriteEEpromData_Buffer[0] = LogData[110]-1;
					
					temp_year_mask = (currentTime.tm_year & 0x0000FF00)>>8;
					LogData[LogDataCounter++] = (char)temp_year_mask;
					EEpromdata.WriteEEpromData_Buffer[1] = (char)temp_year_mask;
					
					temp_year_mask = (currentTime.tm_year & 0x000000FF);
          LogData[LogDataCounter++] = (char)temp_year_mask;
					EEpromdata.WriteEEpromData_Buffer[2] = (char)temp_year_mask;
					
					LogData[LogDataCounter++] = (char)currentTime.tm_mon;
					EEpromdata.WriteEEpromData_Buffer[3] = (char)currentTime.tm_mon;
					
					LogData[LogDataCounter++] = (char)currentTime.tm_mday;
					EEpromdata.WriteEEpromData_Buffer[4] = (char)currentTime.tm_mday;
					
					LogData[LogDataCounter++] = (char)currentTime.tm_hour;
					EEpromdata.WriteEEpromData_Buffer[5] = (char)currentTime.tm_hour;
					
				  LogData[LogDataCounter++] = (char)currentTime.tm_min;
					EEpromdata.WriteEEpromData_Buffer[6] = (char)currentTime.tm_min;
					
					LogData[LogDataCounter++] = (char)currentTime.tm_sec;
					EEpromdata.WriteEEpromData_Buffer[7] = (char)currentTime.tm_sec;
					
          LogData[LogDataCounter++] = (char)Type;
					EEpromdata.WriteEEpromData_Buffer[8] = (char)Type;
					
					temp_vaule_mask = (calvaule & 0x000000000000FF00)>>8;
					LogData[LogDataCounter++] = (char)temp_vaule_mask;
					EEpromdata.WriteEEpromData_Buffer[9] = (char)temp_vaule_mask;
					temp_vaule_mask = (calvaule & 0x00000000000000FF);
					LogData[LogDataCounter++] = (char)temp_vaule_mask;
					EEpromdata.WriteEEpromData_Buffer[10] = (char)temp_vaule_mask;
					
					MpuToEEprom(EEpromdata.WriteEEpromData_Buffer,address_start,11);					
          
					EEpromdata.WriteEEpromData_Buffer[0] = LogData[110];
					MpuToEEprom(EEpromdata.WriteEEpromData_Buffer,110,1);
					LogDataCounter = 0;
					
					
					
}


void State_CalibZero(void)  
{

	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					if(cursor == 0)						cursor = 3;
					else						--cursor;
				}
				else					cursor ^= 1;
				DrawCalibZero(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawCalibZero(cursor, WHITE);
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					++cursor;		cursor = cursor % 4;
				}
				else					cursor ^= 1;
				DrawCalibZero(cursor, YELLOW);
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
				{
					if(cursor == 0)
					{
						if(tempConfigData.calibrationConfig.S1zeroCal + 100 <= 999)
							tempConfigData.calibrationConfig.S1zeroCal += 10;
					}
					else
					{
						if(tempConfigData.calibrationConfig.S1zeroCal < 999)
							++tempConfigData.calibrationConfig.S1zeroCal;
					}
				}
        else if( currentData.Device_Selector_Mode == SENSOR_2_MODE)	
				{
          if(cursor == 0)
					{
						if(tempConfigData.calibrationConfig.S2zeroCal + 100 <= 9999)
							tempConfigData.calibrationConfig.S2zeroCal += 100;
					}
					else
					{
						if(tempConfigData.calibrationConfig.S2zeroCal < 9999)
							++tempConfigData.calibrationConfig.S2zeroCal;
					}
       }			
				else
				{
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S2zeroCal + 100 <= 9999)
							tempConfigData.calibrationConfig.S2zeroCal += 100;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S2zeroCal < 9999)
							++tempConfigData.calibrationConfig.S2zeroCal;
					break;
					case 2:
						if(tempConfigData.calibrationConfig.S2zeroCal + 100 <= 9999)
							tempConfigData.calibrationConfig.S2zeroCal += 100;
					break;
					case 3:
						if(tempConfigData.calibrationConfig.S2zeroCal < 9999)
							++tempConfigData.calibrationConfig.S2zeroCal;
					break;
					}
				}
	
			DrawCalibZero(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
				{
					if(cursor == 0)
					{
						if(tempConfigData.calibrationConfig.S1zeroCal >= 1)
						{
                tempConfigData.calibrationConfig.S1zeroCal-= 10;
            }
						else if(tempConfigData.calibrationConfig.S1zeroCal <= 0)
						{
                 tempConfigData.calibrationConfig.S1zeroCal-= 10;
            }							
					}
					else
					{
						if(tempConfigData.calibrationConfig.S1zeroCal > -999)
							--tempConfigData.calibrationConfig.S1zeroCal;
					}
				}
				
				else if(currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(cursor == 0)
					{
						if(tempConfigData.calibrationConfig.S2zeroCal >= 1)
						{
                tempConfigData.calibrationConfig.S2zeroCal-= 100;
            }
						else if(tempConfigData.calibrationConfig.S2zeroCal <= 0)
						{
                 tempConfigData.calibrationConfig.S2zeroCal-= 100;
            }							
					}
					else
					{
						if(tempConfigData.calibrationConfig.S2zeroCal > -9999)
							--tempConfigData.calibrationConfig.S2zeroCal;
					}
				}
				
				DrawCalibZero(cursor, YELLOW);
			break;
			case BUTTON_ENTER:
				compareValue = 0;
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE  ){
				
					compareValue = 1;
					if(compareValue == 1)
			  	{
					  Tx_S1ZeroCal = configData.calibrationConfig.S1zeroCal;  
					  configData.calibrationConfig = tempConfigData.calibrationConfig;         			
					  Save_CalLog_Data(TEXT96_ZERO_CALIBRATION,configData.calibrationConfig.S1zeroCal);		
					  Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						Update_Cal_Logdata();
						 zero_cal_update_flag = 1;   
						 Tx_S1ZeroCal = configData.calibrationConfig.S1zeroCal;  
						  if(configData.adjustConfig.filterS1 != 0)
							{
                  SendParmeter_Zero_Current = currentData.filterout_S1Current ;
              }
							else {
                  SendParmeter_Zero_Current = currentData.S1mVolt; 
              }
				   }
          
          }
				else if(currentData.Device_Selector_Mode == SENSOR_2_MODE){
				
					compareValue = 1;
					if(compareValue == 1)
			  	{
						Tx_S2ZeroCal = configData.calibrationConfig.S2zeroCal;  
					  configData.calibrationConfig = tempConfigData.calibrationConfig;         			
					  Save_CalLog_Data(TEXT96_ZERO_CALIBRATION,configData.calibrationConfig.S2zeroCal) ;
					  Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						Update_Cal_Logdata();
						 zero_cal_update_flag = 1;  
             Tx_S2ZeroCal = configData.calibrationConfig.S2zeroCal;   								 
							zero_cal_update_flag_S2 = 1;
						
				   }					
				}				
					else
			{
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


void State_CalibManual(void) 
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					DrawCalibManual(cursor, WHITE);
					if(cursor == 0)						cursor = 4;
					else						--cursor;
					DrawCalibManual(cursor, YELLOW);
				}
				else
				{
					DrawCalibManual(cursor, WHITE);
					if(cursor == 0)						cursor = 1;
					else						--cursor;
					DrawCalibManual(cursor, YELLOW);
				}
			break;
			case BUTTON_RIGHT:
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					DrawCalibManual(cursor, WHITE);
					++cursor;					cursor = cursor % 5;
					DrawCalibManual(cursor, YELLOW);
				}
				else
				{
					DrawCalibManual(cursor, WHITE);
					++cursor;					cursor = cursor % 2;
					DrawCalibManual(cursor, YELLOW);
				}
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
				{
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S1manualCal + 10 <= 999)  
							tempConfigData.calibrationConfig.S1manualCal += 10;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S1manualCal < 999)            
							++tempConfigData.calibrationConfig.S1manualCal;
					break;
					case 2:
						
					break;
					}
				}
				else if( currentData.Device_Selector_Mode == SENSOR_2_MODE)  
				{					
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S2manualCal + 100 <= 99999)
							tempConfigData.calibrationConfig.S2manualCal += 100;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S2manualCal < 9999)
							++tempConfigData.calibrationConfig.S2manualCal;
					break;
					case 2:
						
					break;
					}
				}
				DrawCalibManual(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
				{
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S1manualCal >= 10)
							tempConfigData.calibrationConfig.S1manualCal -= 10;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S1manualCal > 0)
							--tempConfigData.calibrationConfig.S1manualCal;
					break;
					case 2:
					
					break;
					}
				}
				else if( currentData.Device_Selector_Mode == SENSOR_2_MODE )
				{					
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S2manualCal >= 100)
							tempConfigData.calibrationConfig.S2manualCal -= 100;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S2manualCal > 0)
							--tempConfigData.calibrationConfig.S2manualCal;
					break;
					case 2:
						if(compareSignValue > -9999)
							--compareSignValue;
					break;
					}
				}
				DrawCalibManual(cursor, YELLOW);
			break;
			case BUTTON_ENTER:
				compareValue = 0;
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
				{
					
					 compareValue = 1;
				   if(compareValue)
					   {
						
						  if(configData.adjustConfig.filterS1 != 0)
							{
                  SendParmeter_Current = currentData.filterout_S1Current ;
              }
							else {
                  SendParmeter_Current = currentData.S1mVolt; 
              }							
							
                Tx_S1manualCal = tempConfigData.calibrationConfig.S1manualCal;
							  manual_cal_flag = 1;
							  Save_CalLog_Data(TEXT96_MANUAL_CALIBRATION,Tx_S1manualCal);
							  Update_Cal_Logdata();
							 configData.calibrationConfig = tempConfigData.calibrationConfig;					  
					      Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
             }
				}
				else if (currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
           
					  compareValue = 1;
				   if(compareValue)
					   {
                Tx_S2manualCal = tempConfigData.calibrationConfig.S2manualCal;
							  manual_cal_flag = 1;
							  Save_CalLog_Data(TEXT96_MANUAL_CALIBRATION,Tx_S2manualCal);
							  Update_Cal_Logdata();
							  configData.calibrationConfig = tempConfigData.calibrationConfig;					  
					  Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						 }
        }
				else
				{					
				}								
				 
				   subState = 0;
				   state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_CalibAuto(void)  
{
	switch(subState)
	{
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
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE)
				{
					DrawCalibAuto(cursor, WHITE);
					if(cursor == 0)						cursor = 1;
					else						--cursor;
					DrawCalibAuto(cursor, YELLOW);
				}
				else
				{
					DrawCalibAuto(cursor, WHITE);
					if(cursor == 0)						cursor = 2;
					else						--cursor;
					DrawCalibAuto(cursor, YELLOW);
				}
			break;
			case BUTTON_RIGHT:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE)
				{
					DrawCalibAuto(cursor, WHITE);
					++cursor;					cursor = cursor % 2;
					DrawCalibAuto(cursor, YELLOW);
				}
				else
				{
					DrawCalibAuto(cursor, WHITE);
					++cursor;					cursor = cursor % 3;
					DrawCalibAuto(cursor, YELLOW);
				}
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S1Value + 10 <= 999)
							tempConfigData.calibrationConfig.S1Value += 10;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S1Value < 999)
							++tempConfigData.calibrationConfig.S1Value;
					break;
					case 2:
						if(tempConfigData.calibrationConfig.errorRange < 99)
							++tempConfigData.calibrationConfig.errorRange;
					break;
					}
				}				
				else if(currentData.Device_Selector_Mode == SENSOR_1_MODE )	{
					
					switch(cursor)
					{
					case 0:
						if(compareSignValue + 100 <= 9999)  
							compareSignValue += 100;
					break;
					case 1:
						if(compareSignValue < 9999)            
							++compareSignValue;
					break;
					}
					
				}
				DrawCalibAuto(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				if( currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					switch(cursor)
					{
					case 0:
						if(tempConfigData.calibrationConfig.S1Value >= 10)
							tempConfigData.calibrationConfig.S1Value -= 10;
					break;
					case 1:
						if(tempConfigData.calibrationConfig.S1Value > 0)
							--tempConfigData.calibrationConfig.S1Value;
					break;
					case 2:
						if(tempConfigData.calibrationConfig.errorRange > 0)
							--tempConfigData.calibrationConfig.errorRange;
					break;
					}
				}
				else	if(currentData.Device_Selector_Mode == SENSOR_1_MODE ){
					
					switch(cursor)
					{
					case 0:
						if(compareSignValue >= 100)
							compareSignValue -= 100;
					break;
					case 1:
						if(compareSignValue > 0)
							--compareSignValue;
					break;
					}
				}
				DrawCalibAuto(cursor, YELLOW);
			break;
			case BUTTON_ENTER:			
					   
			if(currentData.Device_Selector_Mode == SENSOR_2_MODE) {
         	configData.calibrationConfig = tempConfigData.calibrationConfig;
				  
				  Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
       }
			else if	(currentData.Device_Selector_Mode == SENSOR_1_MODE) {
				   Tx_Adj_TempertureVaule = compareSignValue;	
            manual_cal_temp_flag = 1;
					 Save_CalLog_Data(TEXT96_TEMP_CALIBRATION,Tx_Adj_TempertureVaule);					
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
void State_CalibS2Cycle(void)  
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(tempConfigData.calibrationConfig.S2cycle < 999)
					++tempConfigData.calibrationConfig.S2cycle;
		
			break;
			case BUTTON_DOWN:
				if(tempConfigData.calibrationConfig.S2cycle > 0)
					--tempConfigData.calibrationConfig.S2cycle;
			
			break;
			case BUTTON_ENTER:
				if(configData.calibrationConfig.S2cycle != tempConfigData.calibrationConfig.S2cycle)
				{
					configData.calibrationConfig = tempConfigData.calibrationConfig;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}


void State_CalibLog(void)  
{
	int year_temp = 0;
	int vaule_temp = 0;
	
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	
	       if(caldisp_counter == LogData[caldisp_counter*11] ) 
					{
						 if(caldisp_counter == 0)
						 {
                caldisp_array_counter = caldisp_counter * 10;
             }						 
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[0].calibrationTime.tm_year = year_temp;
						 calibLog[0].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[0].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[0].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[0].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[0].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[0].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[0].calibrationValue = vaule_temp; // 1.000		
            				 
          }
          caldisp_counter++;					
					if(caldisp_counter == LogData[caldisp_counter*11] ) // cursor + 2
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						
						 year_temp = LogData[caldisp_array_counter+1];						
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[1].calibrationTime.tm_year = year_temp;
						 calibLog[1].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[1].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[1].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[1].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[1].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[1].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[1].calibrationValue = vaule_temp; // 1.000
						 
          }		
					caldisp_counter++;
					if(caldisp_counter == LogData[caldisp_counter*11] ) // cursor + 2
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[2].calibrationTime.tm_year = year_temp;
						 calibLog[2].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[2].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[2].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[2].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[2].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[2].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[2].calibrationValue = vaule_temp; // 1.000
						
          }		
					caldisp_counter++;
					if(caldisp_counter == LogData[caldisp_counter*11]) // cursor + 2
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter;
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[3].calibrationTime.tm_year = year_temp;
						 calibLog[3].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[3].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[3].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[3].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[3].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[3].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[3].calibrationValue = vaule_temp; // 1.000
						
          }							
	
		RedrawValue();
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(cursor > 0)
				{
					--cursor;				
					caldisp_counter = caldisp_counter-4;
					caldisp_counter--;
					if(caldisp_counter < 0 ) {caldisp_counter = 0;}
					
					
					if(caldisp_counter == LogData[caldisp_counter*11] ) // cursor + 1
					{
						 if(caldisp_counter == 0)
						 {
                caldisp_array_counter = caldisp_counter * 10;
             }						 
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; //
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[0].calibrationTime.tm_year = year_temp;
						 calibLog[0].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[0].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[0].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[0].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[0].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[0].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[0].calibrationValue = vaule_temp; // 1.000
						
          }	
					else{
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
					if(caldisp_counter == LogData[caldisp_counter*11] ) 
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						
						 year_temp = LogData[caldisp_array_counter+1];						
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[1].calibrationTime.tm_year = year_temp;
						 calibLog[1].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[1].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[1].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[1].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[1].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[1].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[1].calibrationValue = vaule_temp; // 1.000
						// caldisp_counter++;
          }	
          else{
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
					if(caldisp_counter == LogData[caldisp_counter*11] ) 
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[2].calibrationTime.tm_year = year_temp;
						 calibLog[2].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[2].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[2].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[2].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[2].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[2].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[2].calibrationValue = vaule_temp; // 1.000
						 
          }	
          else{
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
					if(caldisp_counter == LogData[caldisp_counter*11]) 
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter;
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[3].calibrationTime.tm_year = year_temp;
						 calibLog[3].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[3].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[3].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[3].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[3].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[3].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[3].calibrationValue = vaule_temp; // 1.000
						 
          }	
          else{
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
				if(cursor < 95)
				{
					caldisp_counter++;
					caldisp_counter = caldisp_counter - 3;
					
					++cursor;
					if(caldisp_counter == LogData[caldisp_counter*11] ) // cursor + 1
					{
						 if(caldisp_counter == 0)
						 {
                caldisp_array_counter = caldisp_counter * 10;
             }						 
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; //
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[0].calibrationTime.tm_year = year_temp;
						 calibLog[0].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[0].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[0].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[0].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[0].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[0].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[0].calibrationValue = vaule_temp; // 1.000
						 
          }	
					else{ 
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
					if(caldisp_counter == LogData[caldisp_counter*11] ) 
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						
						 year_temp = LogData[caldisp_array_counter+1];						
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[1].calibrationTime.tm_year = year_temp;
						 calibLog[1].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[1].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[1].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[1].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[1].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[1].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[1].calibrationValue = vaule_temp; // 1.000
						 
          }		
					else{
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
					if(caldisp_counter == LogData[caldisp_counter*11] ) // cursor + 2
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter; 
						
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[2].calibrationTime.tm_year = year_temp;
						 calibLog[2].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[2].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[2].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[2].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[2].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[2].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[2].calibrationValue = vaule_temp; // 1.000
						
          }		
					else{
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
					if(caldisp_counter == LogData[caldisp_counter*11]) // cursor + 2
					{
						 caldisp_array_counter = (caldisp_counter * 10)+caldisp_counter;
						 year_temp = LogData[caldisp_array_counter+1];
						 year_temp = (year_temp<<8) + LogData[caldisp_array_counter+2];
             calibLog[3].calibrationTime.tm_year = year_temp;
						 calibLog[3].calibrationTime.tm_mon = (int)LogData[caldisp_array_counter+3];
					   calibLog[3].calibrationTime.tm_mday = (int)LogData[caldisp_array_counter+4];
						 calibLog[3].calibrationTime.tm_hour = (int)LogData[caldisp_array_counter+5];
						 calibLog[3].calibrationTime.tm_min = (int)LogData[caldisp_array_counter+6];
						 calibLog[3].calibrationTime.tm_sec = (int)LogData[caldisp_array_counter+7];
						
					   calibLog[3].calibrationMethod = (int)LogData[caldisp_array_counter+8];
						 vaule_temp = LogData[caldisp_array_counter+9];
						 vaule_temp = (vaule_temp<<8) + LogData[caldisp_array_counter+10];
					   calibLog[3].calibrationValue = vaule_temp; // 1.000
						
          }
					else{
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
void State_ConfigRelayRelay1(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
				cursor ^= 1;
				DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
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
void State_ConfigRelayRelay2(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
				cursor ^= 1;
				DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
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
void State_ConfigRelayRelay3(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, BLACK);
				cursor ^= 1;
				DrawLineRectangle(0+(cursor*240), 238, 240+(cursor*240), 272, YELLOW);
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

void State_ConfigAdjustGradient(void)  
{
	switch(subState)
	{
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
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					DrawAdjustGradient(cursor, compareValue, WHITE);
					cursor ^= 1;
					DrawAdjustGradient(cursor, compareValue, YELLOW);
				}
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(tempConfigData.adjustConfig.gradientS1 < 999)
					{
						++tempConfigData.adjustConfig.gradientS1;
						DrawAdjustGradient(0, 0, YELLOW);
					}
				}		
				else	{				
				}
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(tempConfigData.adjustConfig.gradientS1 > 0)
					{
						--tempConfigData.adjustConfig.gradientS1;
						DrawAdjustGradient(0, 0, YELLOW);
					}
				}
				else	{					
				}
			break;
			case BUTTON_ENTER:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(configData.adjustConfig.gradientS1 != tempConfigData.adjustConfig.gradientS1)
					{
						configData.adjustConfig = tempConfigData.adjustConfig;
						Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
					}
				}				
				else{
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_ConfigAdjustOffset(void)  
{
	switch(subState)
	{
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
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					DrawAdjustOffset(cursor, compareValue, WHITE);
					cursor ^= 1;
					DrawAdjustOffset(cursor, compareValue, YELLOW);
				}
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(tempConfigData.adjustConfig.offsetS1 < 999)
					{
						++tempConfigData.adjustConfig.offsetS1;
						DrawAdjustOffset(0, 0, YELLOW);
					}
				}				
				else	{
					
				}
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(tempConfigData.adjustConfig.offsetS1 > -999)
					{
						--tempConfigData.adjustConfig.offsetS1;
						DrawAdjustOffset(0, 0, YELLOW);
					}
				}				
				else
				{
					
				}
			break;
			case BUTTON_ENTER:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(configData.adjustConfig.offsetS1 != tempConfigData.adjustConfig.offsetS1)
					{
						configData.adjustConfig = tempConfigData.adjustConfig;
						Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
					}
				}
				else
				{
					if(configData.adjustConfig.offsetS1 != tempConfigData.adjustConfig.offsetS1 || configData.adjustConfig.offsetS2 != tempConfigData.adjustConfig.offsetS2)
					{
						configData.adjustConfig = tempConfigData.adjustConfig;
						Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
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
void State_ConfigAdjustFilter(void)  
{
	switch(subState)
	{
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
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					DrawAdjustFilter(cursor, compareValue, WHITE);
					cursor ^= 1;
					DrawAdjustFilter(cursor, compareValue, YELLOW);
				}
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(tempConfigData.adjustConfig.filterS1 < 999)
					{
						++tempConfigData.adjustConfig.filterS1;
						DrawAdjustFilter(0, 0, YELLOW);
					}
				}				
				else
				{
					
				}
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(tempConfigData.adjustConfig.filterS1 > 0)
					{
						--tempConfigData.adjustConfig.filterS1;
						DrawAdjustFilter(0, 0, YELLOW);
					}
				}				
				else	{					
				}
			break;
			case BUTTON_ENTER:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(configData.adjustConfig.filterS1 != tempConfigData.adjustConfig.filterS1)
					{
						configData.adjustConfig = tempConfigData.adjustConfig;
						configData.adjustConfig.filter_update_flag = 1;
						Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
					}
				}
				else
				{
					
				}
         if(configData.adjustConfig.filterS1 == 0 ||  tempConfigData.adjustConfig.filterS1 == 0)
					{
						configData.adjustConfig = tempConfigData.adjustConfig;
						configData.adjustConfig.filter_update_flag = 0;
						Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
					}
				
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_ConfigAdjustFactoryReset(void)
{
	switch(subState)
	{
	case 0:
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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

void State_ConfigRelayRelay1Wash(void)   
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
					cursor ^= 1;
				else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					if(cursor == 0)
						cursor = 2;
					else
						--cursor;
				}
				DrawRelay1Wash(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawRelay1Wash(cursor, WHITE);
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
					cursor ^= 1;
				else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					++cursor;
					cursor = cursor % 3;
				}
				DrawRelay1Wash(cursor, YELLOW);
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(cursor == 0)
					{
						if(tempConfigData.relayConfig.relay1WashCycle < 999)
							++tempConfigData.relayConfig.relay1WashCycle;
					}
					else
					{
						if(tempConfigData.relayConfig.relay1WashTime < 999)
							++tempConfigData.relayConfig.relay1WashTime;
					}
				}
				else
				{
				}
				DrawRelay1Wash(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(cursor == 0)
					{
						if(tempConfigData.relayConfig.relay1WashCycle > 0)
							--tempConfigData.relayConfig.relay1WashCycle;
					}
					else
					{
						if(tempConfigData.relayConfig.relay1WashTime > 0)
							--tempConfigData.relayConfig.relay1WashTime;
					}
				}
				else
				{
				}
				DrawRelay1Wash(cursor, YELLOW);
			break;
			case BUTTON_ENTER:
				if(tempConfigData.relayConfig.relay1WashCycle != configData.relayConfig.relay1WashCycle || tempConfigData.relayConfig.relay1WashTime != configData.relayConfig.relay1WashTime)
				{
					configData.relayConfig = tempConfigData.relayConfig;
					wash_run_flag = 1;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				else if(tempConfigData.relayConfig.relay1WashCycle == 0 ||tempConfigData.relayConfig.relay1WashTime == 0 ) 
				{
					configData.relayConfig = tempConfigData.relayConfig;
					wash_run_flag = 0;					
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}


void State_ConfigRelayRelay1manualOP(void)
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
			case BUTTON_DOWN:
				  tempConfigData.relayConfig.relay1manual ^= 1;
			    if(tempConfigData.relayConfig.relay1manual){
						 DrawTextETC(200, 130, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
						 configData.relayConfig.relay1WashCycle = 0;
						 configData.relayConfig.relay1WashTime = 0;
						 wash_run_flag = 0;
						 Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						 RELAY1_ON;
					}
       	  else{
						
	         	DrawTextETC(200, 130, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
						 configData.relayConfig.relay1WashCycle = 0;
						 configData.relayConfig.relay1WashTime = 0;
						 wash_run_flag = 0;
						 Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						 RELAY1_OFF;
			    }
				break;
			case BUTTON_ENTER:
				if(configData.relayConfig.relay1manual != tempConfigData.relayConfig.relay1manual)
				{
					configData.relayConfig.relay1manual = tempConfigData.relayConfig.relay1manual;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}

void State_ConfigRelayRelay2AutoAlarm(void)
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {}
				else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					if(cursor == 0)
						cursor = 2;
					else
						--cursor;
				}
			break;
			case BUTTON_RIGHT:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE){}
				else if(currentData.Device_Selector_Mode == SENSOR_12_MODE){
				}
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					tempConfigData.relayConfig.relay2AutoAlarm ^= 1;
				}				
				else
				{
					
				}
				DrawRelay2Alarm(tempConfigData.relayConfig.relay2AutoAlarm, YELLOW);
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)	{
					tempConfigData.relayConfig.relay2AutoAlarm ^= 1;
				}
				else	{					
				}
				DrawRelay2Alarm(tempConfigData.relayConfig.relay2AutoAlarm, YELLOW);
			break;
			case BUTTON_ENTER:
				if(configData.relayConfig.relay2AutoAlarm != tempConfigData.relayConfig.relay2AutoAlarm)
				{
					configData.relayConfig.relay2AutoAlarm = tempConfigData.relayConfig.relay2AutoAlarm;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_ConfigRelayRelay2AlarmOFF(void)
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
			case BUTTON_DOWN:
				tempConfigData.relayConfig.relay2AlarmOFF ^= 1;
				DrawRelayAlarm(tempConfigData.relayConfig.relay2AlarmOFF);
			break;
			case BUTTON_ENTER:
				if(configData.relayConfig.relay2AlarmOFF != tempConfigData.relayConfig.relay2AlarmOFF)
				{
					configData.relayConfig.relay2AlarmOFF = tempConfigData.relayConfig.relay2AlarmOFF;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_ConfigRelayRelay3SetOP(void)
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
					cursor ^= 1;
				else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					if(cursor == 0)
						cursor = 2;
					else
						--cursor;
				}
				DrawRelay3Wash(cursor, YELLOW);
			break;
			case BUTTON_RIGHT:
				DrawRelay3Wash(cursor, WHITE);
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
					cursor ^= 1;
				else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
				{
					++cursor;
					cursor = cursor % 3;
				}
				DrawRelay3Wash(cursor, YELLOW);
			break;
			case BUTTON_UP:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(cursor == 0)
					{
						if(tempConfigData.relayConfig.relay3WashCycle < 999)
							++tempConfigData.relayConfig.relay3WashCycle;
					}
					else
					{
						if(tempConfigData.relayConfig.relay3WashTime < 999)
							++tempConfigData.relayConfig.relay3WashTime;
					}
				}				
				else
				{
					
				}
				DrawRelay3Wash(cursor, YELLOW);
			break;
			case BUTTON_DOWN:
				if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
				{
					if(cursor == 0)
					{
						if(tempConfigData.relayConfig.relay3WashCycle > 0)
							--tempConfigData.relayConfig.relay3WashCycle;
					}
					else
					{
						if(tempConfigData.relayConfig.relay3WashTime > 0)
							--tempConfigData.relayConfig.relay3WashTime;
					}
				}				
				else
				{
					
				}
				DrawRelay3Wash(cursor, YELLOW);
			break;
			case BUTTON_ENTER:
				if(tempConfigData.relayConfig.relay3WashCycle != configData.relayConfig.relay3WashCycle || tempConfigData.relayConfig.relay3WashTime != configData.relayConfig.relay3WashTime)
				{
					configData.relayConfig = tempConfigData.relayConfig;
					wash_run_flag3 = 1;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				else if(tempConfigData.relayConfig.relay3WashCycle == 0 ||tempConfigData.relayConfig.relay3WashTime == 0 )
				{
					configData.relayConfig = tempConfigData.relayConfig;
					wash_run_flag3 = 0;					
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}
void State_ConfigRelayRelay3manualOP(void)
{
	switch(subState)
	{
	case 0:
		tempConfigData = configData;
		RedrawTitle();
		RedrawBottomArea();
		RedrawViewArea();
		subState = 1;
		cursor = 0;
	break;
	case 1:
		if(flag10ms&FLAG10MS_BUTTON)
		{
			flag10ms &= ~FLAG10MS_BUTTON;
			switch(CheckButton())
			{
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
			case BUTTON_DOWN:
				tempConfigData.relayConfig.relay3manual ^= 1;
			 if(tempConfigData.relayConfig.relay3manual) {
			       DrawTextETC(200, 130, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
				     configData.relayConfig.relay3WashCycle = 0;
						 configData.relayConfig.relay3WashTime = 0;
						 wash_run_flag3 = 0;
						 Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						 RELAY3_ON;
			 }
		   else {
			       DrawTextETC(200, 130, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
				     configData.relayConfig.relay3WashCycle = 0;
						 configData.relayConfig.relay3WashTime = 0;
						 wash_run_flag3 = 0;
						 Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
						 RELAY3_OFF;
			 }
			break;
			case BUTTON_ENTER:
				if(configData.relayConfig.relay3manual != tempConfigData.relayConfig.relay3manual)
				{
					configData.relayConfig.relay3manual = tempConfigData.relayConfig.relay3manual;
					Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
				}
				subState = 0;
				state = STATE_MAIN;
			break;
			}
		}
	break;
	}
}

void MoveBackState(void)
{
	state = (state >> 4)&0xFFFFFFFF;
}
void MoveSelectedState(void)
{
	state = (state << 4)&0xFFFFFFFF;
	state |= cursor+1;
}

void FactoryReset(void)
{
	configData.outputConfig.output4mA = 0;
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE) {configData.outputConfig.output20mA = 2000;}
	else if (currentData.Device_Selector_Mode == SENSOR_2_MODE){configData.outputConfig.output20mA = 20000;}
	configData.outputConfig.outputReal4mA = 0;
	configData.outputConfig.outputReal20mA = 0;
	
	configData.modbusConfig.mode = 0;
	configData.modbusConfig.baudrate = 0;
	configData.modbusConfig.modbusSlaveAddr = 0;
	configData.modbusConfig.databit = 0;
	
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
	Flash_Write(SAVEADDR_CONFIG_BASE, (vu32*)&configData, sizeof(ConfigSet));
	Clear_EEprom_Data();
}

void RedrawTime(void)
{
	currentTime = Time_GetCalendarTime();
	if(currentTime.tm_sec != prevTime.tm_sec)
	{
		sprintf(strBuffer, "%04d-%02d-%02d",  currentTime.tm_year, currentTime.tm_mon+1, currentTime.tm_mday);
		DrawSmallNumber(10, 245, strBuffer);
		sprintf(strBuffer, "%02d:%02d:%02d",  currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
		DrawSmallNumber(179, 245, strBuffer);
	}
	prevTime = currentTime;
}

void ClearstrBuffer(void)
{
    int i ;
	
	  for (i=0 ; i<35 ; i++)
	 {
        strBuffer[i] = 0x00;
   }	
}

