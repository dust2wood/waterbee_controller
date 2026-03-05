#include "ReDisplay.h"
#include "Sensor_Manager.h"

const char* comModeText[3] = {"232", "485", "586"};
const char* comDatabitText[2] = {"8", "9"};
const char* comBaudrateText[4] = {"  9600", " 19200", " 38400", "115200"};
int32_t tempIntVal = 0;
int32_t zerointVal = 0;
int32_t offsetintVal = 0;


// SENSOR1 LARGE NUMBER
#define L_NUMBER1_X	90	//200
#define L_NUMBER1_Y	90

// SENSOR2 LARGE NUMBER
#define L_NUMBER2_X	95	//140
#define L_NUMBER2_Y	110


// TITLE TEXT
#define TITLE_X  200	// 153
#define TITLE_X2 220	// 172

#define TITLE_Y 5


//
#define MGL_X0  50
#define MGL_X1  330	//360
#define MGL_X2  340	//370

#define MGL_Y0  50
#define MGL_Y1  90
#define MGL_Y2  140
#define MGL_Y3  190


// ùù ù?ù ùùùùùùùù ùùùùùùù ùùùùùù 
void RedrawValue(void) {
    switch (state) {
        case STATE_MAIN:
        case STATE_MENU:
        case STATE_CONFIG:
        case STATE_CALIB:
        case STATE_CONFIG_RELAY:
        case STATE_CONFIG_RELAY_RELAY1:
        case STATE_CONFIG_RELAY_RELAY3:
        case STATE_CONFIG_ADJUST:
            RedrawMainValue();
            break;
        case STATE_CONFIG_RELAY_RELAY2:
            break;


        case STATE_CONFIG_TIME:
            break;
        case STATE_TREND:

            break;
        case STATE_ALARM:
            break;
        case STATE_DIAGNOSTIC:
            break;

        case STATE_CONFIG_OUTPUT:
            break;
        case STATE_CONFIG_COMM:
            break;
        case STATE_CALIB_ZERO:
            break;
        case STATE_CALIB_TEMP:
            break;
        case STATE_CALIB_MANUAL:
            break;
        case STATE_CALIB_S2_CYCLE:
            break;
        case STATE_CALIB_LOG:
            DrawCalibLog(cursor);
            break;

        case STATE_CONFIG_ADJUST_GRADIENT:
            break;
        case STATE_CONFIG_ADJUST_OFFSET:
            break;
        case STATE_CONFIG_ADJUST_FILTER:
            break;
        case STATE_CONFIG_ADJUST_FACTORYRESET:
            break;

        case STATE_CONFIG_RELAY_RELAY1_AUTO_WASH:
            break;
        case STATE_CONFIG_RELAY_RELAY1_MAN_WASH:
            break;
        case STATE_CONFIG_RELAY_RELAY2_AUTO_ALARM:
            break;
        case STATE_CONFIG_RELAY_RELAY2_ALARM_OFF:
            break;
        case STATE_CONFIG_RELAY_RELAY3_SET:
            break;
        case STATE_CONFIG_RELAY_RELAY3_MAN:
            break;
    }
}


void RedrawTitle_Main(void) {
	 display_home();
}

void RedrawTitle(void) {
    // TITLE
//    if (currentData.Device_Selector_Mode == SENSOR_1_MODE)
//        DrawTextsize96(TITLE_X, TITLE_Y, TEXT96_CI, DRAW_IMAGE_ENABLE);
//    else DrawTextsize96(TITLE_X, TITLE_Y, TEXT96_CI3, DRAW_IMAGE_ENABLE);


#ifndef  SENSOR_PH_EC
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE)
		 display_CL1();
    else display_CL2();
#else
    if (currentData.Device_Selector_Mode == SENSOR_1_MODE)
		 display_O2();
    else display_elec();

#endif


}

void RedrawViewArea(void) {

	int32_t TEMP_Span_Cal=0;


    switch (state) {
        case STATE_MAIN:
            RedrawMainView();
            break;
        case STATE_MENU:
            break;
        case STATE_CONFIG:
            break;
        case STATE_CALIB:
            break;
        case STATE_TREND:

			display_back_graph();
//			display_set9_time_range(1);
//			display_set9_nongdo_range(0);


            break;


		case STATE_ALARM:

            //ClearViewArea();
            //DrawTextsize120(MGL_X0, MGL_Y1, TEXT120_LIMIT_HIGH, DRAW_IMAGE_ENABLE);
            //DrawTextsize120(MGL_X0, MGL_Y2, TEXT120_LIMIT_LOW, DRAW_IMAGE_ENABLE);
            //DrawTextsize120(MGL_X0, MGL_Y3, TEXT120_ZERO_ALARM, DRAW_IMAGE_ENABLE);

            ClearViewArea2();
			display_value_scale();
			display_value_max();
			display_value_min();
			display_zero_alarm();


#ifndef SENSOR_PH_EC
            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {

	            Draw_Back_Oval3_mgl(35, 169);
	            Draw_Back_Oval3_mgl(177, 169);
	            Draw_Back_Oval3_min(319, 169);

                sprintf(strBuffer, " %d.%d", tempConfigData.alarmConfig.highLimit / 100, (tempConfigData.alarmConfig.highLimit % 100) / 10);
                //DrawMediumNumber(L_NUMBER1_X, L_NUMBER1_Y, strBuffer, YELLOW);
				if (tempConfigData.alarmConfig.highLimit>=1000) 	DrawSmallNumber(53-12,180, strBuffer, YELLOW);
                else 												DrawSmallNumber(53,   180, strBuffer, YELLOW);

                //DrawLineRectangle(180, 83, 330, 123, BROWN);

                sprintf(strBuffer, "%01d", tempConfigData.alarmConfig.highLimit % 10);
                //DrawMediumNumber(L_NUMBER1_X + (22 * 3), L_NUMBER1_Y, strBuffer, BROWN);
                DrawSmallNumber(66+(12*3),180, strBuffer, BROWN);

                sprintf(strBuffer, " %d.%02d", tempConfigData.alarmConfig.lowLimit / 100, tempConfigData.alarmConfig.lowLimit % 100);
                //DrawMediumNumber(L_NUMBER1_X, L_NUMBER4_Y, strBuffer, BROWN);
				if (tempConfigData.alarmConfig.lowLimit>=1000) 		DrawSmallNumber(195-12,180, strBuffer, BROWN);
                else 												DrawSmallNumber(195,   180, strBuffer, BROWN);

	            sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm);
	            //DrawMediumNumber(L_NUMBER1_X + 22, MGL_Y3, strBuffer, BROWN);
	            DrawSmallNumber(350, 180, strBuffer, BROWN);

            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
	            Draw_Back_Oval3_ntu(35, 169);
	            Draw_Back_Oval3_ntu(185, 169);
	            Draw_Back_Oval3_min(319, 169);


                sprintf(strBuffer, "%1d.%1d", tempConfigData.alarmConfig.highLimit2 / 1000, (tempConfigData.alarmConfig.highLimit2 / 100)%10);
                DrawSmallNumber(61, 180, strBuffer, YELLOW);

                sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit2 % 100 );
                DrawSmallNumber(62+ (12*3), 180, strBuffer, BROWN);

                sprintf(strBuffer, "%1d.%1d", tempConfigData.alarmConfig.lowLimit2 / 1000, (tempConfigData.alarmConfig.lowLimit2 / 100)%10);
                DrawSmallNumber(203, 180, strBuffer, BROWN);

                sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit2 % 100 );
                DrawSmallNumber(204 + (12*3), 180, strBuffer, BROWN);

	            sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm2);
	            //DrawMediumNumber(L_NUMBER1_X + 22, MGL_Y3, strBuffer, BROWN);
	            DrawSmallNumber(350, 180, strBuffer, BROWN);
            }

#else
            if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {

	            Draw_Back_Oval3(35, 169);
	            Draw_Back_Oval3(177, 169);
	            Draw_Back_Oval3_min(319, 169);

                sprintf(strBuffer, " %d.%d", tempConfigData.alarmConfig.highLimit / 100, (tempConfigData.alarmConfig.highLimit % 100) / 10);
                //DrawMediumNumber(L_NUMBER1_X, L_NUMBER1_Y, strBuffer, YELLOW);
				if (tempConfigData.alarmConfig.highLimit>=1000) 	DrawSmallNumber(63-12,180, strBuffer, YELLOW);
                else 												DrawSmallNumber(63,   180, strBuffer, YELLOW);

                //DrawLineRectangle(180, 83, 330, 123, BROWN);

                sprintf(strBuffer, "%01d", tempConfigData.alarmConfig.highLimit % 10);
                //DrawMediumNumber(L_NUMBER1_X + (22 * 3), L_NUMBER1_Y, strBuffer, BROWN);
                DrawSmallNumber(76+(12*3),180, strBuffer, BROWN);

                sprintf(strBuffer, " %d.%02d", tempConfigData.alarmConfig.lowLimit / 100, tempConfigData.alarmConfig.lowLimit % 100);
                //DrawMediumNumber(L_NUMBER1_X, L_NUMBER4_Y, strBuffer, BROWN);
				if (tempConfigData.alarmConfig.lowLimit>=1000) 		DrawSmallNumber(205-12,180, strBuffer, BROWN);
                else 												DrawSmallNumber(205,   180, strBuffer, BROWN);

	            sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm);
	            //DrawMediumNumber(L_NUMBER1_X + 22, MGL_Y3, strBuffer, BROWN);
	            DrawSmallNumber(350, 180, strBuffer, BROWN);

            } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
	            Draw_Back_Oval3_uscm(35, 169);
	            Draw_Back_Oval3_uscm(177, 169);
	            Draw_Back_Oval3_min (319, 169);

                sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.highLimit2 / 100);
                DrawSmallNumber(41, 180, strBuffer, YELLOW);

                sprintf(strBuffer, "%01d.%01d", (tempConfigData.alarmConfig.highLimit2 % 100)/10, tempConfigData.alarmConfig.highLimit2 % 10);
                DrawSmallNumber(42+ (12*3), 180, strBuffer, BROWN);					 

                sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.lowLimit2 / 100);
                DrawSmallNumber(183, 180, strBuffer, BROWN);

                sprintf(strBuffer, "%01d.%01d", (tempConfigData.alarmConfig.lowLimit2 % 100)/10, tempConfigData.alarmConfig.lowLimit2 % 10);
                DrawSmallNumber(184 + (12*3), 180, strBuffer, BROWN);

	            sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm2);
	            //DrawMediumNumber(L_NUMBER1_X + 22, MGL_Y3, strBuffer, BROWN);
	            DrawSmallNumber(350, 180, strBuffer, BROWN);
            }
#endif

            break;


		// ùùùù 
        case STATE_DIAGNOSTIC:
            //ClearViewArea();
            //DrawBack2();

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                //	DrawTextsize96(30, 70, TEXT96_SENSOR1_WHITE, DRAW_IMAGE_ENABLE);
                //DrawTextsize96(210, 5, TEXT96_CI, DRAW_IMAGE_ENABLE);
//				display_CL1();

                //DrawTextsize96(210,65, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
                display_diag();


                //	DrawTextsize96(180, 205, 0, DRAW_IMAGE_ENABLE); 
                sprintf(strBuffer, "%1d.%02d ", currentData.S1PPM / 100, currentData.S1PPM % 100);
                DrawMediumNumber(103, 155, strBuffer, BROWN);

                //DrawTextETC(190, 157, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
#ifndef  SENSOR_PH_EC
				display_mgl_2();
#else 
				display_2ch_small_unit_ph();
#endif
                sprintf(strBuffer, "%1d.%02d ", currentData.S1mV / 100, currentData.S1mV % 100);
                DrawMediumNumber(250, 155, strBuffer, BROWN);
                //DrawTextETC(260, 160, TEXT_ETC_MA, DRAW_IMAGE_ENABLE);
                //DrawTextETC(350, 157, TEXT_ETC_MV, DRAW_IMAGE_ENABLE);
				display_mV_1();

	            // ùùùù ùùùù ù?ù, ù?ù
	            if (Sensor_State1 == SENSOR_OK) display_set10_sensor(0); // ùùùù 
	            else if (Sensor_State1 == SENSOR_ERROR1) display_set10_sensor(1); // ù?ù, ùùùù ùùùù 
	            else if (Sensor_State1 == SENSOR_ERROR2) display_set10_sensor(2); // ù?ù, ùùùù ùùùù
	            else display_set10_sensor(3); // ùùùù ùùùù ùùùù

            }
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                //DrawTextsize96(210, 5, TEXT96_CI2, DRAW_IMAGE_ENABLE);
//				display_CL2();
                //DrawTextsize96(210, 65, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
                display_diag();


#ifndef  SENSOR_PH_EC
		        if ((currentData.S2PPM) >= 99999)
		            sprintf(strBuffer, "99.999");	
		        else
		            sprintf(strBuffer, "%2d.%03d ", currentData.S2PPM / 1000, currentData.S2PPM % 1000);

                //DrawMediumNumber(95, 155, strBuffer, BROWN);
                //DrawTextETC(190, 157, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
                DrawMediumNumber(55, 155, strBuffer, BROWN);
				display_NTU_2();
#else 

		        if ((currentData.S2PPM) >= 20000)
		            sprintf(strBuffer, "2000.0");	
		        else
		            sprintf(strBuffer, "%4d.%01d ", currentData.S2PPM / 10, currentData.S2PPM % 10);

                //DrawMediumNumber(95, 155, strBuffer, BROWN);
                //DrawTextETC(190, 157, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
                DrawMediumNumber(55, 155, strBuffer, BROWN);

				display_2ch_small_unit_ec();
#endif

                sprintf(strBuffer, "%3d.%01d ", currentData.S2mV / 10, currentData.S2mV % 10);
                //DrawMediumNumber(283, 155, strBuffer, BROWN);
                //DrawTextETC(350, 157, TEXT_ETC_MV, DRAW_IMAGE_ENABLE);
                DrawMediumNumber(245, 155, strBuffer, BROWN);
				display_mV_1();

	            // ùùùù ùùùù ù?ù, ù?ù
	            if (Sensor_State2 == SENSOR_OK) display_set10_sensor(0); // ùùùù 
	            else if (Sensor_State2 == SENSOR_ERROR1) display_set10_sensor(1); // ù?ù, ùùùù ùùùù 
	            else if (Sensor_State2 == SENSOR_ERROR2) display_set10_sensor(2); // ù?ù, ùùùù ùùùù
	            else display_set10_sensor(3); // ùùùù ùùùù ùùùù
			}	

            break;

            // ANALOG 
        case STATE_CONFIG_OUTPUT:
            ClearViewArea2();
            //	ClearViewArea();

            display_analog();
            display_4mA();
            display_20mA();

#ifndef  SENSOR_PH_EC
			if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            Draw_Back_Oval4_mgl(45, 144);
    	        Draw_Back_Oval4_mgl(45, 208);
			}
			else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            //Draw_Back_Oval4_mgl(45, 144);
    	        //Draw_Back_Oval4_mgl(45, 208);
	            Draw_Back_Oval4_ntu(45, 144);
    	        Draw_Back_Oval4_ntu(45, 208);
			}
#else
			if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            Draw_Back_Oval4(45, 144);
    	        Draw_Back_Oval4(45, 208);
			}
			else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            //Draw_Back_Oval4_mgl(45, 144);
    	        //Draw_Back_Oval4_mgl(45, 208);
	            Draw_Back_Oval4_uscm(45, 144);
    	        Draw_Back_Oval4_uscm(45, 208);
			}
#endif

            Draw_Back_Oval4(260, 144);
            Draw_Back_Oval4(260, 208);


            //	  	DrawTextsize96(45, 126, TEXT96_4MA, DRAW_IMAGE_ENABLE);	
            //		Draw_Back_Oval2_mgl(45,144);  


            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

                sprintf(strBuffer, "  0  ");
	            DrawSmallNumber(100, 152, strBuffer, BROWN);


//                sprintf(strBuffer, "%1d.%02d", configData.outputConfig.output20mA / 100, configData.outputConfig.output20mA % 100);
#ifndef  SENSOR_PH_EC
                sprintf(strBuffer, "  2  ");
#else 
                sprintf(strBuffer, "  14 ");
#endif
	            DrawSmallNumber(100, 217, strBuffer, BROWN);
			}
			else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                sprintf(strBuffer, "  0  ");
	            DrawSmallNumber(100, 152, strBuffer, BROWN);


#ifndef  SENSOR_PH_EC
                sprintf(strBuffer, "  10  ");
#else
                sprintf(strBuffer, " 2000 ");
#endif
	            DrawSmallNumber(90, 217, strBuffer, BROWN);
			}


            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            sprintf(strBuffer, "%05d", tempConfigData.outputConfig.outputReal4mA);
    	        DrawSmallNumber(320, 152, strBuffer, BROWN);
	            sprintf(strBuffer, "%05d", tempConfigData.outputConfig.outputReal20mA);
    	        DrawSmallNumber(320, 217, strBuffer, BROWN);
			}
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            sprintf(strBuffer, "%05d", tempConfigData.outputConfig.outputReal4mA2);
    	        DrawSmallNumber(320, 152, strBuffer, BROWN);
	            sprintf(strBuffer, "%05d", tempConfigData.outputConfig.outputReal20mA2);
    	        DrawSmallNumber(320, 217, strBuffer, BROWN);
			}


            break;

		// ùùù ùùùù
        case STATE_CONFIG_COMM:
            //		ClearViewArea();
            ClearViewArea2();

            display_comm();
            display_comm_method();
            display_comm_number();
            display_comm_baud();
            display_comm_databit();

            Draw_Back_Oval4(45, 144);
            Draw_Back_Oval4(45, 208);

            Draw_Back_Oval4(260, 144);
            Draw_Back_Oval4(260, 208);


			if (tempConfigData.modbusConfig.mode!=2) {

	            sprintf(strBuffer, "       ");
	            //DrawMediumNumber(230 + 66, 170, strBuffer, color);
	            DrawSmallNumber(100, 152, strBuffer, YELLOW);

	            //DrawMediumNumber(230 + 66, 70, (char*) comModeText[tempConfigData.modbusConfig.mode], color);
	            DrawSmallNumber(115, 152, (char*) comModeText[tempConfigData.modbusConfig.mode], YELLOW);
			}
			else {
				display_Ethernet(100, 152);
			}


            //		DrawTextsize120(70, 120, TEXT120_COM_BAUD, DRAW_IMAGE_ENABLE);
            DrawSmallNumber(80, 217, (char*) comBaudrateText[tempConfigData.modbusConfig.baudrate], BROWN);
            //		DrawLineRectangle(210, 113, 382, 153, BROWN);

            //		DrawTextsize120(70, 170, TEXT120_MODBUSID, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%3d", tempConfigData.modbusConfig.modbusSlaveAddr + 1);
            DrawSmallNumber(315, 152, strBuffer, BROWN);
            //		DrawLineRectangle(210, 163, 382, 203, BROWN);

            //		DrawTextsize120(70, 220, TEXT120_COM_DATABIT, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%d", tempConfigData.modbusConfig.databit + 8);
            DrawSmallNumber(340, 217, strBuffer, BROWN);
            //		DrawLineRectangle(210, 213, 382, 253, BROWN);

            break;


		// ETHERNET ùùù ùùùù
        case STATE_CONFIG_COMM_ETHERNET:
            //		ClearViewArea();
            ClearViewArea2();

            display_Ethernet2();
            display_IP_addr();
            display_gateway();
            display_subnet();
            display_port();

            Draw_Back_Oval4(45, 144);
            Draw_Back_Oval4(45, 208);

            Draw_Back_Oval4(260, 144);
            Draw_Back_Oval4(260, 208);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR0);
            DrawSmallNumber(50, 152, strBuffer, YELLOW);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR1);
            DrawSmallNumber(90, 152, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR2);
            DrawSmallNumber(130, 152, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.IP_ADDR3);
            DrawSmallNumber(170, 152, strBuffer, BROWN);


            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR0);
            DrawSmallNumber(50, 217, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR1);
            DrawSmallNumber(90, 217, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR2);
            DrawSmallNumber(130, 217, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.NETMASK_ADDR3);
            DrawSmallNumber(170, 217, strBuffer, BROWN);


            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR0);
            DrawSmallNumber(270, 152, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR1);
            DrawSmallNumber(310, 152, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR2);
            DrawSmallNumber(350, 152, strBuffer, BROWN);

            sprintf(strBuffer, "%3d", tempConfigData.EthernetConfig.GW_ADDR3);
            DrawSmallNumber(390, 152, strBuffer, BROWN);

            sprintf(strBuffer, "%5d", tempConfigData.EthernetConfig.ETHERNET_PORT);
            DrawSmallNumber(330, 217, strBuffer, BROWN);


            break;


        case STATE_CONFIG_RELAY:
            break;


            // ùù?ùù
        case STATE_CONFIG_TIME:
            ClearViewArea2();
            display_time();

            display_year();
            display_mon();
            display_day();
            display_hour();
            display_min();
            display_sec();

            Draw_Back_Oval6(56, 144);
            Draw_Back_Oval6(197, 144);
            Draw_Back_Oval6(340, 144);
            Draw_Back_Oval6(56, 207);
            Draw_Back_Oval6(197, 207);
            Draw_Back_Oval6(340, 207);


            //DrawUnit(150, 100, UNIT_YEAR, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%4d", tempConfigTime.tm_year);
            //DrawMediumNumber(49, 100, strBuffer, YELLOW);
            DrawSmallNumber(78, 152, strBuffer, YELLOW);
            //DrawLineRectangle(39, 93, 144, 133, BROWN);

            //DrawUnit(275, 100, UNIT_MONTH, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_mon + 1);
            //DrawMediumNumber(219, 100, strBuffer, BROWN);
            DrawSmallNumber(230, 152, strBuffer, BROWN);
            //DrawLineRectangle(209, 93, 270, 133, BROWN);

            //DrawUnit(400, 100, UNIT_DAY, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_mday);
            //DrawMediumNumber(343, 100, strBuffer, BROWN);
            DrawSmallNumber(373, 152, strBuffer, BROWN);
            //DrawLineRectangle(336, 93, 394, 133, BROWN);

            //DrawUnit(150, 200, UNIT_HOUR, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_hour);
            //DrawMediumNumber(93, 200, strBuffer, BROWN);
            DrawSmallNumber(90, 216, strBuffer, BROWN);
            //DrawLineRectangle(83, 193, 144, 233, BROWN);

            //DrawUnit(275, 200, UNIT_MIN, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_min);
            //DrawMediumNumber(219, 200, strBuffer, BROWN);
            DrawSmallNumber(230, 216, strBuffer, BROWN);
            //DrawLineRectangle(209, 193, 270, 233, BROWN);

            //DrawUnit(400, 200, UNIT_SEC, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%2d", tempConfigTime.tm_sec);
            //DrawMediumNumber(343, 200, strBuffer, BROWN);
            DrawSmallNumber(373, 216, strBuffer, BROWN);
            //DrawLineRectangle(336, 193, 394, 233, BROWN);
            break;


        case STATE_CONFIG_ADJUST:
            break;


        //============================
        //============================
        //============================
        //============================


#ifndef  SENSOR_PH_EC
        // calib-ùùù?ùùù
        case STATE_CALIB_ZERO:
            ClearViewArea2();
			display_zero();

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

	            Draw_Back_Oval2_mgl(115, 163);
                zerointVal = tempConfigData.calibrationConfig.S1zeroCal;
                //DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
                //DrawTextETC(402, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);

                if (zerointVal < 0) {
                    zerointVal = zerointVal * (-1);
                }
                if (tempConfigData.calibrationConfig.S1zeroCal < 0) {
                    sprintf(strBuffer, "-%d.%d", zerointVal / 100, (zerointVal / 10) % 10);
                    //DrawMediumNumber(270, 120, strBuffer, YELLOW);
                    //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                    DrawMediumNumber(192, 175, strBuffer, YELLOW);
                    sprintf(strBuffer, "%d", zerointVal % 10);
                    //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, BROWN);
                    //DrawSmallNumber(210 + (12*4), 175, strBuffer, BROWN);
                    DrawMediumNumber(192+25*3, 175, strBuffer, BROWN);
                }
                else {
                    sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal / 100, (tempConfigData.calibrationConfig.S1zeroCal / 10) % 10);
                    //DrawMediumNumber(270, 120, strBuffer, YELLOW);
                    //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                    DrawMediumNumber(192, 175, strBuffer, YELLOW);
                    sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal % 10);
                    //DrawMediumNumber(270 + (22 * 4), 120, strBuffer, BROWN);
                    //DrawSmallNumber(210 + (12*4), 175, strBuffer, BROWN);
                    DrawMediumNumber(192+25*3, 175, strBuffer, BROWN);
                }
            }

            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                Draw_Back_Oval2_ntu(115, 163);
	            zerointVal = tempConfigData.calibrationConfig.S2zeroCal;
                //DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
                //DrawTextETC(402, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);

                if (zerointVal < 0) {
                    zerointVal = zerointVal * (-1);
                }
                if (tempConfigData.calibrationConfig.S2zeroCal < 0) {
                    sprintf(strBuffer, "-%d.%d", zerointVal / 1000, (zerointVal / 100) % 10);
                    //DrawMediumNumber(248, 120, strBuffer, YELLOW);
                    //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                    DrawMediumNumber(192, 175, strBuffer, YELLOW);
                    sprintf(strBuffer, "%02d", zerointVal % 100);
                    //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, BROWN);
                    //DrawSmallNumber(210 + (12*4), 175, strBuffer, BROWN);
                    DrawMediumNumber(192+25*3, 175, strBuffer, BROWN);
                }
                else {
                    sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal / 1000, (tempConfigData.calibrationConfig.S2zeroCal / 100) % 10);
                    //DrawMediumNumber(248, 120, strBuffer, YELLOW);
                    //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                    DrawMediumNumber(192, 175, strBuffer, YELLOW);
                    sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal % 100);
                    //DrawMediumNumber(248 + (22 * 4), 120, strBuffer, BROWN);
                    //DrawSmallNumber(210 + (12*4), 175, strBuffer, BROWN);
                    DrawMediumNumber(192+25*3, 175, strBuffer, BROWN);
                }

            }
            break;


         // ùùùù ùùùù
        case STATE_CALIB_MANUAL:
            ClearViewArea2();
			display_span();

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            Draw_Back_Oval2_mgl(115, 163);
                //DrawTextsize180(50, 120, TEXT180_S1_MANUAL_CAL, DRAW_IMAGE_ENABLE);
                //DrawTextETC(400, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
				if ((tempConfigData.calibrationConfig.S1manualCal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1manualCal / 100, (tempConfigData.calibrationConfig.S1manualCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, YELLOW);
	                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, BROWN);
	                //DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
	                DrawMediumNumber(174+18*5, 175, strBuffer, BROWN);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1manualCal / 100, (tempConfigData.calibrationConfig.S1manualCal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, YELLOW);
	                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                DrawMediumNumber(192, 175, strBuffer, YELLOW);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, BROWN);
	                //DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
	                DrawMediumNumber(192+18*4, 175, strBuffer, BROWN);
				}

            }
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            Draw_Back_Oval2_ntu(115, 163);
                //DrawTextsize180(50, 120, TEXT180_S1_MANUAL_CAL, DRAW_IMAGE_ENABLE);
                //DrawTextETC(400, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);

				if ((tempConfigData.calibrationConfig.S2manualCal)>=10000) {
	                sprintf(strBuffer, "% d.%d ", tempConfigData.calibrationConfig.S2manualCal / 1000, (tempConfigData.calibrationConfig.S2manualCal / 100) % 10);
	                //DrawMediumNumber(255, 120, strBuffer, YELLOW);
	                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);

	                sprintf(strBuffer, "%02d ", tempConfigData.calibrationConfig.S2manualCal % 100);
	                //DrawMediumNumber(255 + (22 * 4), 120, strBuffer, BROWN);
	                //DrawSmallNumber(210+ 12*4, 175, strBuffer, BROWN);
	                DrawMediumNumber(174+18*5, 175, strBuffer, BROWN);
				}
				else {
	                sprintf(strBuffer, "% d.%d ", tempConfigData.calibrationConfig.S2manualCal / 1000, (tempConfigData.calibrationConfig.S2manualCal / 100) % 10);
	                //DrawMediumNumber(255, 120, strBuffer, YELLOW);
	                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                DrawMediumNumber(192, 175, strBuffer, YELLOW);

	                sprintf(strBuffer, "%02d ", tempConfigData.calibrationConfig.S2manualCal % 100);
	                //DrawMediumNumber(255 + (22 * 4), 120, strBuffer, BROWN);
	                //DrawSmallNumber(210+ 12*4, 175, strBuffer, BROWN);
	                DrawMediumNumber(192+18*4, 175, strBuffer, BROWN);
				}
            }

            break;


         // ùùù ùùùù
        case STATE_CALIB_TEMP:
            ClearViewArea2();
			display_temp();
            Draw_Back_Oval2_c(115, 163);

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {

                if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {

                    //DrawTextsize180(50, 120, TEXT180_S1_TEMP_CAL, DRAW_IMAGE_ENABLE);
                    //DrawTextETC(400, 120, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
					if ((compareSignValue/100)>=10) {

	                    tempIntVal = compareSignValue;
	                    if (tempIntVal < 0)
	                        tempIntVal = tempIntVal * (-1);
	                    if (compareSignValue < 0)
	                        sprintf(strBuffer, "-%d.%01d", tempIntVal / 100, (tempIntVal % 100)/10);
	                    else
	                        sprintf(strBuffer, " %d.%01d", compareSignValue / 100, (compareSignValue % 100)/10);
	                    //DrawMediumNumber(260, 120, strBuffer, YELLOW);
	                	//DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                	DrawMediumNumber(182, 175, strBuffer, YELLOW);

						sprintf(strBuffer, "%01d", (compareSignValue % 100)/10);
	                    //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, BROWN);
	                	//DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
	                	DrawMediumNumber(182+18*4, 175, strBuffer, BROWN);
					}
					else {
	                    tempIntVal = compareSignValue;
	                    if (tempIntVal < 0)
	                        tempIntVal = tempIntVal * (-1);
	                    if (compareSignValue < 0)
	                        sprintf(strBuffer, "-%d.%01d", tempIntVal / 100, (tempIntVal % 100)/10);
	                    else
	                        sprintf(strBuffer, " %d.%01d", compareSignValue / 100, (compareSignValue % 100)/10);
	                    //DrawMediumNumber(260, 120, strBuffer, YELLOW);
	                	//DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                	DrawMediumNumber(200, 175, strBuffer, YELLOW);

						sprintf(strBuffer, "%01d", (compareSignValue % 100)/10);
	                    //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, BROWN);
	                	//DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
	                	DrawMediumNumber(200+18*3, 175, strBuffer, BROWN);
					}


                } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
				// ùùùùùù
                    //DrawTextsize180(50, 120, TEXT180_S1_AUTO_CAL_VAULE, DRAW_IMAGE_ENABLE);
                    //DrawTextETC(402, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);

                    sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1Value / 100, (tempConfigData.calibrationConfig.S1Value / 10) % 10);
                    //DrawMediumNumber(270, 120, strBuffer, YELLOW);
                	//DrawSmallNumber(210, 175, strBuffer, YELLOW);
                	DrawMediumNumber(200, 175, strBuffer, YELLOW);
                    sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1Value % 10);
                    //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, BROWN);
                    //DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
                    DrawMediumNumber(200+27*2, 175, strBuffer, BROWN);


                }
            }
            break;

#else
         // ùùùù ùùùù
        case STATE_CALIB_BUFF_PH4:
        case STATE_CALIB_BUFF_PH7:
            ClearViewArea2();

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {

				display_buff();
				if (state==STATE_CALIB_BUFF_PH4)	{
					display_ph4();
		            Draw_Back_Oval2(115, 163);
	                zerointVal = tempConfigData.calibrationConfig.PH4_Cal;
	                //DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
	                //DrawTextETC(402, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);

	                if (zerointVal < 0) {
	                    zerointVal = zerointVal * (-1);
	                }

			        if (tempConfigData.calibrationConfig.PH4_Cal < 0) {
			            sprintf(strBuffer, "-%d.%02d", zerointVal / 100, zerointVal%100);
			            DrawMediumNumber(192, 175, strBuffer, YELLOW);
			        }
			        else {
			            sprintf(strBuffer, " %d.%02d", zerointVal / 100, zerointVal%100);
			            DrawMediumNumber(192, 175, strBuffer, YELLOW);
			        }
				}
				else  {
					display_ph7();

		            Draw_Back_Oval2(115, 163);
	                zerointVal = tempConfigData.calibrationConfig.PH7_Cal;
	                //DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
	                //DrawTextETC(402, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);

	                if (zerointVal < 0) {
	                    zerointVal = zerointVal * (-1);
	                }

			        if (tempConfigData.calibrationConfig.PH7_Cal < 0) {
			            sprintf(strBuffer, "-%d.%02d", zerointVal / 100, zerointVal%100);
			            DrawMediumNumber(192, 175, strBuffer, YELLOW);
			        }
			        else {
			            sprintf(strBuffer, " %d.%02d", zerointVal / 100, zerointVal%100);
			            DrawMediumNumber(192, 175, strBuffer, YELLOW);
			        }
				}

            }
            break;

		// ùùù?ùùù
        case STATE_CALIB_ZERO:
            ClearViewArea2();
			display_zero();

            if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                Draw_Back_Oval2(115, 163);
	            zerointVal = tempConfigData.calibrationConfig.EC_Cal;
                //DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
                //DrawTextETC(402, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);

                if (zerointVal < 0) {
                    zerointVal = zerointVal * (-1);
                }

		        if (tempConfigData.calibrationConfig.EC_Cal < 0) {
		            sprintf(strBuffer, "-%d.%01d", zerointVal/10, zerointVal%10);
		            DrawMediumNumber(200, 175, strBuffer, YELLOW);
		        }
		        else {
		            sprintf(strBuffer, " %d.%01d", zerointVal/10, zerointVal%10);
		            DrawMediumNumber(200, 175, strBuffer, YELLOW);
		        }
            }
            break;


         // ùùùù ùùùù
        case STATE_CALIB_SPAN:
            ClearViewArea2();
			display_span();

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            Draw_Back_Oval2(115, 163);
                //DrawTextsize180(50, 120, TEXT180_S1_MANUAL_CAL, DRAW_IMAGE_ENABLE);
                //DrawTextETC(400, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
				if ((tempConfigData.calibrationConfig.PH_Span_Cal/100)>=10) {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.PH_Span_Cal / 100, (tempConfigData.calibrationConfig.PH_Span_Cal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, YELLOW);
	                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.PH_Span_Cal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, BROWN);
	                //DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
	                DrawMediumNumber(174+18*5, 175, strBuffer, BROWN);
				}
				else {
	                sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.PH_Span_Cal / 100, (tempConfigData.calibrationConfig.PH_Span_Cal / 10) % 10);
	                //DrawMediumNumber(270, 120, strBuffer, YELLOW);
	                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
	                DrawMediumNumber(192, 175, strBuffer, YELLOW);
	                sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.PH_Span_Cal % 10);
	                //DrawMediumNumber(270 + (22 * 3), 120, strBuffer, BROWN);
	                //DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
	                DrawMediumNumber(192+18*4, 175, strBuffer, BROWN);
				}

            }
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
	            Draw_Back_Oval2(115, 163);

				if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=10000) {
	                sprintf(strBuffer, " %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, BROWN);
				}
				else if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=1000) {
	                sprintf(strBuffer, "  %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, BROWN);
				}
				else if ((tempConfigData.calibrationConfig.EC_Span_Cal)>=100) {
	                sprintf(strBuffer, "   %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, BROWN);
				}
				else {
	                sprintf(strBuffer, "     %d", tempConfigData.calibrationConfig.EC_Span_Cal / 100);
	                DrawMediumNumber(174, 175, strBuffer, YELLOW);

	                sprintf(strBuffer, "%d.%d ", (tempConfigData.calibrationConfig.EC_Span_Cal%100)/10,   tempConfigData.calibrationConfig.EC_Span_Cal % 10);
	                DrawMediumNumber(174+18*4, 175, strBuffer, BROWN);
				}
            }

            break;


         // ùùù ùùùù
        case STATE_CALIB_TEMP:
            ClearViewArea2();
			display_temp();
            Draw_Back_Oval2_c(115, 163);


		   	if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
				TEMP_Span_Cal = tempConfigData.calibrationConfig.TEMP_Span_Cal1;
			}
			else {
				TEMP_Span_Cal = tempConfigData.calibrationConfig.TEMP_Span_Cal2;
			}


            //DrawTextsize180(50, 120, TEXT180_S1_TEMP_CAL, DRAW_IMAGE_ENABLE);
            //DrawTextETC(400, 120, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
			if ((TEMP_Span_Cal)>=100) {

                tempIntVal = TEMP_Span_Cal;
                if (tempIntVal < 0)
                    tempIntVal = tempIntVal * (-1);
                if (compareSignValue < 0)
                    sprintf(strBuffer, "-%d.%01d", tempIntVal / 10, (tempIntVal % 10));
                else
                    sprintf(strBuffer, " %d.%01d", TEMP_Span_Cal / 10, (TEMP_Span_Cal % 10));
                //DrawMediumNumber(260, 120, strBuffer, YELLOW);
            	//DrawSmallNumber(210, 175, strBuffer, YELLOW);
            	DrawMediumNumber(182, 175, strBuffer, YELLOW);

				sprintf(strBuffer, "%01d", (TEMP_Span_Cal % 10));
                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, BROWN);
            	//DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
            	DrawMediumNumber(182+18*4, 175, strBuffer, BROWN);
			}
			else {
                tempIntVal = TEMP_Span_Cal;
                if (tempIntVal < 0)
                    tempIntVal = tempIntVal * (-1);
                if (compareSignValue < 0)
                    sprintf(strBuffer, " -%d.%01d", tempIntVal / 10, (tempIntVal % 10));
                else
                    sprintf(strBuffer, "  %d.%01d", TEMP_Span_Cal / 10, (TEMP_Span_Cal % 10));
                //DrawMediumNumber(260, 120, strBuffer, YELLOW);
            	//DrawSmallNumber(210, 175, strBuffer, YELLOW);
            	DrawMediumNumber(182, 175, strBuffer, YELLOW);

				sprintf(strBuffer, "%01d", (TEMP_Span_Cal % 10));
                //DrawMediumNumber(260 + (22 * 3), 120, strBuffer, BROWN);
            	//DrawSmallNumber(210+ 12*3, 175, strBuffer, BROWN);
            	DrawMediumNumber(182+18*4, 175, strBuffer, BROWN);
			}
            break;
#endif

            //
        case STATE_CALIB_S2_CYCLE:
            ClearViewArea();
            //DrawTextsize180(50, 100, TEXT180_S2_PERIOD, DRAW_IMAGE_ENABLE);
            sprintf(strBuffer, "%3d", tempConfigData.calibrationConfig.S2cycle);
            DrawMediumNumber(270, 100, strBuffer, YELLOW);
            DrawLineRectangle(250, 93, 356, 133, BROWN);
            break;


            // ùùùùù?ù 
        case STATE_CALIB_LOG:
            ClearViewArea4();

            TFT_Fill(20, 102, 460, 104, BACK_COLOR3);
            TFT_Fill(20, 152, 460, 154, BACK_COLOR3);
            TFT_Fill(20, 202, 460, 204, BACK_COLOR3);
            //TFT_Fill(20, 252, 455, 254, BACK_COLOR3);
            break;

        case STATE_CONFIG_RELAY_RELAY1:
            break;
        case STATE_CONFIG_RELAY_RELAY2:
            break;
        case STATE_CONFIG_RELAY_RELAY3:
            break;


        //============================
        //============================
        //============================
        //============================
        // ùùùù-ùùùùù
        case STATE_CONFIG_ADJUST_GRADIENT:
            ClearViewArea2();
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                //DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
                display_slope();
                Draw_Back_Oval2(115, 163);

                if (tempConfigData.adjustConfig.gradientS1 > 999) tempConfigData.adjustConfig.gradientS1 = 0;

                sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS1 / 100, tempConfigData.adjustConfig.gradientS1 % 100);
                //DrawMediumNumber(250, 165, strBuffer, YELLOW);
                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                DrawMediumNumber(200, 175, strBuffer, YELLOW);
                //DrawLineRectangle(230, 152, 358, 207, BROWN);
            } else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                display_slope();
                Draw_Back_Oval2(115, 163);

                if (tempConfigData.adjustConfig.gradientS2 > 999) tempConfigData.adjustConfig.gradientS2 = 0;

                //DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
                sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS2 / 100, tempConfigData.adjustConfig.gradientS2 % 100);
                //DrawMediumNumber(250, 165, strBuffer, YELLOW);
                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                DrawMediumNumber(200, 175, strBuffer, YELLOW);
                //DrawLineRectangle(230, 152, 358, 207, BROWN);
            }

            break;

        // ùùùù-ùùùùùù
        case STATE_CONFIG_ADJUST_OFFSET:
            ClearViewArea2();

            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
	            offsetintVal = tempConfigData.adjustConfig.offsetS1;

                //DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
                display_offset();
                Draw_Back_Oval2(115, 163);


                if (offsetintVal < 0) {
                    offsetintVal = offsetintVal * (-1);
                }
                if (tempConfigData.adjustConfig.offsetS1 < 0) {
	                if (tempConfigData.adjustConfig.offsetS1 <= -1000) {
	                    sprintf(strBuffer, "-%2d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(180, 175, strBuffer, YELLOW);
					}
					else {
	                    sprintf(strBuffer, "-%d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(200, 175, strBuffer, YELLOW);
					}
                } else {
	                if (tempConfigData.adjustConfig.offsetS1 >= -1000) {
	                    sprintf(strBuffer, " %d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(180, 175, strBuffer, YELLOW);
					}
					else {
	                    sprintf(strBuffer, " %2d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(200, 175, strBuffer, YELLOW);
					}
                }
                //		  	DrawLineRectangle(230, 152, 368, 207, BROWN);
            }
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                //DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);

	            offsetintVal = tempConfigData.adjustConfig.offsetS2;
                display_offset();
                Draw_Back_Oval2(115, 163);

                if (offsetintVal < 0) {
                    offsetintVal = offsetintVal * (-1);
                }
                if (tempConfigData.adjustConfig.offsetS2 < 0) {
	                if (tempConfigData.adjustConfig.offsetS2 <= -1000) {
	                    sprintf(strBuffer, "-%2d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(180, 175, strBuffer, YELLOW);
					}
					else {
	                    sprintf(strBuffer, "-%d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(180, 175, strBuffer, YELLOW);
					}
                } else {
	                if (tempConfigData.adjustConfig.offsetS2 >= 1000) {
	                    sprintf(strBuffer, " %2d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(180, 175, strBuffer, YELLOW);
					}
					else {
	                    sprintf(strBuffer, " %d.%02d", offsetintVal / 100, offsetintVal % 100);
	                    DrawMediumNumber(180, 175, strBuffer, YELLOW);
					}
                }
                //DrawLineRectangle(220, 152, 358, 207, BROWN);
            }
            break;

            // ùùùù-ùùùù
        case STATE_CONFIG_ADJUST_FILTER:
            ClearViewArea2();
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE) {
                //DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
                display_filter();
                Draw_Back_Oval2(115, 163);

                sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS1);
                //DrawMediumNumber(250, 165, strBuffer, YELLOW);
                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                DrawMediumNumber(210, 175, strBuffer, YELLOW);
                //DrawLineRectangle(230, 152, 336, 207, BROWN);
            }
            else if (currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                //DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
                display_filter();
                Draw_Back_Oval2(115, 163);

                sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS2);
                //DrawMediumNumber(250, 165, strBuffer, YELLOW);
                //DrawSmallNumber(210, 175, strBuffer, YELLOW);
                DrawMediumNumber(210, 175, strBuffer, YELLOW);
                //DrawLineRectangle(230, 152, 336, 207, BROWN);
            }

            break;


            // ùùùù-ù?ù?
        case STATE_CONFIG_ADJUST_FACTORYRESET:
            ClearViewArea2();
            //DrawTextETC(115, 130, TEXT_ETC_F_RESET_MESSAGE, DRAW_IMAGE_ENABLE);
            display_init();
            display_message6();
            break;


            //=================================


		// ù?ùùùùù
        case STATE_CONFIG_RELAY_RELAY1_AUTO_WASH:
            ClearViewArea2();
            display_autowash();
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                display_set_period();
                display_set_time();
                Draw_Back_Oval2_min(150, 135);
                Draw_Back_Oval2_min(150, 195);


                //DrawTextsize55(90, 100, TEXT55_WASH, DRAW_IMAGE_ENABLE);	
                //DrawTextsize55(150, 100, TEXT55_CYCLE, DRAW_IMAGE_ENABLE);	
                //DrawUnit(400, 100, UNIT_MIN, DRAW_IMAGE_ENABLE);

                sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashCycle);
                //DrawMediumNumber(250, 100, strBuffer, YELLOW);
                DrawSmallNumber(250, 148, strBuffer, YELLOW);
                //DrawLineRectangle(230, 93, 336, 133, BROWN);


                //DrawTextsize55(90, 150, TEXT55_WASH, DRAW_IMAGE_ENABLE);	
                //DrawTextsize55(150, 150, TEXT55_TIME, DRAW_IMAGE_ENABLE);
                //DrawUnit(400, 150, UNIT_MIN, DRAW_IMAGE_ENABLE);

                sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashTime);
                //DrawMediumNumber(250, 150, strBuffer, BROWN);
                DrawSmallNumber(250, 207, strBuffer, BROWN);
                //DrawLineRectangle(230, 143, 336, 183, BROWN);
            }
            else {
            }
            break;


		// ùùùùùùùù
        case STATE_CONFIG_RELAY_RELAY1_MAN_WASH:
            //ClearViewArea();
            ClearViewArea3();
            if (tempConfigData.relayConfig.relay1manual) {
                //		     DrawTextETC(L_NUMBER1_X, L_NUMBER3_Y, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
                display_set11_run(1);
                display_set11_stop(0);

				display_message1();	//RUNNing

            } else {
                //			   DrawTextETC(L_NUMBER1_X, L_NUMBER3_Y, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
                display_set11_run(0);
                display_set11_stop(1);
				display_message_STOP();	//STOP

            }
            //		display_set11_running();
            //display_set11_manwash();
            display_manwash();

            break;
        case STATE_CONFIG_RELAY_RELAY2_AUTO_ALARM:
            ClearViewArea3();
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) {
                //			if(tempConfigData.relayConfig.relay2AutoAlarm)
                //				DrawTextsize55(L_NUMBER1_X, L_NUMBER3_Y, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
                //		  	else
                //				DrawTextsize55(L_NUMBER1_X, L_NUMBER3_Y, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
            }
            else {
            }
            break;

        case STATE_CONFIG_RELAY_RELAY2_ALARM_OFF:
            ClearViewArea3();
            //		display_alarm();
            //		display_message3();

            //		if(tempConfigData.relayConfig.relay2AlarmOFF)
            //			DrawTextsize55(L_NUMBER1_X, L_NUMBER3_Y, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
            //		else
            //			DrawTextsize55(L_NUMBER1_X, L_NUMBER3_Y, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
            break;

            // ùùùù-ùùùùùù-ùùùùùù-ùùùùùùùù
        case STATE_CONFIG_RELAY_RELAY3_SET:
            ClearViewArea2();
            if (currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE) // // TEXT96_CONFIG DrawTextsize96(90, 200, TEXT96_FILTER, DRAW_IMAGE_ENABLE);
            {
                display_act_set();
                display_set_period();
                display_set_time();
                Draw_Back_Oval2_min(150, 134);
                Draw_Back_Oval2_min(150, 195);

                //DrawTextsize96(70, 100, TEXT96_CONFIG, DRAW_IMAGE_ENABLE);	DrawTextsize55(150, 100, TEXT55_CYCLE, DRAW_IMAGE_ENABLE);DrawUnit(350, 100, UNIT_MIN, DRAW_IMAGE_ENABLE);
                sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashCycle);
                //DrawMediumNumber(250, 100, strBuffer, YELLOW);
                DrawSmallNumber(220, 145, strBuffer, YELLOW);
                //DrawLineRectangle(230, 93, 336, 133, BROWN);
                //DrawTextsize96(70, 150, TEXT96_CONFIG, DRAW_IMAGE_ENABLE);	DrawTextsize55(150, 150, TEXT55_TIME, DRAW_IMAGE_ENABLE);DrawUnit(350, 150, UNIT_MIN, DRAW_IMAGE_ENABLE);
                sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashTime);
                //DrawMediumNumber(250, 150, strBuffer, BROWN);
                DrawSmallNumber(220, 203, strBuffer, BROWN);
                //DrawLineRectangle(230, 143, 336, 183, BROWN);
            }
            else {
            }
            break;

            // ùùùù-ùùùùùù-ùùùùùù-ùùùùùùùù
        case STATE_CONFIG_RELAY_RELAY3_MAN:
            //		ClearViewArea();
            ClearViewArea3();
            display_act_manual(); //ùùùùùùùù
            display_message_STOP(); //stop

            display_set11_run(0);
            display_set11_stop(1);

            //		if(tempConfigData.relayConfig.relay3manual)
            //			 DrawTextETC(L_NUMBER1_X, L_NUMBER3_Y, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
            //		else
            //			 DrawTextETC(L_NUMBER1_X, L_NUMBER3_Y, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
            break;
    }
}

void RedrawBottomArea(void) {
    switch (state) {
        case STATE_MAIN:
            //ClearBottomArea();	   //ùùù?ùù?ù....

            //TFT_Fill(0, 234, 480, 236, BROWN);


            //DrawBottomLine();
            RedrawTime();

            //Draw_Back_Oval_140x42(322, 220);
            //DrawTextETC(400, 225, TEXT_ETC_MA, DRAW_IMAGE_ENABLE);
            //DrawBottomCurrent(currentData.current4_20mA);

			if (comm_type==COMM_RS232)		display_RS_232();
			else if (comm_type==COMM_RS485)	display_RS_485();
			else 							display_4_20mA();


            break;


        case STATE_MENU:
            ClearBottomArea();

            TFT_Fill(0, 220, 480, 272, WHITE);
            display_set_set(1);
            display_set_cal(0);
            display_set_trand(0);
            display_set_alarm(0);
            display_set_diag(0);

            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //		DrawTextsize96(0, 240, TEXT96_CONFIG, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(96, 240, TEXT96_CALIBRATION, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(192, 240, TEXT96_TREND, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(288, 240, TEXT96_ALARM, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(384, 240, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
            //		DrawLineRectangle(0+(96*0), 238, 96+(0*96), 272, YELLOW);
            break;
        case STATE_CONFIG:
            ClearBottomArea();

            display_set2_analog(1);
            display_set2_calib(0);
            display_set2_comm(0);
            display_set2_relay(0);
            display_set2_time(0);

            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //		DrawTextsize96(0, 240, TEXT96_ANALOG, DRAW_IMAGE_ENABLE);
            //		DrawTextsize55(116, 240, TEXT55_COMMUNICATION, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(192, 240, TEXT96_RELAY, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(288, 240, TEXT96_TIME, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(384, 240, TEXT96_ADJUST, DRAW_IMAGE_ENABLE);
            //		DrawLineRectangle(0+(96*0), 238, 96+(0*96), 272, YELLOW);
            break;
        case STATE_CALIB:
            ClearBottomArea();
			// if (currentData.Device_Selector_Mode & SENSOR_2_MODE) 
            ClearBottomArea();

#ifndef   SENSOR_PH_EC
            display_set3_zero(1);
#else
			if (currentData.Device_Selector_Mode == SENSOR_1_MODE)    display_set3_buff(1);
			else     display_set3_zero(1);
#endif

            display_set3_span(0);
            display_set3_temp(0);
            display_set3_log(0);
            break;

        case STATE_TREND:
            ClearBottomArea();
            break;
        case STATE_ALARM:
            //ClearBottomArea();
            break;
        case STATE_DIAGNOSTIC:
            //ClearBottomArea();
            break;

        case STATE_CONFIG_OUTPUT:
            ClearBottomArea();
            break;
        case STATE_CONFIG_COMM:
            ClearBottomArea();
            break;
        case STATE_CONFIG_RELAY:
            ClearBottomArea();

            display_set5_clean(1);
            display_set5_alarm(0);
            display_set5_relay(0);

            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //	  DrawTextsize55(50, 240, TEXT55_WASH, DRAW_IMAGE_ENABLE);
            //	  DrawTextsize55(210, 240, TEXT55_ALARM, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(352, 240, TEXT96_RELAY, DRAW_IMAGE_ENABLE);	
            //		DrawLineRectangle(0+(160*0), 238, 160+(0*160), 272, YELLOW);
            break;
        case STATE_CONFIG_TIME:
            ClearBottomArea();
            break;
        case STATE_CONFIG_ADJUST:
            ClearBottomArea();

            display_set4_slope(1);
            display_set4_offset(0);
            display_set4_filter(0);
            display_set4_init(0);

            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //		DrawTextsize96(12, 240, TEXT96_GRADIENT, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(132, 240, TEXT96_OFFSET, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(252, 240, TEXT96_FILTER, DRAW_IMAGE_ENABLE);
            //		DrawTextsize96(372, 240, TEXT96_FACTORY_RESET, DRAW_IMAGE_ENABLE);
            //		DrawLineRectangle(0+(120*0), 238, 120+(0*120), 272, YELLOW);
            break;
        case STATE_CALIB_ZERO:
            ClearBottomArea();
            break;
        case STATE_CALIB_TEMP:
            ClearBottomArea();
            break;
        case STATE_CALIB_MANUAL:
            ClearBottomArea();
            break;
        case STATE_CALIB_S2_CYCLE:
            ClearBottomArea();
            break;
        case STATE_CALIB_LOG:
            ClearBottomArea();
            break;

            // ùùùù-ù?ùùùùù,ùùùùùùùù
        case STATE_CONFIG_RELAY_RELAY1:
            ClearBottomArea();
            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //	    DrawTextsize96(75, 240, TEXT96_AUTO_WASHING, DRAW_IMAGE_ENABLE);		
            //	    DrawTextsize96(310, 240, TEXT96_MANUAL_WASH, DRAW_IMAGE_ENABLE);
            //		DrawLineRectangle(0+(240*0), 238, 240+(0*240), 272, YELLOW);
            display_set7_autowash(1);
            display_set7_manwash(0);

            break;

            // ù?-ù?ùù?ù, ù?ùùùùù
        case STATE_CONFIG_RELAY_RELAY2:
            ClearViewArea3();
            display_alarm();
            display_message3();


            ClearBottomArea();
            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //	    DrawTextsize96(75, 240, TEXT96_AUTO_ALARM, DRAW_IMAGE_ENABLE);
            //	    DrawTextsize96(310, 240, TEXT96_ALARM_OFF, DRAW_IMAGE_ENABLE);
            //			DrawLineRectangle(0+(240*0), 238, 240+(0*240), 272, YELLOW);
            display_set8_autoalarm(1);
            display_set8_noalarm(0);

            break;
        case STATE_CONFIG_RELAY_RELAY3:
            ClearBottomArea();

            display_set6_set(1);
            display_set6_passive(0);
            //		TFT_Fill(0, 234, 480, 236, BROWN);
            //	    DrawTextsize96(75, 240, TEXT96_SET_OPERATION, DRAW_IMAGE_ENABLE);
            //	    DrawTextsize96(310, 240, TEXT96_MANUAL_OPERATION, DRAW_IMAGE_ENABLE);
            //			DrawLineRectangle(0+(240*0), 238, 240+(0*240), 272, YELLOW);
            break;
        case STATE_CONFIG_ADJUST_GRADIENT:
            ClearBottomArea();
            break;
        case STATE_CONFIG_ADJUST_OFFSET:
            ClearBottomArea();
            break;
        case STATE_CONFIG_ADJUST_FILTER:
            ClearBottomArea();
            break;
        case STATE_CONFIG_ADJUST_FACTORYRESET:
            ClearBottomArea();
            break;

        case STATE_CONFIG_RELAY_RELAY1_AUTO_WASH:
            ClearBottomArea();
            break;
        case STATE_CONFIG_RELAY_RELAY1_MAN_WASH:
            ClearBottomArea();
            break;
        case STATE_CONFIG_RELAY_RELAY2_AUTO_ALARM:
            ClearBottomArea();
            break;
        case STATE_CONFIG_RELAY_RELAY2_ALARM_OFF:
            ClearBottomArea();
            break;
        case STATE_CONFIG_RELAY_RELAY3_SET:
            ClearBottomArea();
            break;
        case STATE_CONFIG_RELAY_RELAY3_MAN:
            ClearBottomArea();
            break;
		default: 
			ClearBottomArea();
    }
}


void RedrawBottomArea_CH2(void) {
    switch (state) {
        case STATE_MAIN:
            //ClearBottomArea();	   //ùùù?ùù?ù....

            //TFT_Fill(0, 234, 480, 236, BROWN);


            //DrawBottomLine();

            //Draw_Back_Oval_140x42(322, 220);
            //DrawTextETC(400, 225, TEXT_ETC_MA, DRAW_IMAGE_ENABLE);
            //DrawBottomCurrent(currentData.current4_20mA);
#ifndef SENSOR_PH_EC
		    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
			    display_2ch_set_cl2();
				display_2ch_set_tu1();
			}
			else {
			    display_2ch_set_cl1();
				display_2ch_set_tu2();
			}
#else 
		    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
			    display_2ch_set_ph2();
				display_2ch_set_ec1();
			}
			else {
			    display_2ch_set_ph1();
				display_2ch_set_ec2();
			}
#endif
            break;

		default: 
			ClearBottomArea();
    }
}


//==================================================
extern uint16_t Timer10msec;


#ifndef SENSOR_PH_EC

void RedrawMainValue(void) {
	unsigned char imsi, imsi1;

	// ùùùù ùùùù ù?ùùù ùùùù ùùù ù?ùù?ù 	
//	imsi1 = (imsi1 + imsi/10 + Timer10msec + currentTime.tm_sec + currentTime.tm_sec/10 + 37)%7;
//	imsi = imsi1;
//	imsi %=10;

//    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
        // ù?ùùùùù ?ùù
        if ((currentData.S1PPM) >= 9999)
            sprintf(strBuffer, "99.99");	

//        else if ((currentData.S1PPM / 100) >= 10) {
        else 
		    sprintf(strBuffer, "%2d.%02d", (currentData.S1PPM) / 100, (currentData.S1PPM) % 100);
//		    sprintf(strBuffer, "%2d.%03d", (currentData.S1PPM*10+imsi) / 1000, (currentData.S1PPM*10+imsi) % 1000);
//		}
//        else {
//            sprintf(strBuffer, " %1d.%02d ", currentData.S1PPM / 100, currentData.S1PPM % 100);
//		}
		display_2ch_cl();
		Draw_2CH_HOME_Number(38, L_NUMBER1_Y+20, strBuffer);
		display_2ch_unit_cl();


		if (Relay1_run_on_flag == 0 && Relay3_run_on_flag == 0){

	        // ùùù ?ùù
	        if ((currentData.temperature) >= 9999) sprintf(strBuffer, " 99.9C ");	
	        else if ((currentData.temperature) <= -9999) sprintf(strBuffer, "-99.9C ");	
	        else if ((currentData.temperature) >= 0) 	sprintf(strBuffer, " %02d.%01dC ",      currentData.temperature / 100, currentData.temperature/10 % 10); // C is 'C
	        else 										sprintf(strBuffer, "-%02d.%01dC ", (abs(currentData.temperature)) / 100, (abs(currentData.temperature/10)) % 10); // C is 'C

	        // ùùù ?ùù, ùùù ùùù
	//        Draw_Back_Oval_100x30(80, 152);

	//        if ((currentData.temperature) >= 9999) sprintf(strBuffer, "99.9C");	
	//        else if ((currentData.temperature) <= -9999) sprintf(strBuffer, "-99.9C");	
	//        else sprintf(strBuffer, "%02d.%01dC", currentData.temperature / 100, currentData.temperature % 100); // C is 'C

	        //DrawMediumNumber(200, 170, strBuffer, BROWN);
			TFT_Fill(90, 154, 90+84, 154+5, BACK_COLOR3);
			TFT_Fill(90, 175, 90+84, 175+5, BACK_COLOR3);
	        Draw_2CH_TEMP_Number(90, 159, strBuffer, DRAW_NORMAL);

	//		sprintf(strBuffer, "C");	
	//        DrawSmallNumber(210+54, 159, strBuffer, BACK_COLOR3);


	        // ùùùùù? ('C) ?ùù
	        //DrawTextETC(280, 170, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
		}
//    }

	// ?ùù ?ùù
//    if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {

        if ((currentData.S2PPM) >= 99999)
            sprintf(strBuffer, "99.999 ");	

//        else if ((currentData.S2PPM / 1000) >= 10)
//            sprintf(strBuffer, "%01d.%03d ", currentData.S2PPM / 1000, currentData.S2PPM % 1000);
        else
            sprintf(strBuffer, "%2d.%03d ", (currentData.S2PPM) / 1000, (currentData.S2PPM) % 1000);
//            sprintf(strBuffer, "%2d.%04d", (currentData.S2PPM*10+imsi) / 10000, (currentData.S2PPM*10+imsi) % 10000);

		display_2ch_tu();
		// 2ù?ùù?ù ùù 1ù?ùù?ù ù??ùù?ùùùù?ù ùùùùùù
		if (currentData.S2PPM<10000)        	Draw_2CH_HOME_Number(256, L_NUMBER1_Y+20, strBuffer);
        else 								Draw_2CH_HOME_Number(262, L_NUMBER1_Y+20, strBuffer);
		display_2ch_unit_tu();

        //DrawTextETC(363, 170, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
        //sprintf(strBuffer, "%02d.%02d", currentData.temperature/100, currentData.temperature%100);
        //DrawMediumNumber(235, 170, strBuffer, BROWN);
//    }

    /* EXT sensor overlay: Draw_2CH_HOME_Number reused at same coords as S1/S2PPM.
     * raw[0] used directly (scaled_value is int32_t; fractional part would be lost).
     *   sensors[0] pH_EXT  scale=0.01 : raw 700  -> " 7.00"
     *   sensors[1] EC_EXT  scale=1.0  : raw 1234 -> "123.4"
     * Show "--.-  " while status != SMGR_STATUS_OK (timeout / CRC error / pending).
     * DRAW_NORMAL keeps original NAND bitmap colors identical to existing S1/S2PPM. */
    {
        uint16_t ext_raw;

        if (g_ext_sensor_count >= 1) {
            if (g_ext_sensors[0].status == SMGR_STATUS_OK) {
                ext_raw = g_ext_sensors[0].raw[0];
                sprintf(strBuffer, "%2d.%02d", ext_raw / 100, ext_raw % 100);
            } else {
                sprintf(strBuffer, "--.-  ");
            }
            Draw_2CH_HOME_Number(38, L_NUMBER1_Y + 20, strBuffer, DRAW_NORMAL);
        }

        if (g_ext_sensor_count >= 2) {
            if (g_ext_sensors[1].status == SMGR_STATUS_OK) {
                ext_raw = g_ext_sensors[1].raw[0];
                sprintf(strBuffer, "%4d.%01d", ext_raw / 10, ext_raw % 10);
            } else {
                sprintf(strBuffer, "---.-  ");
            }
            Draw_2CH_HOME_Number(256, L_NUMBER1_Y + 20, strBuffer, DRAW_NORMAL);
        }
    }
}


#else	// define SENSOR_PH_EC


void RedrawMainValue(void) {
	unsigned char imsi, imsi1;

	// ùùùù ùùùù ù?ùùù ùùùù ùùù ù?ùù?ù 	
//	imsi1 = (imsi1 + imsi/10 + Timer10msec + currentTime.tm_sec + currentTime.tm_sec/10 + 37)%7;
//	imsi = imsi1;
//	imsi %=10;

//    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
        // PH ?ùù
        if ((currentData.S1PPM) >= 1400)
            sprintf(strBuffer, "14.00");	

//        else if ((currentData.S1PPM / 100) >= 10) {
        else 
		    sprintf(strBuffer, "%2d.%02d", (currentData.S1PPM) / 100, (currentData.S1PPM) % 100);
//		    sprintf(strBuffer, "%2d.%03d", (currentData.S1PPM*10+imsi) / 1000, (currentData.S1PPM*10+imsi) % 1000);
//		}
//        else {
//            sprintf(strBuffer, " %1d.%02d ", currentData.S1PPM / 100, currentData.S1PPM % 100);
//		}
		display_2ch_ph();
       	Draw_2CH_HOME_Number(80, L_NUMBER1_Y+20, strBuffer);
		display_2ch_unit_ph();


		if (Relay1_run_on_flag == 0 && Relay3_run_on_flag == 0){

	        // ùùù ?ùù, ùùù ùùù
	//        Draw_Back_Oval_100x30(190, 152);

	//        if ((currentData.temperature) >= 9999) sprintf(strBuffer, "99.9C");	
	//        else if ((currentData.temperature) <= -9999) sprintf(strBuffer, "-99.9C");	
	//        else sprintf(strBuffer, "%02d.%01dC", currentData.temperature / 100, currentData.temperature % 100); // C is 'C

	        if ((currentData.temperature) >= 9999) sprintf(strBuffer, " 99.9C ");	
	        else if ((currentData.temperature) <= -9999) sprintf(strBuffer, "-99.9C ");	
	        else if ((currentData.temperature) >= 0) 	sprintf(strBuffer, " %02d.%01dC ",      currentData.temperature / 10, currentData.temperature % 10); // C is 'C
	        else 										sprintf(strBuffer, "-%02d.%01dC ", (abs(currentData.temperature)) / 10, (abs(currentData.temperature)) % 10); // C is 'C

	//        else if ((currentData.temperature) >= 0) 	sprintf(strBuffer, " %02d.%01dC ",      currentData.temperature / 100, (currentData.temperature/10) % 10); // C is 'C
	//        else 										sprintf(strBuffer, "-%02d.%01dC ", (abs(currentData.temperature)) / 100, ((abs(currentData.temperature))/10) % 10); // C is 'C


	        // ùùù ?ùù, ùùù ùùù
	//        Draw_Back_Oval_100x30(80, 152);

			TFT_Fill(90, 154, 90+84, 154+5, BACK_COLOR3);
			TFT_Fill(90, 175, 90+84, 175+5, BACK_COLOR3);
	        Draw_2CH_TEMP_Number(90, 159, strBuffer, DRAW_NORMAL);

	        // ùùùùù? ('C) ?ùù
	        //DrawTextETC(280, 170, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
		}
//    }

	// ùùùùùùùùùù ?ùù
//    if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {

        if ((currentData.S2PPM) >= 20000)
            sprintf(strBuffer, "2000.0");	

//        else if ((currentData.S2PPM / 1000) >= 10)
//            sprintf(strBuffer, "%01d.%03d ", currentData.S2PPM / 1000, currentData.S2PPM % 1000);
        else
            sprintf(strBuffer, "%4d.%01d", (currentData.S2PPM) / 10, (currentData.S2PPM) % 10);
//            sprintf(strBuffer, "%2d.%04d", (currentData.S2PPM*10+imsi) / 10000, (currentData.S2PPM*10+imsi) % 10000);


		display_2ch_ec();
        Draw_2CH_HOME_Number(255, L_NUMBER1_Y+20, strBuffer);
		display_2ch_unit_ec();

        //DrawTextETC(363, 170, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
        //sprintf(strBuffer, "%02d.%02d", currentData.temperature/100, currentData.temperature%100);
        //DrawMediumNumber(235, 170, strBuffer, BROWN);


		if (Relay1_run_on_flag == 0 && Relay3_run_on_flag == 0){
	        // ùùù ?ùù, ùùù ùùù
	//        Draw_Back_Oval_100x30(310, 152);

	//        if ((currentData.temperature) >= 9999) sprintf(strBuffer, "99.9C");	
	//        else if ((currentData.temperature) <= -9999) sprintf(strBuffer, "-99.9C");	
	//        else sprintf(strBuffer, "%02d.%01dC", currentData.temperature / 100, currentData.temperature % 100); // C is 'C

	        if ((currentData.temperature1) >= 9999) sprintf(strBuffer, " 99.9C ");	
	        else if ((currentData.temperature1) <= -9999) sprintf(strBuffer, "-99.9C ");	
	        else if ((currentData.temperature1) >= 0) 	sprintf(strBuffer, " %02d.%01dC ",      currentData.temperature1 / 10, currentData.temperature1 % 10); // C is 'C
	        else 										sprintf(strBuffer, "-%02d.%01dC ", (abs(currentData.temperature1)) / 10, (abs(currentData.temperature1)) % 10); // C is 'C

	//        else if ((currentData.temperature1) >= 0) 	sprintf(strBuffer, " %02d.%01dC ",      currentData.temperature1 / 100, (currentData.temperature1/10) % 10); // C is 'C
	//        else 										sprintf(strBuffer, "-%02d.%01dC ", (abs(currentData.temperature1)) / 100, ((abs(currentData.temperature1))/10) % 10); // C is 'C

		TFT_Fill(320, 154, 330+74, 154+5, BACK_COLOR3);
		TFT_Fill(320, 175, 330+74, 175+5, BACK_COLOR3);
	        Draw_2CH_TEMP_Number(320, 159, strBuffer, DRAW_NORMAL);
		}
//    }

    /* EXT sensor overlay: Draw_2CH_HOME_Number reused at same coords as S1/S2PPM.
     * raw[0] used directly (scaled_value is int32_t; fractional part would be lost).
     *   sensors[0] pH_EXT  scale=0.01 : raw 700  -> " 7.00"
     *   sensors[1] EC_EXT  scale=1.0  : raw 1234 -> "123.4"
     * Show "--.-  " while status != SMGR_STATUS_OK (timeout / CRC error / pending).
     * DRAW_NORMAL keeps original NAND bitmap colors identical to existing S1/S2PPM. */
    {
        uint16_t ext_raw;

        if (g_ext_sensor_count >= 1) {
            if (g_ext_sensors[0].status == SMGR_STATUS_OK) {
                ext_raw = g_ext_sensors[0].raw[0];
                sprintf(strBuffer, "%2d.%02d", ext_raw / 100, ext_raw % 100);
            } else {
                sprintf(strBuffer, "--.-  ");
            }
            Draw_2CH_HOME_Number(80, L_NUMBER1_Y + 20, strBuffer, DRAW_NORMAL);
        }

        if (g_ext_sensor_count >= 2) {
            if (g_ext_sensors[1].status == SMGR_STATUS_OK) {
                ext_raw = g_ext_sensors[1].raw[0];
                sprintf(strBuffer, "%4d.%01d", ext_raw / 10, ext_raw % 10);
            } else {
                sprintf(strBuffer, "---.-  ");
            }
            Draw_2CH_HOME_Number(255, L_NUMBER1_Y + 20, strBuffer, DRAW_NORMAL);
        }
    }
}

#endif


void RedrawMainView(void) {
    ClearViewArea();

//    if (currentData.Device_Selector_Mode & SENSOR_1_MODE)
//        DrawTextETC(MGL_X1, 105, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);

//    if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
//        DrawTextETC(MGL_X1, 125, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
//    }
    RedrawMainValue();
}

