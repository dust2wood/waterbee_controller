#include "Communication.h"

uint32_t rx1HandlerCount = 0, rx2HandlerCount = 0, rx3HandlerCount = 0, rs485DriveCount = 0;
uint16_t tx2Buffering[16], tx3Buffering[16];

char com485State = 0;
uint32_t getModbusValue = 0;


uint16_t comm_run_flag = 0;

uint32_t req_ppm_com_flag = 0;
uint32_t req_current_com_flag = 0;

uint32_t req_zero_ppm_com_flag = 0;
uint32_t req_zero_current_com_flag = 0;

uint32_t req_temp_com_flag = 0;

uint32_t REqParmeterPPM;
uint32_t REqParmeterNTU;
uint32_t REqParmeterCurrent;

uint32_t REqParmeterZero_PPM;
uint32_t REqParmeterZero_Current;

uint32_t SendParmeter_Current;
uint32_t SendParmeter_Zero_Current;

uint32_t Couter_1 = 0;
uint32_t Couter_2 = 0;
uint32_t Couter_3 = 0;
uint32_t Couter_4 = 0;

uint32_t ReqParmeterTemp;

uint32_t ReqParmeterZero_NTU;
uint16_t ErrorCodeRecive;
uint16_t ComCouter = 0;
uint16_t ComCouter_Prev = 0;
uint16_t ComCouter2 = 0;

uint16_t couter_vaule_ppm = 0;
double Adj_gradientS1 = 0;
double Adj_offsetS1 = 0;
double Adj_gradientS2 = 0;
double Adj_offsetS2 = 0;
double mv_vaule = 0;
uint16_t conter_Filter = 0;
uint16_t conter_Filter2 = 0;

double filterSumData = 0;
double filterSumData2 = 0;

uint16_t first_flag = 0;

char strBuffer2[10];

#define MEASURE_X1 20
#define MEASURE_X2 40
#define MEASURE_Y  70

char WORKING_TIMER=0;

uint16_t	rx1_read_ptr=0;

// RS232, RS485 �ð������� 4-20Ma �� �����ϱ�
uint16_t rx2_time_count=0, rx3_time_count=0;


extern uint16_t data_pH, data_pH_imsi, data_EC;		// ph = 0.00  temp = 0.0  ec=0.0
extern int16_t data_TEMP, SET_data_TEMP;

char  ph_temp=0;	// ph �Ǵ� temp �����ϴ°�, ǥ���Ҷ� �߸��Ǵ°�찡 �־ �̸� �����...

unsigned char	Sensor1_OK_TIME=20;
unsigned char	Sensor2_OK_TIME=20;
unsigned char	Sensor3_OK_TIME=20;
unsigned char	Sensor4_OK_TIME=20;


void SensorComHandler(void) {

			
/*	UART1 INT ���� ó������.

    if (rx1Size < 19) {
		rx1_read_ptr=0;
	}
	else 	  {

        if (flag10ms & FLAG10MS_RX1) {	// 10msec ���� ������
            flag10ms &= ~FLAG10MS_RX1;

  			
            ++rx1HandlerCount;
            if (rx1HandlerCount > 1) {
                if (rx1Buffer[0] == '$') {
                    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                        if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'V' && rx1Buffer[5] == ',' && manual_cal_flag == 0 && manual_cal_temp_flag == 0 && zero_cal_update_flag == 0) {
//                            if (state == 0) {
							{
                                //DrawTextsize120(MEASURE_X2, MEASURE_Y, TEXT120_MEASURING, DRAW_IMAGE_ENABLE);
								if (WORKING_TIMER==0) { DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE); WORKING_TIMER=1;}
								else 				  { DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE); WORKING_TIMER=0;}
								Sensor_State=SENSOR_OK;
                            }
                            if (rx1Buffer[6] != 0x2D) {
                                currentData.comm_S1PPM = (rx1Buffer[6] - 0x30) * 100 + (rx1Buffer[7] - 0x30) * 10 + (rx1Buffer[8] - 0x30);
                            }
                            ComCouter++;

                            if (rx1Buffer[9] == ',') {
                                currentData.S1mVolt = (rx1Buffer[10] - 0x30) * 1000 + (rx1Buffer[11] - 0x30) * 100 + (rx1Buffer[12] - 0x30) * 10 + (rx1Buffer[13] - 0x30);
                                currentData.S1mV = currentData.S1mVolt;
                            }

                            S1PPm_Filter_OUT_function();
                            S1PPm_Data_offset_function();
                            RedrawValue();

                            if (rx1Buffer[14] == ',') {
                                currentData.temperature = (rx1Buffer[15] - 0x30) * 1000 + (rx1Buffer[16] - 0x30) * 100 + (rx1Buffer[17] - 0x30) * 10 + (rx1Buffer[18] - 0x30);
                            }
                            comm_run_flag = 2;
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'D' && rx1Buffer[5] == ',') {
                            REqParmeterPPM = (rx1Buffer[6] - 0x30) * 100 + (rx1Buffer[7] - 0x30) * 10 + (rx1Buffer[8] - 0x30);
                            if (REqParmeterPPM == Tx_S1manualCal && manual_cal_flag == 1) {
                                req_ppm_com_flag = 1;

                            }
                            Couter_1++;
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'C' && rx1Buffer[5] == ',') {
                            REqParmeterCurrent = (rx1Buffer[6] - 0x30) * 1000 + (rx1Buffer[7] - 0x30) * 100 + (rx1Buffer[8] - 0x30) * 10 + (rx1Buffer[9] - 0x30);
                            if (REqParmeterCurrent == SendParmeter_Current && manual_cal_flag == 1) {
                                req_current_com_flag = 1;

                            }
                            Couter_2++;
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'T' && rx1Buffer[5] == ',') {
                            ReqParmeterTemp = (rx1Buffer[6] - 0x30) * 1000 + (rx1Buffer[7] - 0x30) * 100 + (rx1Buffer[8] - 0x30) * 10 + (rx1Buffer[9] - 0x30);
                            if (ReqParmeterTemp == Tx_Adj_TempertureVaule && manual_cal_temp_flag == 1) {
                                req_temp_com_flag = 1;

                            }
                            Couter_3++;
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'B' && rx1Buffer[5] == ',') {
							
							// - �϶� ó���ؾ��ϴµ� ������ �ȵ�...���������� �ȸ���
							//if (rx1Buffer[6]=='-') REqParmeterZero_PPM = (rx1Buffer[6] - 0x30) * 100 + (rx1Buffer[7] - 0x30) * 10 + (rx1Buffer[8] - 0x30);
                            
							REqParmeterZero_PPM = (rx1Buffer[6] - 0x30) * 100 + (rx1Buffer[7] - 0x30) * 10 + (rx1Buffer[8] - 0x30);
                            if (REqParmeterZero_PPM == Tx_S1ZeroCal && zero_cal_update_flag == 1) {
                                req_zero_ppm_com_flag = 1;

                            }
                            Couter_4++;
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'A' && rx1Buffer[5] == ',') {
                            REqParmeterZero_Current = (rx1Buffer[6] - 0x30) * 1000 + (rx1Buffer[7] - 0x30) * 100 + (rx1Buffer[8] - 0x30) * 10 + (rx1Buffer[9] - 0x30);
                            if (REqParmeterZero_Current == SendParmeter_Zero_Current && req_zero_ppm_com_flag == 1) {
                                req_zero_current_com_flag = 1;

                            }
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'E' && rx1Buffer[5] == ',') {
                            ErrorCodeRecive = (rx1Buffer[6] - 0x30) * 10 + (rx1Buffer[7] - 0x30);
                        }



                    } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
//                        if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'V' && rx1Buffer[5] == ',' && manual_cal_flag == 0 && manual_cal_temp_flag == 0) {
                        if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'V' && rx1Buffer[5] == ',' && manual_cal_temp_flag == 0) {
//                            if (state == 0) {
							{
                                //DrawTextsize120(MEASURE_X1, MEASURE_Y, TEXT120_MEASURING, DRAW_IMAGE_ENABLE);
								if (WORKING_TIMER==0) { DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE); WORKING_TIMER=1;}
								else 				  { DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE); WORKING_TIMER=0;}
								Sensor_State=SENSOR_OK;
                            }
                            if (rx1Buffer[6] != 0x2D) {
                                currentData.comm_S2NTU = (rx1Buffer[6] - 0x30) * 10000 + (rx1Buffer[7] - 0x30) * 1000 + (rx1Buffer[8] - 0x30) * 100 + (rx1Buffer[9] - 0x30) * 10 + (rx1Buffer[10] - 0x30);
                                ComCouter++;
                            }

                            S1PPm_Filter_OUT_function();
                            S1PPm_Data_offset_function();
                            RedrawValue();

                            if (rx1Buffer[11] == ',') {
                                currentData.S2mVolt = (rx1Buffer[12] - 0x30) * 1000 + (rx1Buffer[13] - 0x30) * 100 + (rx1Buffer[14] - 0x30) * 10 + (rx1Buffer[15] - 0x30);
                                currentData.S2mV = currentData.S2mVolt;
                            }
                            if (rx1Buffer[16] == ',') {
                                currentData.temperature = (rx1Buffer[17] - 0x30) * 1000 + (rx1Buffer[18] - 0x30) * 100 + (rx1Buffer[19] - 0x30) * 10 + (rx1Buffer[20] - 0x30);
                            }
                            comm_run_flag = 2;
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'D' && rx1Buffer[5] == ',') {
                            REqParmeterNTU = (rx1Buffer[6] - 0x30) * 10000 + (rx1Buffer[7] - 0x30) * 1000 + (rx1Buffer[8] - 0x30) * 100 + (rx1Buffer[9] - 0x30) * 10 + (rx1Buffer[10] - 0x30);
                            if (REqParmeterNTU == Tx_S2manualCal && manual_cal_flag == 1) {
                                req_ppm_com_flag = 1;

                            }
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'A' && rx1Buffer[5] == ',') {
                            ReqParmeterZero_NTU = (rx1Buffer[6] - 0x30) * 1000 + (rx1Buffer[7] - 0x30) * 100 + (rx1Buffer[8] - 0x30)*10 + (rx1Buffer[9] - 0x30);
                            if (ReqParmeterZero_NTU == Tx_S2ZeroCal && zero_cal_update_flag == 1) {
                                req_zero_ppm_com_flag = 1;

                            }
                        } else if (rx1Buffer[1] == 'R' && rx1Buffer[2] == 'E' && rx1Buffer[3] == 'S' && rx1Buffer[4] == 'E' && rx1Buffer[5] == ',') {
                            ErrorCodeRecive = (rx1Buffer[6] - 0x30) * 10 + (rx1Buffer[7] - 0x30);
                        }
                    }
                }
                //rx1HandlerCount = 0;
                //rx1Size = 0;
            }

        }

//    } else {
//        rx1HandlerCount = 0;
    }

aaa:
*/

	 // 0=no comm, 1=DISPLAY, 2=no DISPLAY
	if (WORKING_TIMER==1) { DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE); }
	else 				  { DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE);} 

	if (ComCouter==0) RedrawValue();

	if (state==0) 			{ ComCouter++; ComCouter2=0; }
    else if (state==0x15) 	{ ComCouter=0; ComCouter2++; }




#ifndef SENSOR_PH_EC

  // ����� �ȵ����� ���� ó���ϴºκ�
	// stste=0 �� ���θ޴��̰�, state=15 �� ���ܸ��(diag) �϶��� ��� ����üũ�̴�.
	// ���θ޴������� ������ �����̹Ƿ� 4000���� �ߴ�. �� 4~5��
	// ���ܸ޴������� ������ �����̹Ƿ� 70���� �ߴ�. �� 4~5��
//    if (ComCouter2 > 100 && (state == 0 || state == 0x15)) {
//    if ((ComCouter > 150 && state==0)  || (ComCouter2 > 70 && state==0x15) ) {
//    if ((ComCouter > 800 && state==0)  || (ComCouter2 > 200 && state==0x15) ) {

//    if ((ComCouter > 1600 && state==0)  || (ComCouter2 > 500 && state==0x15) ) {
    if ((ComCouter > 1600 && state==0)  || (ComCouter2 > 500 && state==0x15) ) {
//        if (ComCouter_Prev == ComCouter) {
//            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                //DrawTextsize120(MEASURE_X2, MEASURE_Y, TEXT120_MEASURING, DRAW_IMAGE_DISABLE);
//                DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE);
//				Sensor_State=SENSOR_ERROR3;

                currentData.S1PPM = 0;
                currentData.temperature = 0;
				currentData.temperature1= 0;
                currentData.S1mV = 0;
                trans_anlog_pwmVaule = 0;
                trans_temp_pwmVaule = 0;
//                TIM8_Chage_Duty_Channel(2, trans_anlog_pwmVaule);
//                TIM8_Chage_Duty_Channel(3, trans_temp_pwmVaule);
//                RedrawValue();
//            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                //DrawTextsize120(MEASURE_X1, MEASURE_Y, TEXT120_MEASURING, DRAW_IMAGE_DISABLE);
                DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE);
				Sensor_State1=SENSOR_ERROR3;
				Sensor_State2=SENSOR_ERROR3;
				Sensor_State3=SENSOR_ERROR3;
				Sensor_State4=SENSOR_ERROR3;

#ifndef SENSOR_PH_EC
                currentData.S2PPM = 15;	// �ּҰ� 0.015 
#else
                currentData.S2PPM = 0;	// �ּҰ� 
#endif

                currentData.temperature = 0;
				currentData.temperature1= 0;
                currentData.S2mV = 0;
                trans_anlog_pwmVaule = 0;
                trans_temp_pwmVaule = 0;
                TIM8_Chage_Duty_Channel(2, trans_anlog_pwmVaule);
                TIM8_Chage_Duty_Channel(3, trans_temp_pwmVaule);
                RedrawValue();
//            }
//        }
        ComCouter_Prev = ComCouter;
        ComCouter  = 0;
        ComCouter2 = 0;
		WORKING_TIMER=0;	// no display
	}
	//else if (Sensor_State==SENSOR_ERROR3)	Sensor_State=SENSOR_OK;
//    } else if (ComCouter2 > 4001) {
//        ComCouter2 = 0;
//    }
#else
                RedrawValue();

#endif


}

void rx_bufer_clear(void) {
    int i = 0;

    for (i = 0; i < 50; i++) {
        rx1Buffer[i] = 0;
    }
}

void zero_cal_data_update(void) {
    if (zero_cal_update_flag_S2 == 1) {

    } else {

    }
}


/* old
void S1PPm_Filter_OUT_function(void) {
    if (configData.adjustConfig.filterS1 != 0) {
        if (configData.adjustConfig.filter_update_flag == 1) {
            if (conter_Filter < configData.adjustConfig.filterS1) {

                if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {

                    Filter_Data_Value[0][conter_Filter] = currentData.comm_S1PPM;
                    Filter_Data_Value[1][conter_Filter] = currentData.S1mVolt;
                } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                    Filter_Data_Value[0][conter_Filter] = currentData.comm_S2NTU;
                }

                conter_Filter++;
                if (conter_Filter >= configData.adjustConfig.filterS1) {
                    conter_Filter = 0;
                    configData.adjustConfig.filter_update_flag = 2;
                }

                if (configData.adjustConfig.filter_update_flag == 2) {
                    filterSumData = 0;
                    filterSumData2 = 0;
                    for (conter_Filter2 = 0; conter_Filter2 < configData.adjustConfig.filterS1; conter_Filter2++) {
                        filterSumData = filterSumData + (float) Filter_Data_Value[0][conter_Filter2];
                        filterSumData2 = filterSumData2 + (float) Filter_Data_Value[1][conter_Filter2];
                    }
                    filterSumData = (filterSumData / configData.adjustConfig.filterS1);
                    filterSumData2 = (filterSumData2 / configData.adjustConfig.filterS1);

                    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                        currentData.filterout_S1PPM = (uint32_t) filterSumData;
                        currentData.filterout_S1Current = (uint32_t) filterSumData2;
                    } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                        currentData.filterout_S2PPM = (uint32_t) filterSumData;
                    }
                    configData.adjustConfig.filter_update_flag = 3;
                }
            }
        }
        if (configData.adjustConfig.filter_update_flag == 3) {

            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                Filter_Data_Value[0][conter_Filter] = currentData.comm_S1PPM;
                Filter_Data_Value[1][conter_Filter] = currentData.S1mVolt;
            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                Filter_Data_Value[0][conter_Filter] = currentData.comm_S2NTU;
            }
            conter_Filter++;
            filterSumData = 0;
            filterSumData2 = 0;
            for (conter_Filter2 = 0; conter_Filter2 < configData.adjustConfig.filterS1; conter_Filter2++) {
                filterSumData = filterSumData + (float) Filter_Data_Value[0][conter_Filter2];
                filterSumData2 = filterSumData2 + (float) Filter_Data_Value[1][conter_Filter2];
            }
            filterSumData = (filterSumData / configData.adjustConfig.filterS1);
            filterSumData2 = (filterSumData2 / configData.adjustConfig.filterS1);

            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
                currentData.filterout_S1PPM = (uint32_t) filterSumData;
                currentData.filterout_S1Current = (uint32_t) filterSumData2;
            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                currentData.filterout_S2PPM = (uint32_t) filterSumData;
            }

            if (conter_Filter >= configData.adjustConfig.filterS1) {
                conter_Filter = 0;
                first_flag = 1;
            }
        }
    } else {
        configData.adjustConfig.filter_update_flag = 0;
    }

}
*/


// �����Է°��� ���Ͱ����� ���ϱ� 
uint32_t S1PPm_Filter_OUT_function(uint8_t sensor_no, uint32_t sensor) {
	static int cnt[4]={0,}, cnt2, i;
	uint32_t data=0;

	// 1. �����Է°��� �����ϱ�
	Filter_Data_Value[sensor_no][cnt[sensor_no]] = sensor;
	cnt[sensor_no]++;	if (cnt[sensor_no]>50) cnt[sensor_no]=0;

	// 2. �����Է°��� ����ϱ�
	if (configData.adjustConfig.filterS1==0) return (sensor);
	if (configData.adjustConfig.filterS1>100) return (sensor);

	cnt2 = (cnt[sensor_no]-1);	if (cnt2<0) cnt2=49;
	for (data=0, i=0;i<configData.adjustConfig.filterS1;i++)
	{
		data+=Filter_Data_Value[sensor_no][cnt2];
		cnt2--;	if (cnt2<0) cnt2=49;
	}										   
	data /=configData.adjustConfig.filterS1;

	// 3. ���� ���������� ��ġ
	return (data);
}








void S1PPm_Data_offset_function(void) {
    int i = 0;

	int32_t a,b,c,d, e,f;
	uint32_t imsi, imsi2, imsi_S1, imsi_S2;



	//=====================
	// PH ���۱��� �����ϱ�
	//=====================
	// a,b=ph4 �� ������, ������
	// c,d=ph7 �� ������, ������
	// x ���� ������
	// y ���� ������

	data_pH = currentData.comm_S1PPM;

	//data_pH = (d-b)/(c-a) * (x-a) +b;
	//data_pH = (d-b)*(data_pH-a)/(c-a) +b;
	a=configData.calibrationConfig.PH4_Value;
	b=configData.calibrationConfig.PH4_Cal;
	c=configData.calibrationConfig.PH7_Value;
	d=configData.calibrationConfig.PH7_Cal;

	imsi = (d-b)*(data_pH-a)/(c-a) +b;

	//=====================
	// ���ұ��� �����ϱ�
	//=====================
	// f:������, e:������ ������, e0:���� ������ 
	// y=f/e * e0; 	
	f = configData.calibrationConfig.PH_Span_Cal;
	e = configData.calibrationConfig.PH_Span_Value;
	imsi_S1 = imsi*f/e;

	Sensor1_OK_TIME=10;	// 10=10sec



	//=====================
	// EC ���� �����ϱ�
	//=====================
	// a,b=���α����� ������, ������
	// c,d=���ұ����� ������, ������
	// x ���� ������
	// y ���� ������

	data_EC = currentData.comm_S2NTU;

	//data_pH = (d-b)/(c-a) * (x-a) +b;
	//data_pH = (d-b)*(data_pH-a)/(c-a) +b;
	a=configData.calibrationConfig.EC_Value;
	b=configData.calibrationConfig.EC_Cal;
	c=configData.calibrationConfig.EC_Span_Value;
	d=configData.calibrationConfig.EC_Span_Cal;

	imsi_S2 = (d-b)*(data_EC-a)/(c-a) +b;
	Sensor2_OK_TIME=10;	// 10=10sec








	//===========================
	// ����, offset �����ϱ�
	//===========================

    //if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
	{
//        if (configData.adjustConfig.filter_update_flag == 0) {
            Adj_gradientS1 = (imsi_S1 * 0.01) * (configData.adjustConfig.gradientS1 * 0.01);
            Adj_offsetS1 = (configData.adjustConfig.offsetS1 * 0.01);

            Adj_gradientS2 = (imsi_S2 * 0.001) * (configData.adjustConfig.gradientS2 * 0.01);
            Adj_offsetS2 = (configData.adjustConfig.offsetS2 * 0.01);

//        } else {
//            if (first_flag == 1) {
//                Adj_gradientS1 = (currentData.filterout_S1PPM * 0.01) * (configData.adjustConfig.gradientS1 * 0.01);
//                Adj_offsetS1 = (configData.adjustConfig.offsetS1 * 0.01);
                currentData.S1mVolt = currentData.filterout_S1Current;
  //          } else if (first_flag == 0) {
//                Adj_gradientS2 = (currentData.comm_S1PPM * 0.01) * (configData.adjustConfig.gradientS1 * 0.01);
//                Adj_offsetS2 = (configData.adjustConfig.offsetS1 * 0.01);
//            }
//        }
//        if (first_flag == 0 && configData.adjustConfig.filter_update_flag != 0) {
//            currentData.S1PPM = (uint32_t) ((Adj_gradientS2 + Adj_offsetS2)*100);
//        } else 
			currentData.S1PPM = (uint32_t) ((Adj_gradientS1 + Adj_offsetS1)*100);
            currentData.S2PPM = (uint32_t) ((Adj_gradientS2 + Adj_offsetS2)*1000);


#ifndef SENSOR_PH_EC
			if (currentData.S2PPM <15) currentData.S2PPM=15;	// �ּҰ� 0.015 Ź�� 
#else 
            currentData.S2PPM = (uint32_t) ((Adj_gradientS2 + Adj_offsetS2/100)*1000);
			if (currentData.S2PPM <0) currentData.S2PPM=0;	// �ּҰ� 0.015
#endif

/*
        if (couter_vaule_ppm < 60) {
            Mesure_Data_Value[0][couter_vaule_ppm] = currentData.S1PPM;
            Mesure_Data_Value[1][couter_vaule_ppm] = currentData.temperature;
            Mesure_Data_Value[2][couter_vaule_ppm] = currentData.S2PPM;
            Mesure_Data_Value[3][couter_vaule_ppm] = currentData.temperature1;
            couter_vaule_ppm++;
            if (couter_vaule_ppm == 60) {
                if (WirteSDCard_flag == 0) {

                    for (i = 0; i < 60; i++) {
                        PPmSD_card_Data[i] = Mesure_Data_Value[0][i];
                        TEMP_card_Data[i] = Mesure_Data_Value[1][i];
                        PPmSD_card_Data2[i] = Mesure_Data_Value[2][i];
                        TEMP_card_Data2[i] = Mesure_Data_Value[3][i];
                    }
                    WirteSDCard_flag = 1;
                }
                couter_vaule_ppm = 0;
            }
*/
        if (couter_vaule_ppm < 60) {
            PPmSD_card_Data[couter_vaule_ppm] = currentData.S1PPM;
            TEMP_card_Data[couter_vaule_ppm] = currentData.temperature;
            PPmSD_card_Data2[couter_vaule_ppm] = currentData.S2PPM;
            TEMP_card_Data2[couter_vaule_ppm] = currentData.temperature1;
            couter_vaule_ppm++;
            if (couter_vaule_ppm == 60) {
                if (WirteSDCard_flag == 0) {
                    WirteSDCard_flag = 1;
                }
                couter_vaule_ppm = 0;
            }
        }
    }
}


//==================================

void SendCalData000(char dest, uint8_t name, uint32_t vaule) {
    sprintf(strBuffer2, "%5d", vaule);
    if (strBuffer2[0] == 0x20) strBuffer2[0] = '0';
    if (strBuffer2[1] == 0x20) strBuffer2[1] = '0';
    if (strBuffer2[2] == 0x20) strBuffer2[2] = '0';
    if (strBuffer2[3] == 0x20) strBuffer2[3] = '0';
    if (strBuffer2[4] == 0x20) strBuffer2[4] = '0';


    if (dest==1) tx1Buffer[0] = '$';
    if (dest==2) tx1Buffer[0] = '%';
    if (dest==3) tx1Buffer[0] = '&';
    if (dest==4) tx1Buffer[0] = 39;
    tx1Buffer[1] = 'S';
    tx1Buffer[2] = 'E';
    tx1Buffer[3] = 'T';
    tx1Buffer[4] = name;
    tx1Buffer[5] = ',';

    tx1Buffer[6] = strBuffer2[0];
    tx1Buffer[7] = strBuffer2[1];
    tx1Buffer[8] = strBuffer2[2];
    tx1Buffer[9] = strBuffer2[3];
    tx1Buffer[10]= strBuffer2[4];
    tx1Buffer[11]= '\n';

    tx1Size = 12;
    tx1Count = 1;

    USART_SendData(USART1, tx1Buffer[0]);
    USART1->CR1 |= 0x40;
}



void SendCalData(char dest, uint32_t value) {
	SendCalData000(dest, 'D', value);
}

void SendCalDataCurrent(char dest, uint32_t value) {
	SendCalData000(dest, 'C', value);
}

void SendCalZeroData(char dest, uint32_t value) {
	SendCalData000(dest, 'B', value);
}

void SendCalZeroDataCurrent(char dest, uint32_t value) {
	SendCalData000(dest, 'A', value);
}

void SendCalDataTemp(char dest, uint32_t value) {
	SendCalData000(dest, 'T', value);
}





//============================================					 

void Modbus232Handler(void) {
    uint16_t crc = 0;
    uint16_t startAddr = 0, addrCount = 0, i = 0;
    uint32_t tmp = 0;

    if (rx2Size > 0) {
        if (flag10ms & FLAG10MS_RX2) {
            flag10ms &= ~FLAG10MS_RX2;
            ++rx2HandlerCount;
            if (rx2HandlerCount > 1) {
                if (rx2Buffer[1] == 0x03) {
                    if (rx2Buffer[0] == configData.modbusConfig.modbusSlaveAddr + 1) {
                        if (rx2Size == 8) {
                            startAddr = rx2Buffer[2] * 256 + rx2Buffer[3];
                            addrCount = rx2Buffer[4] * 256 + rx2Buffer[5];
                            crc = CRC16Modbus(rx2Buffer, 6);
                            if (startAddr < 3) {
                                if (addrCount < 11 && addrCount > 0) {
                                    if (((((crc & 0xFF00) >> 8)&0xFF) == rx2Buffer[7]) && ((crc & 0xFF) == rx2Buffer[6])) {
                                        tx2Buffer[0] = configData.modbusConfig.modbusSlaveAddr + 1;
                                        tx2Buffer[1] = 0x03;
                                        tx2Buffer[2] = addrCount * 2;

                                        tx2Buffering[0] = (uint16_t) currentData.S1PPM & 0xFFFF;
                                        tx2Buffering[1] = (uint16_t) currentData.S2PPM & 0xFFFF;
                                        tx2Buffering[2] = (uint16_t) currentData.temperature & 0xFFFF;
                                        tx2Buffering[3] = (uint16_t) currentData.temperature1& 0xFFFF;
                                        tx2Buffering[4] = 0; //
                                        tx2Buffering[5] = 0; //

                                        for (i = 0; i < addrCount; ++i) {
                                            tx2Buffer[3 + (i * 2)] = ((tx2Buffering[startAddr + i]&0xFF00) >> 8)&0xFF;
                                            tx2Buffer[3 + (i * 2) + 1] = tx2Buffering[startAddr + i]&0xFF;
                                        }

                                        crc = CRC16Modbus(tx2Buffer, 3 + (addrCount * 2));
                                        tx2Buffer[3 + (addrCount * 2)] = crc & 0xFF;
                                        tx2Buffer[3 + (addrCount * 2) + 1] = ((crc & 0xFF00) >> 8);
                                        tx2Size = 3 + (addrCount * 2) + 2;
                                        tx2Count = 1;
                                        USART_SendData(USART2, tx2Buffer[0]);
                                        USART2->CR1 |= 0x40;

										comm_type=COMM_RS232;
										rx2_time_count=0;

                                    }
                                }
                            }
                        }
                    }
                } else if (rx2Buffer[1] == 0x10) {
                    if (rx2Buffer[0] == configData.modbusConfig.modbusSlaveAddr + 1) {
                        startAddr = rx2Buffer[2] * 256 + rx2Buffer[3];
                        addrCount = rx2Buffer[4] * 256 + rx2Buffer[5];
                        tmp = addrCount * 2 + 7;
                        crc = CRC16Modbus(rx2Buffer, tmp);

                        if (rx2Size == (tmp + 2)) {
                            if (startAddr == 1000) {
                                if (addrCount == 1) {
                                    if (((((crc & 0xFF00) >> 8)&0xFF) == rx2Buffer[tmp + 1]) && ((crc & 0xFF) == rx2Buffer[tmp])) {
                                        getModbusValue = (rx2Buffer[7] * 256) + rx2Buffer[8];

                                        tx2Buffer[0] = configData.modbusConfig.modbusSlaveAddr + 1;
                                        tx2Buffer[1] = 0x10;
                                        tx2Buffer[2] = rx2Buffer[2];
                                        tx2Buffer[3] = rx2Buffer[3];
                                        tx2Buffer[4] = rx2Buffer[4];
                                        tx2Buffer[5] = rx2Buffer[5];
                                        crc = CRC16Modbus(tx2Buffer, 6);
                                        tx2Buffer[6] = crc & 0xFF;
                                        tx2Buffer[7] = ((crc & 0xFF00) >> 8);

                                        tx2Size = 8;
                                        tx2Count = 1;
                                        USART_SendData(USART2, tx2Buffer[0]);
                                        USART2->CR1 |= 0x40;

										comm_type=COMM_RS232;
										rx2_time_count=0;
                                    }
                                }
                            }
                        }
                    }
                }

                rx2HandlerCount = 0;
                rx2Size = 0;
            }
        }
    } else
	{
        rx2HandlerCount = 0;
		if (rx2_time_count>100)
		{
			comm_type=COMM_4_20MA;
		}
	}

}




#ifdef  SENSOR_PH_EC
void init_tx3Buffer(void)
{
    uint16_t crc = 0;

    // ������ ��û�ϱ�
    RS485_DRIVE_HIGH;

    Delay_10msec(300);  /* RS485 ?? ??? (??: 10?300) */

    /* SENSOR_PH_EC: 항상 pH(addr=2)부터 시작 -> pH->EC->pH 교대 폴링 */
    tx3Buffer[0] = 2;

    tx3Buffer[1] = 3;

    tx3Buffer[2] = 0;
	{ ph_temp=0; tx3Buffer[3] = 0; }	// 0=temp, 1=ph

    tx3Buffer[4] = 0;
    tx3Buffer[5] = 1;


 
    crc = CRC16Modbus(tx3Buffer, 6);
    tx3Buffer[6] = crc & 0xFF;
    tx3Buffer[7] = ((crc & 0xFF00) >> 8);

    tx3Size = 8;
    tx3Count = 1;
    com485State = 1;
    rs485DriveCount = 0;

	comm_type=COMM_RS485;
	rx3_time_count=0;

}

#endif










void Modbus485Handler(void) {
    uint16_t crc = 0;
    uint16_t startAddr = 0, addrCount = 0, i = 0;
    uint32_t tmp = 0;
	static uint32_t init_count=0;

	int32_t a,b,c,d, e,f;
	uint32_t imsi, imsi2;

    switch (com485State) {
        case 0:



#ifdef  SENSOR_PH_EC
	// ����� �ȵǸ� ó������ �ٽ� ��Žõ��Ѵ�.
	init_count++;
	if (init_count>10) {
		init_tx3Buffer();
		init_count=0;
	}
#endif



            if (rx3Size > 0) {
                if (flag10ms & FLAG10MS_RX3) {
                    flag10ms &= ~FLAG10MS_RX3;
                    ++rx3HandlerCount;



#ifdef  SENSOR_PH_EC
                        if (rx3Buffer[1] == 0x03) {

							// 2. receive pH data (addr=2)	 and TEMP data
                            if (rx3Buffer[0] == 2) {
                                if (rx3Size == 7) {
									rx3Size = 0;
                                    crc = CRC16Modbus(rx3Buffer, 5);
									if (crc==((rx3Buffer[6]<<8) | rx3Buffer[5]))
									{

										//  currentData.S1PPM = data_pH;
										//	currentData.temperature = data_TEMP*10;

										//=====================
										// PH, �µ� ������ �޾Ƽ� �Ի��ϱ�
										//=====================

										if ( ph_temp==1 && init_count>1 && init_count<9)		{
											data_pH   = ((rx3Buffer[3]<<8) | rx3Buffer[4]);

											if (data_pH<0) 		data_pH=0;
											if (data_pH>1400) 	data_pH=1400;
	
	
											//=====================
											// ���۱��� �����ϱ�
											//=====================
											// a,b=ph4 �� ������, ������
											// c,d=ph7 �� ������, ������
											// x ���� ������
											// y ���� ������
	
											//data_pH = (d-b)/(c-a) * (x-a) +b;
											//data_pH = (d-b)*(data_pH-a)/(c-a) +b;
											a=configData.calibrationConfig.PH4_Value;
											b=configData.calibrationConfig.PH4_Cal;
											c=configData.calibrationConfig.PH7_Value;
											d=configData.calibrationConfig.PH7_Cal;
	
											data_pH_imsi = (d-b)*(data_pH-a)/(c-a) +b;
	
											//=====================
											// ���ұ��� �����ϱ�
											//=====================
											// f:������, e:������ ������, e0:���� ������ 
											// y=f/e * e0; 	
											f = configData.calibrationConfig.PH_Span_Cal;
											e = configData.calibrationConfig.PH_Span_Value;
											imsi2 = data_pH_imsi*f/e;
	
											currentData.S1PPM = imsi2;
											Sensor1_OK_TIME=10;	// 10=10sec


										}
										else if (init_count>1 && init_count<9) {
											data_TEMP = ((rx3Buffer[3]<<8) | rx3Buffer[4])*10;

											//=====================
											// �µ� ������ �޾Ƽ� �Ի��ϱ�
											//=====================
											// f:������, e:������ ������, e0:���� ������ 
											// y=f/e * e0; 	
											f = configData.calibrationConfig.TEMP_Span_Cal1;
											e = configData.calibrationConfig.TEMP_Span_Value1;
											imsi2 = data_TEMP*f/e;
	
											currentData.temperature = imsi2;
											//6 = data_TEMP;
										  	if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
												SET_data_TEMP = data_TEMP;
											}
										}


										Sensor_State3=SENSOR_OK;
										Sensor1_OK_TIME=10;	// 10=10sec


										//=====================
									    // 다음 요청: pH 처리 완료 -> EC(addr=4) 요청 (교대 폴링)
										//=====================
                                        RS485_DRIVE_HIGH;
                                        tx3Buffer[0] = 4;  /* 다음은 EC(addr=4) */
                                        tx3Buffer[1] = 3;

                                        tx3Buffer[2] = 0;
										{ ph_temp=0; tx3Buffer[3] = 0; }	// EC는 temp(reg0)부터

                                        tx3Buffer[4] = 0;
                                        tx3Buffer[5] = 1;

									    crc = CRC16Modbus(tx3Buffer, 6);
									    tx3Buffer[6] = crc & 0xFF;
									    tx3Buffer[7] = ((crc & 0xFF00) >> 8);

                                        tx3Size = 8;
                                        tx3Count = 1;

	                                    com485State = 1;
	                                    rs485DriveCount = 0;
	
										comm_type=COMM_RS485;
										rx3_time_count=0;
									}
                                }
                            }


							// 3. receive EC data (addr=4)
                            else if (rx3Buffer[0] == 4) {
                                if (rx3Size == 7 || rx3Size == 15 ) {
	                                if (rx3Size == 7 ) 			crc = CRC16Modbus(rx3Buffer, 5);
	                                else if (rx3Size == 15 ) 	crc = CRC16Modbus(&rx3Buffer[8], 5);

									if (crc==((rx3Buffer[6]<<8) | rx3Buffer[5]) || crc==((rx3Buffer[8+6]<<8) | rx3Buffer[8+5]))
									{
										if ( ph_temp==1 && init_count>1 && init_count<8)	{
											if (rx3Size==7)	data_EC   = ((rx3Buffer[3]<<8) | rx3Buffer[4]);
											else if (rx3Size==15)	data_EC   = ((rx3Buffer[8+3]<<8) | rx3Buffer[8+4]);

											data_EC *=9.999;
											if (data_EC<0) data_EC=0;
											if (data_EC>20000) data_EC=20000;
	
	
	
	
											//=====================
											// ���� �����ϱ�
											//=====================
											// a,b=���α����� ������, ������
											// c,d=���ұ����� ������, ������
											// x ���� ������
											// y ���� ������
	
											//data_pH = (d-b)/(c-a) * (x-a) +b;
											//data_pH = (d-b)*(data_pH-a)/(c-a) +b;
											a=configData.calibrationConfig.EC_Value;
											b=configData.calibrationConfig.EC_Cal;
											c=configData.calibrationConfig.EC_Span_Value;
											d=configData.calibrationConfig.EC_Span_Cal;
	
											currentData.S2PPM = (d-b)*(data_EC-a)/(c-a) +b;
											if (currentData.S2PPM<0) 		currentData.S2PPM=0;
											if (currentData.S2PPM>20000) 	currentData.S2PPM=20000;

											Sensor2_OK_TIME=10;	// 10=10sec

	
										}
										else if (init_count>1 && init_count<8){
	
											if (rx3Size==7)	data_TEMP = ((rx3Buffer[3]<<8) | rx3Buffer[4])*10;
											else if (rx3Size==15)	data_TEMP = ((rx3Buffer[8+3]<<8) | rx3Buffer[8+4])*10;
										 	

	
											//=====================
											// �µ� ������ �޾Ƽ� �Ի��ϱ�
											//=====================
											// f:������, e:������ ������, e0:���� ������ 
											// y=f/e * e0; 	
											f = configData.calibrationConfig.TEMP_Span_Cal2;
											e = configData.calibrationConfig.TEMP_Span_Value2;
											imsi2 = data_TEMP*f/e;
	
											currentData.temperature1 = imsi2;
	
											//currentData.S2PPM = data_EC;
											//currentData.temperature = data_TEMP*10;
										  	if (currentData.Device_Selector_Mode & SENSOR_1_MODE) { }
											else {
												SET_data_TEMP = data_TEMP;
											}	
										}


										Sensor_State4=SENSOR_OK;
										Sensor2_OK_TIME=10;	// 10=10sec


									    // ������ ��û�ϱ�
                                        RS485_DRIVE_HIGH;
                                        tx3Buffer[0] = 4;
                                        tx3Buffer[1] = 3;

                                        tx3Buffer[2] = 0;
										if (ph_temp==0)   { ph_temp=1; tx3Buffer[3] = 1; }// 0=temp, 1=ph
										else			  { ph_temp=0; tx3Buffer[3] = 0; }	// 0=temp, 1=ph

                                        tx3Buffer[4] = 0;
                                        tx3Buffer[5] = 1;

										crc = CRC16Modbus(tx3Buffer, 6);
										tx3Buffer[6] = crc & 0xFF;
										tx3Buffer[7] = ((crc & 0xFF00) >> 8);

                                        tx3Size = 8;
                                        tx3Count = 1;

	                                    com485State = 1;
	                                    rs485DriveCount = 0;
	
										comm_type=COMM_RS485;
										rx3_time_count=0;
									}
									rx3Size = 0;

                                }
                            }
					}
#endif




                    if (rx3HandlerCount > 1) {

						// function = 03
                        if (rx3Buffer[1] == 0x03) {
//#ifndef  SENSOR_PH_EC
							// 1. if master request, then send data
                            if (rx3Buffer[0] == configData.modbusConfig.modbusSlaveAddr + 1) {
                                if (rx3Size == 8) {
                                    startAddr = rx3Buffer[2] * 256 + rx3Buffer[3];
                                    addrCount = rx3Buffer[4] * 256 + rx3Buffer[5];
                                    crc = CRC16Modbus(rx3Buffer, 6);
                                    if (startAddr < 3) {
                                        if (addrCount < 11 && addrCount > 0) {
                                            if (((((crc & 0xFF00) >> 8)&0xFF) == rx3Buffer[7]) && ((crc & 0xFF) == rx3Buffer[6])) {
                                                RS485_DRIVE_HIGH;
                                                tx3Buffer[0] = configData.modbusConfig.modbusSlaveAddr + 1;
                                                tx3Buffer[1] = 0x03;
                                                tx3Buffer[2] = addrCount * 2;

                                                tx3Buffering[0] = (uint16_t) currentData.S1PPM & 0xFFFF;
                                                tx3Buffering[1] = (uint16_t) currentData.S2PPM & 0xFFFF;
                                                tx3Buffering[2] = (uint16_t) currentData.temperature & 0xFFFF;
                                                tx3Buffering[3] = (uint16_t) currentData.temperature1 & 0xFFFF;
                                                tx3Buffering[4] = 0; //
                                                tx3Buffering[5] = 0; //

                                                for (i = 0; i < addrCount; ++i) {
                                                    tx3Buffer[3 + (i * 2)] = ((tx3Buffering[startAddr + i]&0xFF00) >> 8)&0xFF;
                                                    tx3Buffer[3 + (i * 2) + 1] = tx3Buffering[startAddr + i]&0xFF;
                                                }

                                                crc = CRC16Modbus(tx3Buffer, 3 + (addrCount * 2));
                                                tx3Buffer[3 + (addrCount * 2)] = crc & 0xFF;
                                                tx3Buffer[3 + (addrCount * 2) + 1] = ((crc & 0xFF00) >> 8);
                                                tx3Size = 3 + (addrCount * 2) + 2;
                                                tx3Count = 1;
                                                com485State = 1;
                                                rs485DriveCount = 0;

												comm_type=COMM_RS485;
												rx3_time_count=0;

                                            }
                                        }
                                    }
                                }
                            }
//#else
//#endif



                        } else if (rx3Buffer[1] == 0x10) {
                            if (rx3Buffer[0] == configData.modbusConfig.modbusSlaveAddr + 1) {
                                startAddr = rx3Buffer[2] * 256 + rx3Buffer[3];
                                addrCount = rx3Buffer[4] * 256 + rx3Buffer[5];
                                tmp = addrCount * 2 + 7;
                                crc = CRC16Modbus(rx3Buffer, tmp);

                                if (rx3Size == (tmp + 2)) {
                                    if (startAddr == 1000) {
                                        if (addrCount == 1) {
                                            if (((((crc & 0xFF00) >> 8)&0xFF) == rx3Buffer[tmp + 1]) && ((crc & 0xFF) == rx3Buffer[tmp])) {
                                                RS485_DRIVE_HIGH;
                                                getModbusValue = (rx3Buffer[7] * 256) + rx3Buffer[8];

                                                tx3Buffer[0] = configData.modbusConfig.modbusSlaveAddr + 1;
                                                tx3Buffer[1] = 0x10;
                                                tx3Buffer[2] = rx3Buffer[2];
                                                tx3Buffer[3] = rx3Buffer[3];
                                                tx3Buffer[4] = rx3Buffer[4];
                                                tx3Buffer[5] = rx3Buffer[5];
                                                crc = CRC16Modbus(tx3Buffer, 6);
                                                tx3Buffer[6] = crc & 0xFF;
                                                tx3Buffer[7] = ((crc & 0xFF00) >> 8);

                                                tx3Size = 8;
                                                tx3Count = 1;
                                                com485State = 1;
                                                rs485DriveCount = 0;

												comm_type=COMM_RS485;
												rx3_time_count=0;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        rx3HandlerCount = 0;
                        rx3Size = 0;
                    }
                }
            } else {
                rx3HandlerCount = 0;
				if (rx3_time_count>100)
				{
					comm_type=COMM_4_20MA;
				}

			}
            break;
        case 1:		// 485�����ϴ°�, �ѹ������ٰ� �ι�����, ù° ����Ʈ�� �����ϰ� com485state=2�� �Ѵ�.
            if (flag10ms & FLAG10MS_RS485) {
                flag10ms &= ~FLAG10MS_RS485;
                ++rs485DriveCount;
                if (rs485DriveCount > 1) {
                    USART_SendData(USART3, tx3Buffer[0]);
                    USART3->CR1 |= 0x40;
                    rs485DriveCount = 0;
                    com485State = 2;
                }
            }
            break;
        case 2:	// INT ���� ��ü�����ϱ⸦ ��ٸ���., INT ���� ��ü�����ϸ� com485state=3�� �ȴ�.
            break;
        case 3:	// ��ü �����̳����� �ѹ������ٰ� �ι�°�� 485������ ������.
            if (flag10ms & FLAG10MS_RS485) {
                RS485_DRIVE_LOW;
				init_count=0;
                flag10ms &= ~FLAG10MS_RS485;
                ++rs485DriveCount;
                if (rs485DriveCount > 1) {
                    RS485_DRIVE_LOW;
                    rs485DriveCount = 0;
                    com485State = 0;
                }
            }
            break;
        case 4:
            break;
    }
}

uint16_t CRC16Modbus(uint16_t* nData, uint16_t wLength) {
    uint16_t nTemp = 0;
    uint16_t index = 0;
    uint16_t wCRCWord = 0xFFFF;

    while ((wLength--) != 0) {
        nTemp = (uint16_t) (((nData[index]) ^ wCRCWord)&0xFF);
        index++;
        wCRCWord >>= 8;
        wCRCWord ^= CRC16_Table[nTemp];
    }
    return wCRCWord;
}

const uint16_t CRC16_Table[256] = {0x0, 0xC0C1, 0xC181, 0x140, 0xC301, 0x3C0, 0x280, 0xC241, 0xC601, 0x6C0, 0x780, 0xC741, 0x500, 0xC5C1, 0xC481, 0x440, 0xCC01, 0xCC0, 0xD80, 0xCD41,
    0xF00, 0xCFC1, 0xCE81, 0xE40, 0xA00, 0xCAC1, 0xCB81, 0xB40, 0xC901, 0x9C0, 0x880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00,
    0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1,
    0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41,
    0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40,
    0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01,
    0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};
const uint16_t POLYNOMIAL = 0x8005;


