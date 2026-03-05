#include "CurrentSorceTrans.h"

#define SENSOR1 1
#define SENSOR2 3
#define TEMPSENSOR 2


#ifndef SENSOR_PH_EC
	uint16_t High_PPm_AD = 2;  
	uint16_t Low_PPm_AD  = 0;

	uint16_t  High_NTU_AD = 10;
	uint16_t  Low_NTU_AD = 0;
#else
	uint16_t High_PPm_AD = 14;  
	uint16_t Low_PPm_AD  = 0;

	uint16_t  High_NTU_AD = 2000;
	uint16_t  Low_NTU_AD = 0;
#endif


uint16_t High_TempD = 60; 
uint16_t Low_TempD  = 0;

uint16_t High_Current = 20;  
uint16_t Low_Current  = 4;

float Senosr1Slope = 0.0;
float Senosr1Offset = 0.0;

float Current1Slope = 0.0;
float Current11Offset = 0.0;

float Senosr2Slope = 0.0;
float Senosr2Offset = 0.0;

float Current2Slope = 0.0;
float Current21Offset = 0.0;

float Temp_Slope = 0.0;
float Temp_Offset = 0.0;

uint16_t compare_vaule_20mA = 5000;  //5000; BY JU
uint16_t compare_vaule_4mA  = 0;   //600  100;

uint16_t compare_vaule_20mA2 = 5000;  //5000; BY JU
uint16_t compare_vaule_4mA2  = 0;   //600  100;

uint16_t final_OutPut_vaule = 0;
uint16_t trans_anlog_pwmVaule  = 0;
uint16_t trans_anlog_pwmVaule2 = 0;
uint16_t trans_temp_pwmVaule = 0;

uint16_t Lowpwm = 0;
uint16_t Highpwm = 0;
uint16_t LowCurrentpwm = 0;
uint16_t HighCurrentpwm = 0;


// 12-4.0
// 13=4.05
// 14-4.16
// 15-4.27
// 16-4.37
// 17- 4.44
// 18-4.54
// 19-4.63
// 20-4.7

//30 - 5.55
//40-5.8

// 100-11.6
// 150-15.8
// 180-18.45
// 190-19.28
// 195-19.73
// 197-19.85


// index 가 ppm 이다. 20230902
const unsigned int DAC_4_20MA[34] = 
{   
1106,1150,1200,1250,1300, 1350,1400,1450,1500,1550,
1600,1650,1700,1750,1800, 1850,1900,1950,2000,2050,
2100,2150,2200,2250,2300, 2350,2400,2450,2500,2550,
2600,2650,2675, 2695 };


const unsigned int TABLE_4_20MA_100[34] = 
{
400,448,503,558,612, 667,722,776,831,886,
941,995,1050,1105,1160, 1214,1269,1324,1378,1433,
1488,1543,1597,1652,1707, 1761,1816,1871,1926, 1980,
2035,2090,2117, 2140 };


		 /*  152 차이 -1  */


 		 /*  152 차이 -2   */


#define NO1


#ifdef NO1

const float PPM_4_20MA[18] = {
3.76	,
4.68	,
5.64	,
6.76	,
7.88	,
8.84	,
9.88	,
10.84	,
11.88	,
12.76	,
13.72	,
14.6	,
15.56	,
16.36	,
17.32	,
18.28	,
19.4	,
20.28	

};


const float PWM_4_20MA[18] =  {
1116	,
1419	,
1723	,
2026	,
2330	,
2633	,
2937	,
3246	,
3561	,
3856	,
4160	,
4465	,
4770	,
5075	,
5379	,
5684	,
5989	,
6200	


};

#endif


#ifdef NO2

const float PPM_4_20MA[18] = {
4.01	,
5.00	,
6.01	,
7.00	,
7.99	,
9.00	,
9.98	,
10.96	,
11.91	,
12.83	,
13.74	,
14.62	,
15.50	,
16.37	,
17.21	,
18.19	,
19.30	,
20.00	


};


const float PWM_4_20MA[18] =  {
1145	,
1447	,
1749	,
2052	,
2355	,
2659	,
2962	,
3270	,
3584	,
3878	,
4183	,
4487	,
4791	,
5095	,
5399	,
5703	,
6007	,
6200	


};

#endif


void Senosr1_Gain_ADJ (void)
{
	//Senosr1_Slop_Cal( , (uint16_t)configData.outputConfig.outputReal20mA);

	Lowpwm = (uint16_t)configData.outputConfig.outputReal4mA;
	Highpwm = (uint16_t)configData.outputConfig.outputReal20mA;

	Senosr1Slope = (float)(((float)(Highpwm - Lowpwm)) / ((float)(High_PPm_AD - Low_PPm_AD)));
	Senosr1Offset = (float)((float)Lowpwm - (float)( Senosr1Slope * Low_PPm_AD ));


  	LowCurrentpwm = (uint16_t)configData.outputConfig.outputReal4mA;
	HighCurrentpwm = (uint16_t)configData.outputConfig.outputReal4mA;
	if(LowCurrentpwm == 0){ LowCurrentpwm = DAC_4_20MA[0]; }
	if(HighCurrentpwm == 0) { HighCurrentpwm = DAC_4_20MA[33];}  

	Current1Slope = (float)(((float)(High_Current - Low_Current)) / ((float)(HighCurrentpwm - LowCurrentpwm)));
	Current11Offset = (float)((float)Low_Current - (float)( Current1Slope * LowCurrentpwm ));  
}

void Senosr2_Gain_ADJ (void)
{
	Lowpwm = (uint16_t)configData.outputConfig.outputReal4mA2;
	Highpwm = (uint16_t)configData.outputConfig.outputReal20mA2;

	Senosr2Slope = (float)(((float)(Highpwm - Lowpwm)) / ((float)(High_NTU_AD - Low_NTU_AD)));
	Senosr2Offset = (float)((float)Lowpwm - (float)( Senosr2Slope * Low_NTU_AD ));

  	LowCurrentpwm = (uint16_t)configData.outputConfig.outputReal4mA2;
	HighCurrentpwm = (uint16_t)configData.outputConfig.outputReal4mA2;
	if(LowCurrentpwm == 0){ LowCurrentpwm = DAC_4_20MA[0]; }
	if(HighCurrentpwm == 0) { HighCurrentpwm = DAC_4_20MA[33];}  

	Current2Slope = (float)(((float)(High_Current - Low_Current)) / ((float)(HighCurrentpwm - LowCurrentpwm)));
	Current21Offset = (float)((float)Low_Current - (float)( Current2Slope * LowCurrentpwm ));  
}


void Temperture_Slop_Cal (uint16_t lowpwmvaule , uint16_t highpwmvaule)
{
  Lowpwm = lowpwmvaule;
	Highpwm = highpwmvaule;

	//if(lowpwmvaule == 0){ Lowpwm = DAC_4_20MA[40]; }
	if(highpwmvaule == 0) { Highpwm = DAC_4_20MA[200]; } //5700;}	

	Temp_Slope = (float)(((float)(Highpwm - Lowpwm)) / ((float)(High_TempD - Low_TempD)));
	Temp_Offset = (float)((float)Lowpwm - (float)( Temp_Slope * Low_TempD ));

}

void Update_CurrentTrans (void)
{ 
	int index, i,j, k, l, data;
	float f1,f2, min, max, imsi, ppm_4_20ma;


	if(currentData.holdState == 0) {
		if(comm_run_flag == 2 && state == 0 ) {

			comm_run_flag = 0;
			if((compare_vaule_4mA != configData.outputConfig.outputReal4mA) || (compare_vaule_20mA != configData.outputConfig.outputReal20mA))
			{
				Senosr1_Gain_ADJ();
				compare_vaule_4mA = configData.outputConfig.outputReal4mA;
				compare_vaule_20mA = configData.outputConfig.outputReal20mA;
			}
			if((compare_vaule_4mA2 != configData.outputConfig.outputReal4mA2) || (compare_vaule_20mA2 != configData.outputConfig.outputReal20mA2))
			{
				Senosr2_Gain_ADJ();
				compare_vaule_4mA2 = configData.outputConfig.outputReal4mA2;
				compare_vaule_20mA2 = configData.outputConfig.outputReal20mA2;
			}

//			if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
				trans_anlog_pwmVaule = (uint16_t)((Senosr1Slope*((float)currentData.S1PPM * 0.01))+Senosr1Offset);
				currentData.S1mA = (uint32_t)(((Current1Slope * (float)trans_anlog_pwmVaule)+Current11Offset)*100);
//			}
//			else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)	{
				trans_anlog_pwmVaule2 = (uint16_t)((Senosr2Slope*((float)currentData.S2PPM * 0.001))+Senosr2Offset);
				currentData.S2mA = (uint32_t)(((Current2Slope * (float)trans_anlog_pwmVaule2)+Current21Offset)*100);
//			}

	//		if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
//				trans_anlog_pwmVaule = (uint16_t)((Senosr1Slope*((float)currentData.S1PPM * 0.01))+Senosr1Offset);
//				currentData.S1mA = (uint32_t)(((Current1Slope * (float)trans_anlog_pwmVaule)+Current11Offset)*100);


	//			currentData.S1mA = (uint32_t)(((Current1Slope * (float)trans_anlog_pwmVaule)+Current11Offset)*100);
	//		}


			// DAC1 output
			TIM8_Chage_Duty_Channel(2,trans_anlog_pwmVaule);

			TIM8_Chage_Duty_Channel(3,trans_anlog_pwmVaule2);

			#ifdef NEW_BOARD
//			DAC_output(1, trans_anlog_pwmVaule);
			#endif

			if(currentData.temperature != 0)
			{
				Temperture_Slop_Cal(0,0);
				trans_temp_pwmVaule = (uint16_t)((Temp_Slope*((float)currentData.temperature * 0.01))+Temp_Offset); 

				// DAC2 output
				TIM8_Chage_Duty_Channel(1,trans_temp_pwmVaule);
				#ifdef NEW_BOARD
				//DAC_output(2, trans_temp_pwmVaule);
				#endif
			}
		}
	}
	else {
		TIM8_Chage_Duty_Channel(2,0);
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
