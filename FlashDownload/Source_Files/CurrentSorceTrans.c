#include "CurrentSorceTrans.h"

#define SENSOR1 1
#define SENSOR2 3
#define TEMPSENSOR 2

uint16_t High_PPm_AD = 2;  
uint16_t Low_PPm_AD  = 0;

uint16_t  High_NTU_AD = 20;
uint16_t  Low_NTU_AD = 0;

uint16_t High_TempD = 60; 
uint16_t Low_TempD  = 0;

uint16_t High_Current = 20;  
uint16_t Low_Current  = 4;

float Senosr1Slope = 0.0;
float Senosr1Offset = 0.0;

float Current1Slope = 0.0;
float Current11Offset = 0.0;

float Temp_Slope = 0.0;
float Temp_Offset = 0.0;

uint16_t compare_vaule_20mA = 5500;  //5000; BY JU
uint16_t compare_vaule_4mA  = 600;   //100;

uint16_t final_OutPut_vaule = 0;
uint16_t trans_anlog_pwmVaule = 0;
uint16_t trans_temp_pwmVaule = 0;

uint16_t Lowpwm = 0;
uint16_t Highpwm = 0;
uint16_t LowCurrentpwm = 0;
uint16_t HighCurrentpwm = 0;
	
void Senosr1_Gain_ADJ (void)
{
	Senosr1_Slop_Cal((uint16_t)configData.outputConfig.outputReal4mA , (uint16_t)configData.outputConfig.outputReal20mA);
	current_conversion_slop_cal((uint16_t)configData.outputConfig.outputReal4mA , (uint16_t)configData.outputConfig.outputReal20mA);
}

void Senosr1_Slop_Cal (uint16_t lowpwmvaule , uint16_t highpwmvaule)
{
  Lowpwm = lowpwmvaule;
	Highpwm = highpwmvaule;
	
	if(lowpwmvaule == 0){ Lowpwm = 600; }
	if(highpwmvaule == 0) { Highpwm = 5500;}	

  if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
	Senosr1Slope = (float)(((float)(Highpwm - Lowpwm)) / ((float)(High_PPm_AD - Low_PPm_AD)));
	Senosr1Offset = (float)((float)Lowpwm - (float)( Senosr1Slope * Low_PPm_AD ));
	}
	else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)	{
	Senosr1Slope = (float)(((float)(Highpwm - Lowpwm)) / ((float)(High_NTU_AD - Low_NTU_AD)));
	Senosr1Offset = (float)((float)Lowpwm - (float)( Senosr1Slope * Low_NTU_AD ));
  }
}


void current_conversion_slop_cal(uint16_t lowpwmvaule , uint16_t highpwmvaule)
{
  	LowCurrentpwm = lowpwmvaule;
	HighCurrentpwm = highpwmvaule;
	if(lowpwmvaule == 0){ LowCurrentpwm = 600; }
	if(highpwmvaule == 0) { HighCurrentpwm = 5500;}  
  
	Current1Slope = (float)(((float)(High_Current - Low_Current)) / ((float)(HighCurrentpwm - LowCurrentpwm)));
	Current11Offset = (float)((float)Low_Current - (float)( Current1Slope * LowCurrentpwm ));  

}

void Temperture_Slop_Cal (uint16_t lowpwmvaule , uint16_t highpwmvaule)
{
  Lowpwm = lowpwmvaule;
	Highpwm = highpwmvaule;
	
	if(lowpwmvaule == 0){ Lowpwm = 600; }
	if(highpwmvaule == 0) { Highpwm = 5700;}	

	Temp_Slope = (float)(((float)(Highpwm - Lowpwm)) / ((float)(High_TempD - Low_TempD)));
	Temp_Offset = (float)((float)Lowpwm - (float)( Temp_Slope * Low_TempD ));
	
}

void Update_CurrentTrans (void)
{ 
	if(currentData.holdState == 0) {
		if(comm_run_flag == 2 && state == 0 ) {
			
			comm_run_flag = 0;
			if((compare_vaule_4mA != configData.outputConfig.outputReal4mA) || (compare_vaule_20mA != configData.outputConfig.outputReal20mA))
			{
				Senosr1_Gain_ADJ();
				compare_vaule_4mA = configData.outputConfig.outputReal4mA;
				compare_vaule_20mA = configData.outputConfig.outputReal20mA;
			}
			if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
				trans_anlog_pwmVaule = (uint16_t)((Senosr1Slope*((float)currentData.S1PPM * 0.01))+Senosr1Offset);
				currentData.S1mA = (uint32_t)(((Current1Slope * (float)trans_anlog_pwmVaule)+Current11Offset)*100);
			}
			else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)	{
				trans_anlog_pwmVaule = (uint16_t)((Senosr1Slope*((float)currentData.S2PPM * 0.001))+Senosr1Offset);
				currentData.S1mA = (uint32_t)(((Current1Slope * (float)trans_anlog_pwmVaule)+Current11Offset)*100);
			}
			TIM8_Chage_Duty_Channel(1,trans_anlog_pwmVaule);
			
			if(currentData.temperature != 0)
			{
				Temperture_Slop_Cal(0,0);
				trans_temp_pwmVaule = (uint16_t)((Temp_Slope*((float)currentData.temperature * 0.01))+Temp_Offset); 
				TIM8_Chage_Duty_Channel(3,trans_temp_pwmVaule);
			}
		}
	}
	else {
		TIM8_Chage_Duty_Channel(1,0);
		TIM8_Chage_Duty_Channel(3,0);
	}
}

	   
void TIM8_Chage_Duty_Channel(uint16_t channel , uint16_t Duty)
{ 
	 
		switch (channel)
  	 {
        case 1 :
					      TIM8->CCR1 = Duty;  
				      break;
				case 2 :
					      TIM8->CCR2 = Duty;   
					    break;
				case 3 :
					      TIM8->CCR3 = Duty;   
					    break;
				case 4 :
					      TIM8->CCR4 = Duty;
					    break;
				default:
					    break;				
     }
}
