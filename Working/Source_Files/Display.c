#include "Display.h"
#include "Sensor_Manager.h"

extern uint16_t trand_select_Y, trand_select_Y_NO;		// Y ?? ?????? ?????????

// Trand ?? 60??(1??????), 6????(6??????), 12????(12??????), 24????(24??????), 168????=7??(168??????), 336????=14??(336??????)


// Trand diamond
// trendValue ?? ???? ?????? ?????.
// 2.0??? Y??? 85 ,   85 = a*2 + b		
// 0 ??? Y??? 220,    220 = a*0 + b
// a=-67.5 b=220
// y = -67.5*x + 220
// 85=GRAPH_Y_MAX, 220=GRAPH_Y_ZERO
//trand_select_Y ?? Y?? ?????? ????????? 
void DrawTrandDiamond(uint32_t cur) {
    uint32_t i = 0, j;
	uint32_t x1,y1,x2,y2;
   	uint32_t x3,y3,x4,y4;

    float data;
	float f;


	TFT_Line(GRAPH_X_START, GRAPH_Y_ZERO,GRAPH_X_MAX, GRAPH_Y_ZERO, BLACK);
	TFT_Line(GRAPH_X_START, GRAPH_Y_MAX, GRAPH_X_MAX, GRAPH_Y_MAX,  BLACK);
	TFT_Line(GRAPH_X_START, (GRAPH_Y_ZERO+GRAPH_Y_MAX)/2, GRAPH_X_MAX, (GRAPH_Y_ZERO+GRAPH_Y_MAX)/2, BLACK);

	TFT_Line(GRAPH_X_START, (GRAPH_Y_ZERO-GRAPH_Y_MAX)/4   + GRAPH_Y_MAX, GRAPH_X_MAX, (GRAPH_Y_ZERO-GRAPH_Y_MAX)/4   + GRAPH_Y_MAX, BLACK);
	TFT_Line(GRAPH_X_START, (GRAPH_Y_ZERO-GRAPH_Y_MAX)*3/4 + GRAPH_Y_MAX, GRAPH_X_MAX, (GRAPH_Y_ZERO-GRAPH_Y_MAX)*3/4 + GRAPH_Y_MAX, BLACK);


	TFT_Line(GRAPH_X_START, GRAPH_Y_ZERO, GRAPH_X_START, GRAPH_Y_MAX, BLACK);
	TFT_Line(GRAPH_X_MAX, GRAPH_Y_ZERO, GRAPH_X_MAX, GRAPH_Y_MAX, BLACK);
	TFT_Line((GRAPH_X_START+GRAPH_X_MAX)/2, GRAPH_Y_ZERO, (GRAPH_X_START+GRAPH_X_MAX)/2, GRAPH_Y_MAX, BLACK);

	TFT_Line(GRAPH_X_START + (GRAPH_X_MAX-GRAPH_X_START)/4, GRAPH_Y_ZERO, GRAPH_X_START + (GRAPH_X_MAX-GRAPH_X_START)/4, GRAPH_Y_MAX, BLACK);
	TFT_Line(GRAPH_X_START + (GRAPH_X_MAX-GRAPH_X_START)*3/4, GRAPH_Y_ZERO, GRAPH_X_START + (GRAPH_X_MAX-GRAPH_X_START)*3/4, GRAPH_Y_MAX, BLACK);


    for (i = 1; i < 61; ++i) {
		water_field_t f0 = sensor_manager_get_display_field(0);
		water_field_t f1 = sensor_manager_get_display_field(1);

	    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
			data = (trendValue[compareValue][i - 1]);
			if (f0 == WATER_FIELD_PH) {
				if (trand_select_Y==0) data=data*20/14;
				else if (trand_select_Y==1) data=data*20/10;
				else if (trand_select_Y==2) data=data*20/8;
				else if (trand_select_Y==3) data=data*20/6;
				f = 67.5*data;
				f=GRAPH_Y_ZERO-f/1000;
			} else {
				if (trand_select_Y==1) data*=2;
				else if (trand_select_Y==2) data*=4;
				f = 67.5*data;
				f=GRAPH_Y_ZERO-f/100;
			}
		} else {
			data = (trendValue[compareValue][i - 1]);
			if (f1 == WATER_FIELD_EC) {
				if (trand_select_Y==1) data*=2;
				else if (trand_select_Y==2) data*=4;
				else if (trand_select_Y==3) data*=500;
				f = 6.75*data;
				f=GRAPH_Y_ZERO-f/1000;
			} else {
				if (trand_select_Y==1) data*=2;
				else if (trand_select_Y==2) data*=20;
				else if (trand_select_Y==3) data*=40;
				f = 6.75*data;
				f=GRAPH_Y_ZERO-f/1000;
			}
		}

		x1 = GRAPH_X_START + (i*6)-3;
		x2 = GRAPH_X_START + (i*6)+3;

		y1 = f-3;	
		y2 = f+3;

		if (y1<GRAPH_Y_MAX) y1 = GRAPH_Y_MAX-3;
		if (y2<GRAPH_Y_MAX) y2 = GRAPH_Y_MAX+3;

		x3 = (x1+x2)/2;
		y3 = (y1+y2)/2;

//           	TFT_Line(x3,y1,x1,y3, RED);
//           	TFT_Line(x3,y1,x2,y3, RED);
//           	TFT_Line(x1,y3,x3,y2, RED);
//          	TFT_Line(x3,y2,x2,y3, RED);
		for (y4=y1, j=0;y4<=y2;y4++,j++)
		{
			if (j<3)	TFT_Line(x3-j,y4,x3+j,y4, RED);
			else 		TFT_Line(x3+j-6,y4,x3+6-j,y4, RED);
		}

//            TFT_Line(GRAPH_X_START + ((i - 1)*6), GRAPH_Y_ZERO - ((trendValue[compareValue][i - 1]* 180) / 200),
//                    GRAPH_X_START + (i * 6), GRAPH_Y_ZERO - ((trendValue[compareValue][i] * 180) / 200)+2, RED);
    }


//    } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
//        for (i = 1; i < 61; ++i) {
//            TFT_Line(GRAPH_X_START + ((i - 1)*6), GRAPH_Y_ZERO - (((trendValue[compareValue][i - 1] / 100)* 180) / 200),
//                    GRAPH_X_START + (i * 6), GRAPH_Y_ZERO - (((trendValue[compareValue][i] / 100) * 180) / 200)+2, RED);
//        }
//    }
}


#define trand_select_time_NO 6		// ???????? ?????????

extern uint16_t trand_select_time;
extern uint8_t trandData_TIME[trand_select_time_NO][61][6];


void DrawTrandText(uint32_t cur) {
	int imsi_text[6] = {1,6,12,24,7,14};

	{
		water_field_t f0 = sensor_manager_get_display_field(0);
		water_field_t f1 = sensor_manager_get_display_field(1);
		if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
			if (f0 == WATER_FIELD_PH) {
				if (trendValue[compareValue][cur]>1400)
					sprintf(strBuffer, "14.00 ");
				else
					sprintf(strBuffer, "%1d.%02d  ", trendValue[compareValue][cur] / 100, trendValue[compareValue][cur]-((trendValue[compareValue][cur]/100)*100));
				DrawSmallNumber(370, 60, strBuffer, CYAN);
				display_2ch_small_ph_xy(330, 60);
			} else {
				if (trendValue[compareValue][cur]>40000)
					sprintf(strBuffer, "400.00 ");
				else
					sprintf(strBuffer, "%1d.%02d   ", trendValue[compareValue][cur] / 100, trendValue[compareValue][cur]-((trendValue[compareValue][cur]/100)*100));
				DrawSmallNumber(338, 60, strBuffer, CYAN);
				display_mgl_3(410, 60);
			}
		} else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
			if (f1 == WATER_FIELD_EC) {
				if (trendValue[compareValue][cur]>20000)
					sprintf(strBuffer, "2000.0 ");
				else
					sprintf(strBuffer, "%4d.%01d ", trendValue[compareValue][cur] / 10, trendValue[compareValue][cur]-((trendValue[compareValue][cur]/10)*10));
				DrawSmallNumber(336, 60, strBuffer, CYAN);
				display_2ch_small_ec_xy(410, 63);
			} else {
				if (trendValue[compareValue][cur]>400000)
					sprintf(strBuffer, "400.00  ");
				else
					sprintf(strBuffer, "%1d.%03d   ", trendValue[compareValue][cur] / 1000, trendValue[compareValue][cur]-((trendValue[compareValue][cur]/1000)*1000));
				DrawSmallNumber(338, 60, strBuffer, CYAN);
				display_NTU_3(410, 63);
			}
		}
	}


    sprintf(strBuffer, "%04d-%02d-%02d ", trandData_TIME[trand_select_time][cur][0]+2000, trandData_TIME[trand_select_time][cur][1], trandData_TIME[trand_select_time][cur][2]);
   	DrawSmallNumber(78, 60, strBuffer, CYAN);

    sprintf(strBuffer, "%02d:%02d:%02d ", trandData_TIME[trand_select_time][cur][3], trandData_TIME[trand_select_time][cur][4], trandData_TIME[trand_select_time][cur][5]);
    DrawSmallNumber(218, 60, strBuffer, CYAN);


	display_trend_hour(trand_select_time);

  /*
    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
        sprintf(strBuffer, "%d", imsi_text[trand_select_time]);
    } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
        sprintf(strBuffer, "%d", imsi_text[trand_select_time]);
    }
	DrawSmallNumber(400, 60, strBuffer, BLUE);
	display_day_trand();
*/

}									

void ClearGraphArea(void) {
//    TFT_Fill(60, 89, 421, 270, BLACK);
	display_back_graph();
}


// ???????
void DrawCalibLog(uint32_t cur) {
    int16_t Calibration_Trans_compare_Value = 0;
    int Calibration_Trans_vaule = 0;
    int i = 0;

    sprintf(strBuffer, "%2d", cur + 1);
    DrawTinyNumber(20, 70, strBuffer, WHITE);
    sprintf(strBuffer, "%2d", cur + 2);
    DrawTinyNumber(20, 120, strBuffer, WHITE);
    sprintf(strBuffer, "%2d", cur + 3);
    DrawTinyNumber(20, 170, strBuffer, WHITE);
    sprintf(strBuffer, "%2d", cur + 4);
    DrawTinyNumber(20, 220, strBuffer, WHITE);

    sprintf(strBuffer, "%04d-%02d-%02d", calibLog[0].calibrationTime.tm_year, calibLog[0].calibrationTime.tm_mon + 1, calibLog[0].calibrationTime.tm_mday);
    DrawTinyNumber(75, 		70 - 10, strBuffer, BLACK);
    sprintf(strBuffer, "%02d:%02d:%02d", calibLog[0].calibrationTime.tm_hour, calibLog[0].calibrationTime.tm_min, calibLog[0].calibrationTime.tm_sec);
    DrawTinyNumber(75 + 15, 70 + 10, strBuffer, BLACK);
    sprintf(strBuffer, "%04d-%02d-%02d", calibLog[1].calibrationTime.tm_year, calibLog[1].calibrationTime.tm_mon + 1, calibLog[1].calibrationTime.tm_mday);
    DrawTinyNumber(75, 		120 - 10, strBuffer, BLACK);
    sprintf(strBuffer, "%02d:%02d:%02d", calibLog[1].calibrationTime.tm_hour, calibLog[1].calibrationTime.tm_min, calibLog[1].calibrationTime.tm_sec);
    DrawTinyNumber(75 + 15, 120 + 10, strBuffer, BLACK);
    sprintf(strBuffer, "%04d-%02d-%02d", calibLog[2].calibrationTime.tm_year, calibLog[2].calibrationTime.tm_mon + 1, calibLog[2].calibrationTime.tm_mday);
    DrawTinyNumber(75, 		170 - 10, strBuffer, BLACK);
    sprintf(strBuffer, "%02d:%02d:%02d", calibLog[2].calibrationTime.tm_hour, calibLog[2].calibrationTime.tm_min, calibLog[2].calibrationTime.tm_sec);
    DrawTinyNumber(75 + 15, 170 + 10, strBuffer, BLACK);
    sprintf(strBuffer, "%04d-%02d-%02d", calibLog[3].calibrationTime.tm_year, calibLog[3].calibrationTime.tm_mon + 1, calibLog[3].calibrationTime.tm_mday);
    DrawTinyNumber(75, 		220 - 10, strBuffer, BLACK);
    sprintf(strBuffer, "%02d:%02d:%02d", calibLog[3].calibrationTime.tm_hour, calibLog[3].calibrationTime.tm_min, calibLog[3].calibrationTime.tm_sec);
    DrawTinyNumber(75 + 15, 220 + 10, strBuffer, BLACK);

    for (i = 0; i < 4; i++) {
        Calibration_Trans_compare_Value = 0x80 & (int16_t) calibLog[i].calibrationValue;
        if (Calibration_Trans_compare_Value != 0)
            Calibration_Trans_vaule = (int16_t) calibLog[i].calibrationValue * (-1);


		{
			water_field_t f0 = sensor_manager_get_display_field(0);
			water_field_t f1 = sensor_manager_get_display_field(1);
			if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
				if ((int16_t) calibLog[i].calibrationValue < 0)
					sprintf(strBuffer, "-%1d.%02d", Calibration_Trans_vaule / 100, Calibration_Trans_vaule % 100);
				else
					sprintf(strBuffer, " %1d.%02d", calibLog[i].calibrationValue / 100, calibLog[i].calibrationValue % 100);
				DrawSmallNumber(350, 70 + (50 * i), strBuffer, WHITE);
			} else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
				if (f1 == WATER_FIELD_EC) {
					if (calibLog[i].calibrationMethod==20) {
						if ((int16_t) calibLog[i].calibrationValue < 0)
							sprintf(strBuffer, "-%1d.%01d", Calibration_Trans_vaule / 100, Calibration_Trans_vaule % 100);
						else
							sprintf(strBuffer, " %1d.%01d", calibLog[i].calibrationValue / 100, calibLog[i].calibrationValue % 100);
					} else {
						if ((int16_t) calibLog[i].calibrationValue < 0)
							sprintf(strBuffer, "-%1d.%01d", Calibration_Trans_vaule / 10, Calibration_Trans_vaule % 10);
						else
							sprintf(strBuffer, " %1d.%01d", calibLog[i].calibrationValue / 10, calibLog[i].calibrationValue % 10);
					}
					DrawSmallNumber(370, 70 + (50 * i), strBuffer, WHITE);
				} else {
					if ((int16_t) calibLog[i].calibrationValue < 0)
						sprintf(strBuffer, "-%1d.%03d", Calibration_Trans_vaule / 1000, Calibration_Trans_vaule % 1000);
					else
						sprintf(strBuffer, " %1d.%03d", calibLog[i].calibrationValue / 1000, calibLog[i].calibrationValue % 1000);
					DrawSmallNumber(350, 70 + (50 * i), strBuffer, WHITE);
				}
			}
		}

    }

    for (i = 0; i < 4; i++) {
        if (calibLog[i].calibrationMethod == 0) {
            calibLog[i].calibrationMethod = 7;
        }
        //DrawTextsize96(245, 70 + (50 * i), calibLog[i].calibrationMethod, DRAW_IMAGE_ENABLE);
		if (calibLog[i].calibrationMethod==15)
			display_span_W_XY(220,65+50*i);
		else if (calibLog[i].calibrationMethod==20)
			display_temp_W_XY(220,65+50*i);
		else if (calibLog[i].calibrationMethod==22) {
			if (currentData.Device_Selector_Mode & SENSOR_1_MODE && sensor_manager_get_display_field(0) == WATER_FIELD_PH)
				display_buffer_W_XY(220,65+50*i);
			else
				display_zero_W_XY(220,65+50*i);
		}
		else 
			display_clear_W_XY(220,65+50*i);
    }
}

// ??????? ???
void DrawRelay1Wash(uint32_t cur, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        if (cur == 0) {
            sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashCycle);
            //DrawMediumNumber(250, 150, strBuffer, color);
            DrawSmallNumber(250, 148, strBuffer, color);
        }
        else {
            sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashTime);
            //DrawMediumNumber(250, 200, strBuffer, color);
            DrawSmallNumber(250, 207, strBuffer, color);
        }
    } else if (currentData.Device_Selector_Mode == SENSOR_12_MODE) {
    } else {
        if (cur == 0) {
            sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashCycle);
            //DrawMediumNumber(250, 150, strBuffer, color);
            DrawSmallNumber(250, 148, strBuffer, color);
        } else if (cur == 1) {
            sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashTime);
            //DrawMediumNumber(250, 200, strBuffer, color);
            DrawSmallNumber(250, 20, strBuffer, color);
        } else {
            if (compareValue) {
                if (color == WHITE) {
                }                    //DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_W, DRAW_IMAGE_ENABLE);
                else {
                }
                //DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_Y, DRAW_IMAGE_ENABLE);
            } else {
                if (color == WHITE) {
                }                    //DrawTextsize260(110, 200, TEXT260_S2_PRIME_W, DRAW_IMAGE_ENABLE);
                else {
                }
                //DrawTextsize260(110, 200, TEXT260_S2_PRIME_Y, DRAW_IMAGE_ENABLE);
            }
        }
    }
}

/* ???? A: UNUSED - ??? ???
void DrawRelay2Alarm(uint32_t cur, uint32_t color) {
    if ((currentData.Device_Selector_Mode == SENSOR_1_MODE) || (currentData.Device_Selector_Mode == SENSOR_2_MODE)) {
        if (cur == 0) {
//            DrawTextsize55(200, 130, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
        } else {
//            DrawTextsize55(200, 130, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
        }
    } else {
        if (cur == 0) {
            //sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay2WashCycle);		
            //DrawMediumNumber(250, 100, strBuffer, color);
        } else if (cur == 1) {
            //sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay2WashTime);		
            //DrawMediumNumber(250, 150, strBuffer, color);
        } else {
            if (compareValue) {
//                if (color == WHITE)
//                    DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_W, DRAW_IMAGE_ENABLE);
//                else
//                    DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_Y, DRAW_IMAGE_ENABLE);
//            } else {
//                if (color == WHITE)
//                    DrawTextsize260(110, 200, TEXT260_S2_PRIME_W, DRAW_IMAGE_ENABLE);
//                else
//                    DrawTextsize260(110, 200, TEXT260_S2_PRIME_Y, DRAW_IMAGE_ENABLE);
            }
        }
    }
}
*/

void DrawRelay3Wash(uint32_t cur, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        if (cur == 0) {
            sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashCycle);
            //DrawMediumNumber(250, 100, strBuffer, color);
            DrawSmallNumber(220, 145, strBuffer, color);
        } else {
            sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashTime);
            //DrawMediumNumber(250, 150, strBuffer, color);
            DrawSmallNumber(220, 203, strBuffer, color);
        }
    }
    else {

    }
}

/* ???? A: UNUSED - ??? ???
void DrawRelayAlarm(uint32_t val) {
//    if (val)
//        DrawTextsize55(200, 130, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
//    else
//        DrawTextsize55(200, 130, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
}
*/
//-----------------------------------------------------------------------------------------------------------------//

void DrawAdjustFilter(uint32_t cur, uint32_t sen, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS1);
        //DrawMediumNumber(250, 165, strBuffer, color);
        //DrawSmallNumber(210, 175, strBuffer, color);
        DrawMediumNumber(210, 175, strBuffer, color);
    } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS2);
        //DrawMediumNumber(250, 165, strBuffer, color);
        //DrawSmallNumber(210, 175, strBuffer, color);
        DrawMediumNumber(210, 175, strBuffer, color);
    } else {

    }
}


//----------------------------------------------------------------------------------
// ????-??????

int32_t offsetintVal2 = 0;

void DrawAdjustOffset(uint32_t cur, uint32_t sen, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        offsetintVal  = tempConfigData.adjustConfig.offsetS1;
        offsetintVal2 = tempConfigData.adjustConfig.offsetS1;
	}
    if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        offsetintVal  = tempConfigData.adjustConfig.offsetS2;
        offsetintVal2 = tempConfigData.adjustConfig.offsetS2;
	}

	{
        if (offsetintVal2 < 0) {
            offsetintVal2 = offsetintVal2 * (-1);
        }
        if (offsetintVal < 0) {
            if (tempConfigData.adjustConfig.offsetS1 <= -1000) {
                sprintf(strBuffer, "-%2d.%02d", offsetintVal2 / 100, offsetintVal2 % 100);
                DrawMediumNumber(180, 175, strBuffer, color);
			}
			else {
                sprintf(strBuffer, "-%d.%02d", offsetintVal2 / 100, offsetintVal2 % 100);
                DrawMediumNumber(180, 175, strBuffer, color);
			}
        } else {
            if (tempConfigData.adjustConfig.offsetS1 >= 1000) {
                sprintf(strBuffer, " %2d.%02d", offsetintVal2 / 100, offsetintVal2 % 100);
                DrawMediumNumber(180, 175, strBuffer, color);
			}
			else {
                sprintf(strBuffer, " %d.%02d", offsetintVal2 / 100, offsetintVal2 % 100);
                DrawMediumNumber(180, 175, strBuffer, color);
			}
        }
    }
}


// ???? - ????
void DrawAdjustGradient(uint32_t cur, uint32_t sen, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS1 / 100, (tempConfigData.adjustConfig.gradientS1 % 100));
        //DrawMediumNumber(250, 165, strBuffer, color);
        //DrawSmallNumber(210, 175, strBuffer, color);
        DrawMediumNumber(200, 175, strBuffer, color);
    } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS2 / 100, (tempConfigData.adjustConfig.gradientS2 % 100));
        //DrawMediumNumber(250, 165, strBuffer, color);
        //DrawSmallNumber(210, 175, strBuffer, color);
        DrawMediumNumber(200, 175, strBuffer, color);
    } else {

    }
}

/* ???? A: UNUSED - ??? ???
void DrawAdjustFactoryReset(uint32_t cur) {
}
*/

// ??????? temp
void DrawCalibAuto(uint32_t cur, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

        tempIntVal = compareSignValue;
        if (tempIntVal < 0)
            tempIntVal = tempIntVal * (-1);

        switch (cur) {
            case 0:
				if ((tempIntVal/100)>=10) {
	                if (compareSignValue < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 100, (tempIntVal % 100)/10);
	                else
	                    sprintf(strBuffer, " %d.%01d", compareSignValue / 100, (compareSignValue % 100)/10);
	                //DrawMediumNumber(260, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(182, 175, strBuffer, color);

					sprintf(strBuffer, "%01d", (compareSignValue % 100)/10);
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(182+18*4, 175, strBuffer, WHITE);
				}
				else {
	                if (compareSignValue < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 100, (tempIntVal % 100)/10);
	                else
	                    sprintf(strBuffer, " %d.%01d", compareSignValue / 100, (compareSignValue % 100)/10);
	                //DrawMediumNumber(260, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(200, 175, strBuffer, color);

					sprintf(strBuffer, "%01d", (compareSignValue % 100)/10);
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(200+18*3, 175, strBuffer, WHITE);
				}

                break;
            case 1:
				if ((tempIntVal/100)>=10) {
	                if (compareSignValue < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 100, (tempIntVal % 100)/10);
	                else
	                    sprintf(strBuffer, " %d.%01d", compareSignValue / 100, (compareSignValue % 100)/10);

	                //DrawMediumNumber(260, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(182, 175, strBuffer, WHITE);

					sprintf(strBuffer, "%01d", (compareSignValue % 100)/10);
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(182+18*4, 175, strBuffer, color);
				}
				else {
	                if (compareSignValue < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 100, (tempIntVal % 100)/10);
	                else
	                    sprintf(strBuffer, " %d.%01d", compareSignValue / 100, (compareSignValue % 100)/10);

	                //DrawMediumNumber(260, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(200, 175, strBuffer, WHITE);

					sprintf(strBuffer, "%01d", (compareSignValue % 100)/10);
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(200+18*3, 175, strBuffer, color);
				}
                break;
            case 2:
                break;
        }
    } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	// ??????
        switch (cur) {
            case 0:
                sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1Value / 1000, (tempConfigData.calibrationConfig.S1Value / 100) % 10);
                //DrawMediumNumber(270, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);

				sprintf(strBuffer, "%2d", tempConfigData.calibrationConfig.S1Value % 100);
                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, WHITE);
                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
                DrawMediumNumber(192+27*2, 175, strBuffer, WHITE);
                break;
            case 1:
                sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1Value / 1000, (tempConfigData.calibrationConfig.S1Value / 100) % 10);
                //DrawMediumNumber(270, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);

				sprintf(strBuffer, "%2d", tempConfigData.calibrationConfig.S1Value % 100);
                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, color);
                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
                DrawMediumNumber(192+27*2, 175, strBuffer, color);
                break;
            case 2:
                break;
        }
    } else {

    }
}


// ??????? span
void DrawCalibManual(uint32_t cur, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        switch (cur) {
            case 0:
				if ((tempConfigData.calibrationConfig.S1manualCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1manualCal / 100, (tempConfigData.calibrationConfig.S1manualCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(174, 175, strBuffer, color);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(174+18*5, 175, strBuffer, WHITE);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1manualCal / 100, (tempConfigData.calibrationConfig.S1manualCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(192, 175, strBuffer, color);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(192+18*4, 175, strBuffer, WHITE);
				}

                break;
            case 1:
				if ((tempConfigData.calibrationConfig.S1manualCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1manualCal / 100, (tempConfigData.calibrationConfig.S1manualCal / 10) % 10);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(174+18*5, 175, strBuffer, color);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1manualCal / 100, (tempConfigData.calibrationConfig.S1manualCal / 10) % 10);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(192, 175, strBuffer, WHITE);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(192+18*4, 175, strBuffer, color);
				}
                break;
            case 2:

                break;
        }
    }
    else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        switch (cur) {
            case 0:
				if ((tempConfigData.calibrationConfig.S2manualCal)>=10000) {
	                sprintf(strBuffer, " %d.%d ", tempConfigData.calibrationConfig.S2manualCal / 1000, (tempConfigData.calibrationConfig.S2manualCal / 100) % 10);
	                //DrawMediumNumber(255, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(174, 175, strBuffer, color);
	                sprintf(strBuffer, "%02d ", tempConfigData.calibrationConfig.S2manualCal % 100);
	                // sprintf(strBuffer, "%d", (tempConfigData.calibrationConfig.S1manualCal%10));  
	                //DrawMediumNumber(255 + (22 * 4), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
	                DrawMediumNumber(174+18*5, 175, strBuffer, WHITE);
				}
				else {
	                sprintf(strBuffer, " %d.%d ", tempConfigData.calibrationConfig.S2manualCal / 1000, (tempConfigData.calibrationConfig.S2manualCal / 100) % 10);
	                //DrawMediumNumber(255, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(192, 175, strBuffer, color);
	                sprintf(strBuffer, "%02d ", tempConfigData.calibrationConfig.S2manualCal % 100);
	                // sprintf(strBuffer, "%d", (tempConfigData.calibrationConfig.S1manualCal%10));  
	                //DrawMediumNumber(255 + (22 * 4), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
	                DrawMediumNumber(192+18*4, 175, strBuffer, WHITE);
				}

                break;
            case 1:
				if ((tempConfigData.calibrationConfig.S2manualCal)>=10000) {
	                sprintf(strBuffer, " %d.%d ", tempConfigData.calibrationConfig.S2manualCal / 1000, (tempConfigData.calibrationConfig.S2manualCal / 100) % 10);
	                //DrawMediumNumber(255, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);
	                sprintf(strBuffer, "%02d ", tempConfigData.calibrationConfig.S2manualCal % 100);
	                //DrawMediumNumber(255 + (22 * 4), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
	                DrawMediumNumber(174+18*5, 175, strBuffer, color);
				}
				else {
	                sprintf(strBuffer, " %d.%d ", tempConfigData.calibrationConfig.S2manualCal / 1000, (tempConfigData.calibrationConfig.S2manualCal / 100) % 10);
	                //DrawMediumNumber(255, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(192, 175, strBuffer, WHITE);
	                sprintf(strBuffer, "%02d ", tempConfigData.calibrationConfig.S2manualCal % 100);
	                //DrawMediumNumber(255 + (22 * 4), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
	                DrawMediumNumber(192+18*4, 175, strBuffer, color);
				}
                break;
            case 2:

                break;
        }
    }
}


// ????????
void DrawCalibZero(uint32_t cur, uint32_t color) {

    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        zerointVal = tempConfigData.calibrationConfig.S1zeroCal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }
        if (cur == 0) {
            if (tempConfigData.calibrationConfig.S1zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 100, (zerointVal / 10) % 10);
                //DrawMediumNumber(270, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);
				sprintf(strBuffer, "%d", zerointVal % 10);
                //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, WHITE);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
            }
            else {
				if ((tempConfigData.calibrationConfig.S1zeroCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(174, 175, strBuffer, color);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
	                DrawMediumNumber(174+23*4, 175, strBuffer, WHITE);
				}
				else {	
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(192, 175, strBuffer, color);

	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
	                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
				}
            }
        } else if (cur == 1) {
            if (tempConfigData.calibrationConfig.S1zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 100, (zerointVal / 10) % 10);
                //DrawMediumNumber(270, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);
                sprintf(strBuffer, "%d", zerointVal % 10);
                //DrawMediumNumber(270+(22*4), 120, strBuffer, color);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
                DrawMediumNumber(192+25*3, 175, strBuffer, color);
            }
            else {
				if ((tempConfigData.calibrationConfig.S1zeroCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270+(22*4), 120, strBuffer, color);				
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
	                DrawMediumNumber(174+23*4, 175, strBuffer, color);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(192, 175, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270+(22*4), 120, strBuffer, color);				
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
	                DrawMediumNumber(192+25*3, 175, strBuffer, color);
				}
            }

        }
    }        

	// ????? -----------------------------------------------------------------------//
    else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        zerointVal = tempConfigData.calibrationConfig.S2zeroCal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }
        if (cur == 0) {
            if (tempConfigData.calibrationConfig.S2zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 1000, (zerointVal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);
                sprintf(strBuffer, "%02d", zerointVal % 100);
                //DrawMediumNumber(270+(22*3), 100, strBuffer, WHITE);				
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
            }
            else {
                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal / 1000, (tempConfigData.calibrationConfig.S2zeroCal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);
                sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal % 100);
                //	DrawMediumNumber(270+(22*3), 100, strBuffer, WHITE);				
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
            }
            //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, WHITE);
        } else if (cur == 1) {
            if (tempConfigData.calibrationConfig.S2zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 1000, (zerointVal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);
                sprintf(strBuffer, "%02d", zerointVal % 100);
                //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, color);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
                DrawMediumNumber(192+25*3, 175, strBuffer, color);
            }
            else {
                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal / 1000, (tempConfigData.calibrationConfig.S2zeroCal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);
                sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal % 100);
                //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, color);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
                DrawMediumNumber(192+25*3, 175, strBuffer, color);
            }
        }
    }
}


// ???????
void DrawCalibPH4(uint32_t cur, uint32_t color) {

    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        zerointVal = tempConfigData.calibrationConfig.PH4_Cal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }


        if (tempConfigData.calibrationConfig.PH4_Cal < 0) {
            sprintf(strBuffer, "-%d.%02d", zerointVal / 100, zerointVal%100);
            DrawMediumNumber(192, 175, strBuffer, color);
        }
        else {
            sprintf(strBuffer, " %d.%02d", zerointVal / 100, zerointVal%100);
            DrawMediumNumber(192, 175, strBuffer, color);
        }
    }        

}


void DrawCalibPH7(uint32_t cur, uint32_t color) {

    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        zerointVal = tempConfigData.calibrationConfig.PH7_Cal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }


        if (tempConfigData.calibrationConfig.PH7_Cal < 0) {
            sprintf(strBuffer, "-%d.%02d", zerointVal / 100, zerointVal%100);
            DrawMediumNumber(192, 175, strBuffer, color);
        }
        else {
            sprintf(strBuffer, " %d.%02d", zerointVal / 100, zerointVal%100);
            DrawMediumNumber(192, 175, strBuffer, color);
        }
    }        

}


void DrawCalib_EC(uint32_t cur, uint32_t color) {

	// ?????????? -----------------------------------------------------------------------//
    if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        zerointVal = tempConfigData.calibrationConfig.EC_Cal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }
        if (tempConfigData.calibrationConfig.EC_Cal < 0) {
            sprintf(strBuffer, "-%d.%01d", zerointVal/10, zerointVal%10);
            DrawMediumNumber(200, 175, strBuffer, color);
        }
        else {
            sprintf(strBuffer, " %d.%01d", zerointVal/10, zerointVal%10);
            DrawMediumNumber(200, 175, strBuffer, color);
        }
    }
}


// ???????
void DrawCalibBuff(uint32_t cur, uint32_t color) {

    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        zerointVal = tempConfigData.calibrationConfig.S1zeroCal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }
        if (cur == 0) {
            if (tempConfigData.calibrationConfig.S1zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 100, (zerointVal / 10) % 10);
                //DrawMediumNumber(270, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);
				sprintf(strBuffer, "%d", zerointVal % 10);
                //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, WHITE);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
            }
            else {
				if ((tempConfigData.calibrationConfig.S1zeroCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(174, 175, strBuffer, color);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
	                DrawMediumNumber(174+23*4, 175, strBuffer, WHITE);
				}
				else {	
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(192, 175, strBuffer, color);

	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
	                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
				}
            }
        } else if (cur == 1) {
            if (tempConfigData.calibrationConfig.S1zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 100, (zerointVal / 10) % 10);
                //DrawMediumNumber(270, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);
                sprintf(strBuffer, "%d", zerointVal % 10);
                //DrawMediumNumber(270+(22*4), 120, strBuffer, color);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
                DrawMediumNumber(192+25*3, 175, strBuffer, color);
            }
            else {
				if ((tempConfigData.calibrationConfig.S1zeroCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270+(22*4), 120, strBuffer, color);				
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
	                DrawMediumNumber(174+23*4, 175, strBuffer, color);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(192, 175, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
	                //DrawMediumNumber(270+(22*4), 120, strBuffer, color);				
	                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
	                DrawMediumNumber(192+25*3, 175, strBuffer, color);
				}
            }

        }
    }        

	// ????? -----------------------------------------------------------------------//
    else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        zerointVal = tempConfigData.calibrationConfig.S2zeroCal;

        if (zerointVal < 0) {
            zerointVal = zerointVal * (-1);
        }
        if (cur == 0) {
            if (tempConfigData.calibrationConfig.S2zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 1000, (zerointVal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);
                sprintf(strBuffer, "%02d", zerointVal % 100);
                //DrawMediumNumber(270+(22*3), 100, strBuffer, WHITE);				
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
            }
            else {
                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal / 1000, (tempConfigData.calibrationConfig.S2zeroCal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, color);
                //DrawSmallNumber(210, 175, strBuffer, color);
                DrawMediumNumber(192, 175, strBuffer, color);
                sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal % 100);
                //	DrawMediumNumber(270+(22*3), 100, strBuffer, WHITE);				
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, WHITE);
                DrawMediumNumber(192+25*3, 175, strBuffer, WHITE);
            }
            //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, WHITE);
        } else if (cur == 1) {
            if (tempConfigData.calibrationConfig.S2zeroCal < 0) {
                sprintf(strBuffer, "-%d.%d", zerointVal / 1000, (zerointVal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);
                sprintf(strBuffer, "%02d", zerointVal % 100);
                //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, color);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
                DrawMediumNumber(192+25*3, 175, strBuffer, color);
            }
            else {
                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal / 1000, (tempConfigData.calibrationConfig.S2zeroCal / 100) % 10);
                //DrawMediumNumber(248, 120, strBuffer, WHITE);
                //DrawSmallNumber(210, 175, strBuffer, WHITE);
                DrawMediumNumber(192, 175, strBuffer, WHITE);
                sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal % 100);
                //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, color);
                //DrawSmallNumber(210 + (22 * 4), 175, strBuffer, WHITE);
                //DrawSmallNumber(210+12*4, 175, strBuffer, color);
                DrawMediumNumber(192+25*3, 175, strBuffer, color);
            }
        }
    }
}

//=====================

void DrawCalibSpan(uint32_t cur, uint32_t color) {
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
        switch (cur) {
            case 0:
				if ((tempConfigData.calibrationConfig.PH_Span_Cal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.PH_Span_Cal / 100, (tempConfigData.calibrationConfig.PH_Span_Cal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(174, 175, strBuffer, color);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.PH_Span_Cal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(174+18*5, 175, strBuffer, WHITE);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.PH_Span_Cal / 100, (tempConfigData.calibrationConfig.PH_Span_Cal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(192, 175, strBuffer, color);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.PH_Span_Cal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(192+18*4, 175, strBuffer, WHITE);
				}

                break;
            case 1:
				if ((tempConfigData.calibrationConfig.PH_Span_Cal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.PH_Span_Cal / 100, (tempConfigData.calibrationConfig.PH_Span_Cal / 10) % 10);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.PH_Span_Cal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(174+18*5, 175, strBuffer, color);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.PH_Span_Cal / 100, (tempConfigData.calibrationConfig.PH_Span_Cal / 10) % 10);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(192, 175, strBuffer, WHITE);
	                //DrawMediumNumber(270, 120, strBuffer, WHITE);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.PH_Span_Cal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(192+18*4, 175, strBuffer, color);
				}
                break;
            case 2:

                break;
        }
    }
    else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
        switch (cur) {
            case 0:

				if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=10000) {
	                sprintf(strBuffer, " %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, color);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, WHITE);
				}
				else if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=1000) {
	                sprintf(strBuffer, "  %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, color);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, WHITE);
				}
				else if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=100) {
	                sprintf(strBuffer, "   %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, color);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, WHITE);
				}
				else {
	                sprintf(strBuffer, "     %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, color);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, WHITE);
				}

                break;
            case 1:

				if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=10000) {
	                sprintf(strBuffer, " %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, color);
				}
				else if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=1000) {
	                sprintf(strBuffer, "  %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, color);
				}
				else if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=100) {
	                sprintf(strBuffer, "   %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, color);
				}
				else {
	                sprintf(strBuffer, "     %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, WHITE);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, color);
				}

                break;
            case 2:

                break;
        }
    }
}

void DrawCalibTemp(uint32_t cur, uint32_t color) {

	int32_t TEMP_Span_Cal=0;

   	if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
		TEMP_Span_Cal = tempConfigData.calibrationConfig.TEMP_Span_Cal1;
	}
	else {
		TEMP_Span_Cal = tempConfigData.calibrationConfig.TEMP_Span_Cal2;
	}

        tempIntVal = TEMP_Span_Cal;
        if (tempIntVal < 0)
            tempIntVal = tempIntVal * (-1);

        switch (cur) {
            case 0:
				if ((tempIntVal)>=100) {
	                if (TEMP_Span_Cal < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 10, (tempIntVal % 10));
	                else
	                    sprintf(strBuffer, " %d.%01d", TEMP_Span_Cal / 10, (TEMP_Span_Cal % 10));
	                //DrawMediumNumber(260, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(182, 175, strBuffer, color);

					sprintf(strBuffer, "%01d", (TEMP_Span_Cal % 10));
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(182+18*4, 175, strBuffer, WHITE);
				}
				else {
	                if (TEMP_Span_Cal < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 10, (tempIntVal % 10));
	                else
	                    sprintf(strBuffer, " %d.%01d", TEMP_Span_Cal / 10, (TEMP_Span_Cal % 10));
	                //DrawMediumNumber(260, 120, strBuffer, color);
	                //DrawSmallNumber(210, 175, strBuffer, color);
	                DrawMediumNumber(200, 175, strBuffer, color);

					sprintf(strBuffer, "%01d", (TEMP_Span_Cal % 10));
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, WHITE);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, WHITE);
	                DrawMediumNumber(200+18*3, 175, strBuffer, WHITE);
				}

                break;
            case 1:
				if ((tempIntVal)>=100) {
	                if (TEMP_Span_Cal < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 10, (tempIntVal % 10));
	                else
	                    sprintf(strBuffer, " %d.%01d", TEMP_Span_Cal / 10, (TEMP_Span_Cal % 10));

	                //DrawMediumNumber(260, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(182, 175, strBuffer, WHITE);

					sprintf(strBuffer, "%01d", (TEMP_Span_Cal % 10));
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(182+18*4, 175, strBuffer, color);
				}
				else {
	                if (TEMP_Span_Cal < 0)
	                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 10, (tempIntVal % 10));
	                else
	                    sprintf(strBuffer, " %d.%01d", TEMP_Span_Cal / 10, (TEMP_Span_Cal % 10));

	                //DrawMediumNumber(260, 120, strBuffer, WHITE);
	                //DrawSmallNumber(210, 175, strBuffer, WHITE);
	                DrawMediumNumber(200, 175, strBuffer, WHITE);

					sprintf(strBuffer, "%01d", (TEMP_Span_Cal % 10));
	                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, color);
	                //DrawSmallNumber(210+12*3, 175, strBuffer, color);
	                DrawMediumNumber(200+18*3, 175, strBuffer, color);
				}
                break;
            case 2:
                break;
        }
}


//==========================


// analog

void DrawOutputConfig(uint32_t cur, uint32_t color) {
    switch (cur) {

        case 4:
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal4mA / 100);
	            //		DrawMediumNumber(230, 170, strBuffer, color);
	            DrawSmallNumber(320, 152, strBuffer, color);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal4mA%100);
	            //		DrawMediumNumber(230+(22*3), 170, strBuffer, WHITE);
	            DrawSmallNumber(320+12*3, 152, strBuffer, WHITE);
			}
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal4mA2 / 100);
	            //		DrawMediumNumber(230, 170, strBuffer, color);
	            DrawSmallNumber(320, 152, strBuffer, color);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal4mA2%100);
	            //		DrawMediumNumber(230+(22*3), 170, strBuffer, WHITE);
	            DrawSmallNumber(320+12*3, 152, strBuffer, WHITE);
			}

            break;
        case 5:
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal4mA / 100);
	            //		DrawMediumNumber(230, 170, strBuffer, WHITE);
	            DrawSmallNumber(320, 152, strBuffer, WHITE);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal4mA%100);
	            //		DrawMediumNumber(230+(22*3), 170, strBuffer, color);
	            DrawSmallNumber(320+12*3, 152, strBuffer, color);
			}
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal4mA2 / 100);
	            //		DrawMediumNumber(230, 170, strBuffer, WHITE);
	            DrawSmallNumber(320, 152, strBuffer, WHITE);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal4mA2%100);
	            //		DrawMediumNumber(230+(22*3), 170, strBuffer, color);
	            DrawSmallNumber(320+12*3, 152, strBuffer, color);
			}
            break;
        case 6:
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal20mA / 100);
	            //		DrawMediumNumber(230, 220, strBuffer, color);
	            DrawSmallNumber(320, 217, strBuffer, color);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal20mA%100);
	            //		DrawMediumNumber(230+(22*3), 220, strBuffer, WHITE);
	            DrawSmallNumber(320+12*3, 217, strBuffer, WHITE);
			}
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal20mA2 / 100);
	            //		DrawMediumNumber(230, 220, strBuffer, color);
	            DrawSmallNumber(320, 217, strBuffer, color);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal20mA2%100);
	            //		DrawMediumNumber(230+(22*3), 220, strBuffer, WHITE);
	            DrawSmallNumber(320+12*3, 217, strBuffer, WHITE);
			}
            break;
        case 7:
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal20mA / 100);
	            //		DrawMediumNumber(230, 220, strBuffer, WHITE);
	            DrawSmallNumber(320, 217, strBuffer, WHITE);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal20mA%100);
	            //		DrawMediumNumber(230+(22*3), 220, strBuffer, color);
	            DrawSmallNumber(320+12*3, 217, strBuffer, color);
			}
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal20mA2 / 100);
	            //		DrawMediumNumber(230, 220, strBuffer, WHITE);
	            DrawSmallNumber(320, 217, strBuffer, WHITE);
	            sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal20mA2%100);
	            //		DrawMediumNumber(230+(22*3), 220, strBuffer, color);
	            DrawSmallNumber(320+12*3, 217, strBuffer, color);
			}
            break;

    }
}

// ???????  Value Scale
void DrawAlarmConfig(uint32_t cur, uint32_t color) {
    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
        switch (cur) {

            case 0:
                sprintf(strBuffer, " %d.%d", tempConfigData.alarmConfig.highLimit / 100, (tempConfigData.alarmConfig.highLimit % 100) / 10);
                //DrawMediumNumber(200, 90, strBuffer, color);
				if (tempConfigData.alarmConfig.highLimit>=1000) 	DrawSmallNumber(63-12,180, strBuffer, color);
                else 											DrawSmallNumber(63,   180, strBuffer, color);

                sprintf(strBuffer, "%01d", tempConfigData.alarmConfig.highLimit % 10);
                //DrawMediumNumber(200 + (22 * 3), 90, strBuffer, WHITE);
                DrawSmallNumber(76+ (12*3), 180, strBuffer, WHITE);
                break;
            case 1:
                sprintf(strBuffer, " %d.%d", tempConfigData.alarmConfig.highLimit / 100, (tempConfigData.alarmConfig.highLimit % 100) / 10);
                //DrawMediumNumber(200, 90, strBuffer, WHITE);
				if (tempConfigData.alarmConfig.highLimit>=10000) 	DrawSmallNumber(63-12,180, strBuffer, WHITE);
                else 											DrawSmallNumber(63,   180, strBuffer, WHITE);

                sprintf(strBuffer, "%01d", tempConfigData.alarmConfig.highLimit % 10);
                //DrawMediumNumber(200 + (22 * 3), 90, strBuffer, color);
                DrawSmallNumber(76 + (12*3), 180, strBuffer, color);
                break;
            case 2:
                sprintf(strBuffer, " %d.%d", tempConfigData.alarmConfig.lowLimit / 100, (tempConfigData.alarmConfig.lowLimit % 100) / 10);
                //DrawMediumNumber(200, 140, strBuffer, color);
				if (tempConfigData.alarmConfig.lowLimit>=1000) 	DrawSmallNumber(205-12,180, strBuffer, color);
                else 												DrawSmallNumber(205,   180, strBuffer, color);

                sprintf(strBuffer, "%01d", tempConfigData.alarmConfig.lowLimit % 10);
                //DrawMediumNumber(200 + (22 * 3), 140, strBuffer, WHITE);
                DrawSmallNumber(217 + (12*3), 180, strBuffer, WHITE);
                break;
            case 3:
                sprintf(strBuffer, " %d.%d", tempConfigData.alarmConfig.lowLimit / 100, (tempConfigData.alarmConfig.lowLimit % 100) / 10);
                //DrawMediumNumber(200, 140, strBuffer, WHITE);
				if (tempConfigData.alarmConfig.lowLimit>=1000) 	DrawSmallNumber(205-12,180, strBuffer, WHITE);
                else 												DrawSmallNumber(205,   180, strBuffer, WHITE);

                sprintf(strBuffer, "%01d", tempConfigData.alarmConfig.lowLimit % 10);
                //DrawMediumNumber(200 + (22 * 3), 140, strBuffer, color);
                DrawSmallNumber(217 + (12*3), 180, strBuffer, color);
                break;
            case 4:
                sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm);
                //DrawMediumNumber(200 + 22, 190, strBuffer, color);
				DrawSmallNumber(360,180, strBuffer, color);
                break;
        }
    }
    else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
        if (sensor_manager_get_display_field(1) == WATER_FIELD_EC) {
            switch (cur) {
                case 0:
                    sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.highLimit2 / 100);
                    DrawSmallNumber(41, 180, strBuffer, color);
                    sprintf(strBuffer, "%01d.%01d", (tempConfigData.alarmConfig.highLimit2 % 100)/10, tempConfigData.alarmConfig.highLimit2 % 10);
                    DrawSmallNumber(42+ (12*3), 180, strBuffer, WHITE);
                    break;
                case 1:
                    sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.highLimit2 / 100);
                    DrawSmallNumber(41, 180, strBuffer, WHITE);
                    sprintf(strBuffer, "%01d.%01d", (tempConfigData.alarmConfig.highLimit2 % 100)/10, tempConfigData.alarmConfig.highLimit2 % 10);
                    DrawSmallNumber(42+ (12*3), 180, strBuffer, color);
                    break;
                case 2:
                    sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.lowLimit2 / 100);
                    DrawSmallNumber(183, 180, strBuffer, color);
                    sprintf(strBuffer, "%01d.%01d", (tempConfigData.alarmConfig.lowLimit2 % 100)/10, tempConfigData.alarmConfig.lowLimit2 % 10);
                    DrawSmallNumber(184 + (12*3), 180, strBuffer, WHITE);
                    break;
                case 3:
                    sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.lowLimit2 / 100);
                    DrawSmallNumber(183, 180, strBuffer, WHITE);
                    sprintf(strBuffer, "%01d.%01d", (tempConfigData.alarmConfig.lowLimit2 % 100)/10, tempConfigData.alarmConfig.lowLimit2 % 10);
                    DrawSmallNumber(184 + (12*3), 180, strBuffer, color);
                    break;
                case 4:
                    sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm2);
                    DrawSmallNumber(350,180, strBuffer, color);
                    break;
            }
        } else {
            switch (cur) {
                case 0:
                    sprintf(strBuffer, "%1d.%1d", tempConfigData.alarmConfig.highLimit2 / 1000, (tempConfigData.alarmConfig.highLimit2 / 100)%10);
                    DrawSmallNumber(61, 180, strBuffer, color);
                    sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit2 % 100);
                    DrawSmallNumber(62+ (12*3), 180, strBuffer, WHITE);
                    break;
                case 1:
                    sprintf(strBuffer, "%1d.%1d", tempConfigData.alarmConfig.highLimit2 / 1000, (tempConfigData.alarmConfig.highLimit2 / 100)%10);
                    DrawSmallNumber(61, 180, strBuffer, WHITE);
                    sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit2 % 100);
                    DrawSmallNumber(62+ (12*3), 180, strBuffer, color);
                    break;
                case 2:
                    sprintf(strBuffer, "%1d.%1d", tempConfigData.alarmConfig.lowLimit2 / 1000, (tempConfigData.alarmConfig.lowLimit2 / 100)%10);
                    DrawSmallNumber(203, 180, strBuffer, color);
                    sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit2 % 100);
                    DrawSmallNumber(204 + (12*3), 180, strBuffer, WHITE);
                    break;
                case 3:
                    sprintf(strBuffer, "%1d.%1d", tempConfigData.alarmConfig.lowLimit2 / 1000, (tempConfigData.alarmConfig.lowLimit2 / 100)%10);
                    DrawSmallNumber(203, 180, strBuffer, WHITE);
                    sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit2 % 100);
                    DrawSmallNumber(204 + (12*3), 180, strBuffer, color);
                    break;
                case 4:
                    sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm2);
                    DrawSmallNumber(350,180, strBuffer, color);
                    break;
            }
        }
    }
}

void DrawTimeConfig(uint32_t cur, uint32_t color) {


    switch (cur) {
        case 0:
            Draw_Back_Oval6(56, 144);
            sprintf(strBuffer, "%4d", tempConfigTime.tm_year);
            //DrawMediumNumber(49, 100, strBuffer, color);
            DrawSmallNumber(78, 152, strBuffer, color);
            break;
        case 1:
            Draw_Back_Oval6(197, 144);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_mon + 1);
            //DrawMediumNumber(219, 100, strBuffer, color);
            DrawSmallNumber(230, 152, strBuffer, color);
            break;
        case 2:
            Draw_Back_Oval6(340, 144);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_mday);
            //DrawMediumNumber(343, 100, strBuffer, color);
            DrawSmallNumber(373, 152, strBuffer, color);
            break;
        case 3:
            Draw_Back_Oval6(56, 207);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_hour);
            //DrawMediumNumber(93, 200, strBuffer, color);
            DrawSmallNumber(90, 216, strBuffer, color);
            break;
        case 4:
            Draw_Back_Oval6(197, 207);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_min);
            //DrawMediumNumber(219, 200, strBuffer, color);
            DrawSmallNumber(230, 216, strBuffer, color);
            break;
        case 5:
            Draw_Back_Oval6(340, 207);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_sec);
            //DrawMediumNumber(343, 200, strBuffer, color);
            DrawSmallNumber(373, 216, strBuffer, color);
            break;
    }
}

void DrawCommConfig(uint32_t cur, uint32_t color) {
    switch (cur) {
        case 0:
			if (tempConfigData.modbusConfig.mode!=2) {

	            sprintf(strBuffer, "       ");
	            //DrawMediumNumber(230 + 66, 170, strBuffer, color);
	            DrawSmallNumber(100, 152, strBuffer, color);

	            //DrawMediumNumber(230 + 66, 70, (char*) comModeText[tempConfigData.modbusConfig.mode], color);
	            DrawSmallNumber(115, 152, (char*) comModeText[tempConfigData.modbusConfig.mode], color);
			}
			else {
				display_Ethernet(100, 152);
			}
            break;
        case 1:
            //DrawMediumNumber(230, 120, (char*) comBaudrateText[tempConfigData.modbusConfig.baudrate], color);
            DrawSmallNumber(80, 217, (char*) comBaudrateText[tempConfigData.modbusConfig.baudrate], color);
            break;
        case 2:
            sprintf(strBuffer, "%3d", tempConfigData.modbusConfig.modbusSlaveAddr + 1);
            //DrawMediumNumber(230 + 66, 170, strBuffer, color);
            DrawSmallNumber(315, 152, strBuffer, color);
            break;
        case 3:
            sprintf(strBuffer, "%d", tempConfigData.modbusConfig.databit + 8);
            //DrawMediumNumber(230 + 110, 220, strBuffer, color);
            DrawSmallNumber(340, 217, strBuffer, color);
            break;
    }
}


void DrawCommEthernetConfig(uint32_t cur, uint32_t color) {
    switch (cur) {
        case 0:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR0);
            DrawSmallNumber(50, 152, strBuffer, color);
            break;
        case 1:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR1);
            DrawSmallNumber(90, 152, strBuffer, color);
            break;
        case 2:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR2);
            DrawSmallNumber(130, 152, strBuffer, color);
            break;
        case 3:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR3);
            DrawSmallNumber(170, 152, strBuffer, color);
            break;


        case 4:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR0);
            DrawSmallNumber(50, 217, strBuffer, color);
            break;
        case 5:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR1);
            DrawSmallNumber(90, 217, strBuffer, color);
            break;
        case 6:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR2);
            DrawSmallNumber(130, 217, strBuffer, color);
            break;
        case 7:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR3);
            DrawSmallNumber(170, 217, strBuffer, color);
            break;

        case 8:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR0);
            DrawSmallNumber(270, 152, strBuffer, color);
            break;
        case 9:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR1);
            DrawSmallNumber(310, 152, strBuffer, color);
            break;
        case 10:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR2);
            DrawSmallNumber(350, 152, strBuffer, color);
            break;
        case 11:
            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR3);
            DrawSmallNumber(390, 152, strBuffer, color);
            break;


        case 12:
            sprintf(strBuffer, "%5d", tempConfigData.EthernetConfig.ETHERNET_PORT);
            DrawSmallNumber(330, 217, strBuffer, color);
			break;

    }
}

/* ���� A: UNUSED - �ּ� ó��
void DrawBottomCurrent(uint32_t current) {
    sprintf(strBuffer, "4-20"); //"%2d.%01d", current/10, current %10);
    DrawSmallNumber(340, 233, strBuffer, WHITE);
}
*/

