/**
 * Communication.c - Modbus RS232/RS485 handlers (unified firmware)
 *
 * RS485: pH (slave 2), EC (slave 4) -> currentData + water_data
 * RS232: CL ($RES1), NTU ($RES2) -> currentData + water_data
 */
#include "Communication.h"
#include "Sensor_Manager.h"

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

// RS232, RS485 �??���� 4-20Ma �� �����?�
uint16_t rx2_time_count=0, rx3_time_count=0;


extern uint16_t data_pH, data_pH_imsi, data_EC;		// ph = 0.00  temp = 0.0  ec=0.0
extern int16_t data_TEMP, SET_data_TEMP;

char  ph_temp=0;	// ph �?� temp �����?��, ?���?� �?��?��? �?? �?� �����...

/* Expected slave addr for current request - only accept matching response (multi-drop safe) */
static uint8_t s_expected_rs485_addr = 0;
static uint8_t s_rs485_sensor_index = 0;
static uint8_t s_rs485_started = 0;

static uint8_t rs485_sensor_addr(uint8_t sensor_index)
{
    if (sensor_index == 0) return configData.modbusConfig.modbusSensor1Addr;
    return configData.modbusConfig.modbusSensor2Addr;
}

static void rs485_queue_request(uint8_t sensor_addr)
{
    uint16_t crc = 0;

    tx3Buffer[0] = sensor_addr;
    tx3Buffer[1] = 3;
    tx3Buffer[2] = 0;
    tx3Buffer[3] = 0;
    tx3Buffer[4] = 0;
    tx3Buffer[5] = 4;

    crc = CRC16Modbus(tx3Buffer, 6);
    tx3Buffer[6] = crc & 0xFF;
    tx3Buffer[7] = ((crc & 0xFF00) >> 8);

    tx3Size = 8;
    tx3Count = 1;
    s_expected_rs485_addr = sensor_addr;
    com485State = 1;
    rs485DriveCount = 0;
    comm_type = COMM_RS485;
    rx3_time_count = 0;
}

static void rs485_advance_request(void)
{
    s_rs485_sensor_index ^= 1;
}

unsigned char	Sensor1_OK_TIME=20;
unsigned char	Sensor2_OK_TIME=20;
unsigned char	Sensor3_OK_TIME=20;
unsigned char	Sensor4_OK_TIME=20;


void SensorComHandler(void) {


	 // 0=no comm, 1=DISPLAY, 2=no DISPLAY
	if (WORKING_TIMER==1) { DrawIcon(ICON_WORKING, DRAW_IMAGE_ENABLE); }
	else 				  { DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE);} 

	if (ComCouter==0) RedrawValue();

	if (state==0) 			{ ComCouter++; ComCouter2=0; }
    else if (state==0x15) 	{ ComCouter=0; ComCouter2++; }


	/* 통합 펌웨어: 통신 타임아웃 시 에러값 설정 (CL/NTU/PH/EC 공통) */
	if ((ComCouter > 1600 && state==0) || (ComCouter2 > 500 && state==0x15)) {
		DrawIcon(ICON_WORKING, DRAW_IMAGE_DISABLE);
		Sensor_State1=SENSOR_ERROR3;
		Sensor_State2=SENSOR_ERROR3;
		Sensor_State3=SENSOR_ERROR3;
		Sensor_State4=SENSOR_ERROR3;

		currentData.S1PPM = 0;
		currentData.temperature = 0;
		currentData.temperature1 = 0;
		currentData.S1mV = 0;
		currentData.S2PPM = (sensor_manager_get_display_field(1) == WATER_FIELD_NTU) ? 15 : 0;
		currentData.S2mV = 0;
		trans_anlog_pwmVaule = 0;
		trans_temp_pwmVaule = 0;
		TIM8_Chage_Duty_Channel(2, trans_anlog_pwmVaule);
		TIM8_Chage_Duty_Channel(3, trans_temp_pwmVaule);
		RedrawValue();

		ComCouter_Prev = ComCouter;
		ComCouter = 0;
		ComCouter2 = 0;
		WORKING_TIMER = 0;
	}


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


// �����?���� ���?����� ���?� 
uint32_t S1PPm_Filter_OUT_function(uint8_t sensor_no, uint32_t sensor) {
	static int cnt[4]={0,}, cnt2, i;
	uint32_t data=0;

	// 1. �����?���� �����?�
	Filter_Data_Value[sensor_no][cnt[sensor_no]] = sensor;
	cnt[sensor_no]++;	if (cnt[sensor_no]>50) cnt[sensor_no]=0;

	// 2. �����?���� ����?�
	if (configData.adjustConfig.filterS1==0) return (sensor);
	if (configData.adjustConfig.filterS1>100) return (sensor);

	cnt2 = (cnt[sensor_no]-1);	if (cnt2<0) cnt2=49;
	for (data=0, i=0;i<configData.adjustConfig.filterS1;i++)
	{
		data+=Filter_Data_Value[sensor_no][cnt2];
		cnt2--;	if (cnt2<0) cnt2=49;
	}										   
	data /=configData.adjustConfig.filterS1;

	// 3. ���� ���������� ��?
	return (data);
}


void S1PPm_Data_offset_function(void) {
    int i = 0;

	int32_t a,b,c,d, e,f;
	uint32_t imsi, imsi2, imsi_S1, imsi_S2;


	//=====================
	// PH ���?��� �����?�
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
	// ���?��� �����?�
	//=====================
	// f:������, e:������ ������, e0:���� ������ 
	// y=f/e * e0; 	
	f = configData.calibrationConfig.PH_Span_Cal;
	e = configData.calibrationConfig.PH_Span_Value;
	imsi_S1 = imsi*f/e;

	Sensor1_OK_TIME=10;	// 10=10sec


	//=====================
	// EC ���� �����?�
	//=====================
	// a,b=���?����� ������, ������
	// c,d=���?����� ������, ������
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
	// ����, offset �����?�
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
			if (sensor_manager_get_display_field(1) == WATER_FIELD_NTU) {
				currentData.S2PPM = (uint32_t) ((Adj_gradientS2 + Adj_offsetS2)*1000);
				if (currentData.S2PPM < 15) currentData.S2PPM = 15;
			} else {
				currentData.S2PPM = (uint32_t) ((Adj_gradientS2 + Adj_offsetS2/100)*1000);
				if (currentData.S2PPM < 0) currentData.S2PPM = 0;
			}


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


void init_tx3Buffer(void)
{
    rx3Size = 0;
    rx3HandlerCount = 0;

    RS485_DRIVE_HIGH;
    Delay_10msec(10);

    if (!s_rs485_started) {
        s_rs485_sensor_index = 0;
        s_rs485_started = 1;
    }

    rs485_queue_request(rs485_sensor_addr(s_rs485_sensor_index));

}


void Modbus485Handler(void) {
    uint16_t crc = 0;
    uint16_t startAddr = 0, addrCount = 0, i = 0;
    uint32_t tmp = 0;
	static uint32_t init_count=0;

	int32_t a,b,c,d, e,f;
	uint32_t imsi, imsi2;

    switch (com485State) {
        case 0:
	init_count++;
	if (init_count > 10) {
		rs485_advance_request();
		init_tx3Buffer();
		init_count = 0;
	}

            if (rx3Size > 0) {
                if (flag10ms & FLAG10MS_RX3) {
                    flag10ms &= ~FLAG10MS_RX3;
                    ++rx3HandlerCount;


                        if (rx3Buffer[1] == 0x03) {

							/* Only accept response from sensor we requested (multi-drop safe) */
							if (rx3Buffer[0] != s_expected_rs485_addr) {
								rx3Size = 0;
								rx3HandlerCount = 0;
							} else if (rx3Size == 13) {
								crc = CRC16Modbus(rx3Buffer, 11);
								if (crc == ((rx3Buffer[12] << 8) | rx3Buffer[11])) {
									uint16_t r0 = (rx3Buffer[3] << 8) | rx3Buffer[4];
									uint16_t r1 = (rx3Buffer[5] << 8) | rx3Buffer[6];
									uint16_t r2 = (rx3Buffer[7] << 8) | rx3Buffer[8];
									uint16_t r3 = (rx3Buffer[9] << 8) | rx3Buffer[10];

									if (r0 <= 14 && r1 < 100) {
										data_pH = r0 * 100 + r1;
										if (data_pH > 1400) data_pH = 1400;
										a = configData.calibrationConfig.PH4_Value;
										b = configData.calibrationConfig.PH4_Cal;
										c = configData.calibrationConfig.PH7_Value;
										d = configData.calibrationConfig.PH7_Cal;
										data_pH_imsi = (c != a) ? ((d - b) * (data_pH - a) / (c - a) + b) : b;
										f = configData.calibrationConfig.PH_Span_Cal;
										e = configData.calibrationConfig.PH_Span_Value;
										currentData.S1PPM = (e != 0) ? (data_pH_imsi * f / e) : data_pH_imsi;
										water_data.ph = (float)currentData.S1PPM / 100.0f;

										data_TEMP = (int16_t)(r2 * 10 + r3);
										f = configData.calibrationConfig.TEMP_Span_Cal1;
										e = configData.calibrationConfig.TEMP_Span_Value1;
										currentData.temperature = (e != 0) ? (data_TEMP * f / e) : data_TEMP;
										if (currentData.Device_Selector_Mode & SENSOR_1_MODE)
											SET_data_TEMP = data_TEMP;

										Sensor_State1 = SENSOR_OK;
										Sensor_State3 = SENSOR_OK;
										Sensor1_OK_TIME = 10;
									} else {
										union { uint8_t b[4]; float f; } u;
										u.b[0] = rx3Buffer[6];
										u.b[1] = rx3Buffer[5];
										u.b[2] = rx3Buffer[4];
										u.b[3] = rx3Buffer[3];
										data_EC = (uint32_t)(u.f * 1000.0f);
										if (data_EC > 200000) data_EC = 200000;

										u.b[0] = rx3Buffer[10];
										u.b[1] = rx3Buffer[9];
										u.b[2] = rx3Buffer[8];
										u.b[3] = rx3Buffer[7];
										data_TEMP = (int16_t)(u.f * 10.0f);

										a = configData.calibrationConfig.EC_Value;
										b = configData.calibrationConfig.EC_Cal;
										c = configData.calibrationConfig.EC_Span_Value;
										d = configData.calibrationConfig.EC_Span_Cal;
										currentData.S2PPM = (c != a) ? ((d - b) * (data_EC - a) / (c - a) + b) : b;
										if ((int32_t)currentData.S2PPM < 0) currentData.S2PPM = 0;
										if (currentData.S2PPM > 200000) currentData.S2PPM = 200000;
										water_data.ec = (currentData.S2PPM >= 20000) ?
											(float)currentData.S2PPM / 10.0f : (float)currentData.S2PPM / 1000.0f;

										f = configData.calibrationConfig.TEMP_Span_Cal2;
										e = configData.calibrationConfig.TEMP_Span_Value2;
										currentData.temperature1 = (e != 0) ? (data_TEMP * f / e) : data_TEMP;
										if (!(currentData.Device_Selector_Mode & SENSOR_1_MODE))
											SET_data_TEMP = data_TEMP;

										Sensor_State2 = SENSOR_OK;
										Sensor_State4 = SENSOR_OK;
										Sensor2_OK_TIME = 10;
									}

									RS485_DRIVE_HIGH;
									rs485_advance_request();
									rs485_queue_request(rs485_sensor_addr(s_rs485_sensor_index));
								}
								rx3Size = 0;
								rx3HandlerCount = 0;
							} else if (rx3Size == 9) {
								crc = CRC16Modbus(rx3Buffer, 7);
								if (crc == ((rx3Buffer[8] << 8) | rx3Buffer[7])) {
									uint16_t r0 = (rx3Buffer[3] << 8) | rx3Buffer[4];
									uint16_t r1 = (rx3Buffer[5] << 8) | rx3Buffer[6];

									if (r0 <= 14 && r1 < 100) {
										data_pH = r0 * 100 + r1;
										if (data_pH > 1400) data_pH = 1400;
										a = configData.calibrationConfig.PH4_Value;
										b = configData.calibrationConfig.PH4_Cal;
										c = configData.calibrationConfig.PH7_Value;
										d = configData.calibrationConfig.PH7_Cal;
										data_pH_imsi = (c != a) ? ((d - b) * (data_pH - a) / (c - a) + b) : b;
										f = configData.calibrationConfig.PH_Span_Cal;
										e = configData.calibrationConfig.PH_Span_Value;
										currentData.S1PPM = (e != 0) ? (data_pH_imsi * f / e) : data_pH_imsi;
										water_data.ph = (float)currentData.S1PPM / 100.0f;
										Sensor_State1 = SENSOR_OK;
										Sensor_State3 = SENSOR_OK;
										Sensor1_OK_TIME = 10;
									} else {
										union { uint8_t b[4]; float f; } u;
										u.b[0] = rx3Buffer[6];
										u.b[1] = rx3Buffer[5];
										u.b[2] = rx3Buffer[4];
										u.b[3] = rx3Buffer[3];
										data_EC = (uint32_t)(u.f * 1000.0f);
										if (data_EC > 200000) data_EC = 200000;
										a = configData.calibrationConfig.EC_Value;
										b = configData.calibrationConfig.EC_Cal;
										c = configData.calibrationConfig.EC_Span_Value;
										d = configData.calibrationConfig.EC_Span_Cal;
										currentData.S2PPM = (c != a) ? ((d - b) * (data_EC - a) / (c - a) + b) : b;
										if ((int32_t)currentData.S2PPM < 0) currentData.S2PPM = 0;
										water_data.ec = (currentData.S2PPM >= 20000) ?
											(float)currentData.S2PPM / 10.0f : (float)currentData.S2PPM / 1000.0f;
										Sensor_State2 = SENSOR_OK;
										Sensor_State4 = SENSOR_OK;
										Sensor2_OK_TIME = 10;
									}

									RS485_DRIVE_HIGH;
									rs485_advance_request();
									rs485_queue_request(rs485_sensor_addr(s_rs485_sensor_index));
								}
								rx3Size = 0;
								rx3HandlerCount = 0;
							} else if (rx3Buffer[0] == configData.modbusConfig.modbusSensor1Addr) {
                                if (rx3Size == 7) {
									rx3Size = 0;
                                    crc = CRC16Modbus(rx3Buffer, 5);
									if (crc==((rx3Buffer[6]<<8) | rx3Buffer[5]))
									{
										if ( ph_temp==1)	{
											data_pH   = ((rx3Buffer[3]<<8) | rx3Buffer[4]);

											if (data_pH<0) 		data_pH=0;
											if (data_pH>1400) 	data_pH=1400;


											//=====================
											// ���?��� �����?�
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
											// ���?��� �����?�
											//=====================
											// f:������, e:������ ������, e0:���� ������ 
											// y=f/e * e0; 	
											f = configData.calibrationConfig.PH_Span_Cal;
											e = configData.calibrationConfig.PH_Span_Value;
											imsi2 = data_pH_imsi*f/e;

											currentData.S1PPM = imsi2;
											water_data.ph = (float)imsi2 / 100.0f;
											Sensor1_OK_TIME=10;	// 10=10sec


										}
										else {
											data_TEMP = ((rx3Buffer[3]<<8) | rx3Buffer[4])*10;

											f = configData.calibrationConfig.TEMP_Span_Cal1;
											e = configData.calibrationConfig.TEMP_Span_Value1;
											imsi2 = data_TEMP*f/e;

											currentData.temperature = imsi2;
										  	if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
												SET_data_TEMP = data_TEMP;
											}
										}

										Sensor_State3=SENSOR_OK;
										Sensor1_OK_TIME=10;	// 10=10sec


                                        RS485_DRIVE_HIGH;
										rs485_advance_request();
										rs485_queue_request(rs485_sensor_addr(s_rs485_sensor_index));
                                        rx3Size = 0;
	                                    rx3HandlerCount = 0;
										comm_type=COMM_RS485;
										rx3_time_count=0;
									}
                                }
                            }

							/* EC (addr=4) - reject concatenated/garbled */
                            else if (rx3Buffer[0] == configData.modbusConfig.modbusSensor2Addr) {
                                if (rx3Size == 7) {
	                                crc = CRC16Modbus(rx3Buffer, 5);
									if (crc==((rx3Buffer[6]<<8) | rx3Buffer[5]))
									{
										if ( ph_temp==1)	{
											data_EC = ((rx3Buffer[3]<<8) | rx3Buffer[4]);
											data_EC *=9.999;
											if (data_EC<0) data_EC=0;
											if (data_EC>20000) data_EC=20000;


											//=====================
											// ���� �����?�
											//=====================
											// a,b=���?����� ������, ������
											// c,d=���?����� ������, ������
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
											water_data.ec = (currentData.S2PPM >= 20000) ?
												(float)currentData.S2PPM / 10.0f : (float)currentData.S2PPM / 1000.0f;

											Sensor2_OK_TIME=10;	// 10=10sec


										}
										else {
											data_TEMP = ((rx3Buffer[3]<<8) | rx3Buffer[4])*10;
											f = configData.calibrationConfig.TEMP_Span_Cal2;
											e = configData.calibrationConfig.TEMP_Span_Value2;
											imsi2 = data_TEMP*f/e;
											currentData.temperature1 = imsi2;
										  	if (currentData.Device_Selector_Mode & SENSOR_1_MODE) { }
											else {
												SET_data_TEMP = data_TEMP;
											}	
										}

										Sensor_State4=SENSOR_OK;
										Sensor2_OK_TIME=10;	// 10=10sec


                                        RS485_DRIVE_HIGH;
										rs485_advance_request();
										rs485_queue_request(rs485_sensor_addr(s_rs485_sensor_index));
	                                    rx3Size = 0;
	                                    rx3HandlerCount = 0;
										comm_type=COMM_RS485;
										rx3_time_count=0;
									}
									rx3Size = 0;

                                }
                            }
					}

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
        case 1:		// 485�����?��, �?������?� �?�����, �� ����?�� �����?� com485state=2�� �?�.
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
        case 2:	// INT ���� ��������?? ��?���., INT ���� ��������?� com485state=3�� �?�.
            break;
        case 3:	// ��� �����?����� �?������?� �?���� 485������ ������.
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


