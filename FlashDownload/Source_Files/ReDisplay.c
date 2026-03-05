#include "ReDisplay.h"

const char* comModeText[2] = {"232", "485"}; 
const char* comDatabitText[2] = {"8", "9"}; 
const char* comBaudrateText[4] = {"  9600", " 19200", " 38400","115200"};
int32_t tempIntVal = 0;
int32_t zerointVal = 0;
int32_t offsetintVal = 0;

void RedrawValue(void)
{
	switch(state)
	{
	case STATE_MAIN:
		RedrawMainValue();
	break;
	case STATE_MENU:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
		
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{		
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CONFIG:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CALIB:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
	sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_TREND:
		if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
		{
			if(compareValue == 0)
			{
			//	DrawTextsize96(20, 50, TEXT96_SENSOR1_YELLOW, DRAW_IMAGE_ENABLE);
			//	DrawTextsize96(120, 50, TEXT96_SENSOR2_WHITE, DRAW_IMAGE_ENABLE);
			}
			else
			{
			//	DrawTextsize96(20, 50, TEXT96_SENSOR1_WHITE, DRAW_IMAGE_ENABLE);
				DrawTextsize96(20, 50, TEXT96_CI, DRAW_IMAGE_ENABLE);
				DrawTextsize96(120, 50, TEXT96_SENSOR2_YELLOW, DRAW_IMAGE_ENABLE);
			}
		}
		DrawGraphText(cursor);		
		DrawGraph(cursor);
		TFT_Fill(GRAPH_X_START + (cursor*6), 89, GRAPH_X_START + (cursor*6) + 1, 270, RED);
	break;
	case STATE_ALARM:
	break;
	case STATE_DIAGNOSTIC:
	break;
	
	case STATE_CONFIG_OUTPUT:
	break;
	case STATE_CONFIG_COMM:
	break;
	case STATE_CONFIG_RELAY:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CONFIG_TIME:
	break;
	case STATE_CONFIG_ADJUST:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
	sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CALIB_ZERO:
	break;
	case STATE_CALIB_AUTO:
	break;
	case STATE_CALIB_MANUAL:
	break;
	case STATE_CALIB_S2_CYCLE:
	break;
	case STATE_CALIB_LOG:
		DrawCalibLog(cursor);
	break;
	
	case STATE_CONFIG_RELAY_RELAY1:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CONFIG_RELAY_RELAY2:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CONFIG_RELAY_RELAY3:
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
	}
	break;
	case STATE_CONFIG_ADJUST_GRADIENT:
	break;
	case STATE_CONFIG_ADJUST_OFFSET:
	break;
	case STATE_CONFIG_ADJUST_FILTER:
	break;
	case STATE_CONFIG_ADJUST_FACTORYRESET:
	break;
	
	case STATE_CONFIG_RELAY_RELAY1_WASH:
	break;
	case STATE_CONFIG_RELAY_RELAY1_ALARM:
	break;
	case STATE_CONFIG_RELAY_RELAY2_WASH:
	break;
	case STATE_CONFIG_RELAY_RELAY2_ALARM:
	break;
	case STATE_CONFIG_RELAY_RELAY3_WASH:
	break;
	case STATE_CONFIG_RELAY_RELAY3_ALARM:
	break;	
	}
}

void RedrawTitle(void)   
{
	switch(state)
	{
	case STATE_MAIN:
		ClearTitle();
	break;
	case STATE_MENU:
		ClearTitle();
	break;
	case STATE_CONFIG:
		ClearTitle();
	break;
	case STATE_CALIB:
		ClearTitle();
	break;
	case STATE_TREND: 
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_TREND, DRAW_IMAGE_ENABLE);
	break;
	case STATE_ALARM:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_ALARM, DRAW_IMAGE_ENABLE);
	break;
	case STATE_DIAGNOSTIC:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
	break;
	
	case STATE_CONFIG_OUTPUT:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_ANALOG, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_COMM:
		ClearTitle();
		DrawTextsize55(172, 5, TEXT55_COMMUNICATION, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_RELAY:
		ClearTitle();
	break;
	case STATE_CONFIG_TIME:
		ClearTitle();
		DrawTextsize55(172, 5, TEXT55_TIME, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST:
		ClearTitle();
	break;
	case STATE_CALIB_ZERO:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_ZERO_CALIBRATION, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CALIB_MANUAL:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_MANUAL_CALIBRATION, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CALIB_AUTO:
		ClearTitle();
	  if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
		{DrawTextsize96(153, 5, TEXT96_AUTO_CALIBRATION, DRAW_IMAGE_ENABLE); }
		else 
		DrawTextsize96(153, 5, TEXT96_TEMP_CALIBRATION, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CALIB_S2_CYCLE:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_SENSOR2_CYCLE, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CALIB_LOG:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_CALIBRATION_LOG, DRAW_IMAGE_ENABLE);
	break;
	
	case STATE_CONFIG_RELAY_RELAY1:
		ClearTitle();
	break;
	case STATE_CONFIG_RELAY_RELAY2:
		ClearTitle();
	break;
	case STATE_CONFIG_RELAY_RELAY3:
		ClearTitle();
	break;
	case STATE_CONFIG_ADJUST_GRADIENT:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_GRADIENT, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST_OFFSET:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_OFFSET, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST_FILTER:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_FILTER, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST_FACTORYRESET:
		ClearTitle();
		DrawTextsize96(153, 5, TEXT96_FACTORY_RESET, DRAW_IMAGE_ENABLE);
	break;
	
	case STATE_CONFIG_RELAY_RELAY1_WASH:
		ClearTitle();
	  DrawTextsize96(172, 5, TEXT96_AUTO_WASHING, DRAW_IMAGE_ENABLE);  
	break;
	case STATE_CONFIG_RELAY_RELAY1_ALARM:
		ClearTitle();
	  DrawTextsize96(172, 5, TEXT96_MANUAL_WASH, DRAW_IMAGE_ENABLE);   
	break;
	case STATE_CONFIG_RELAY_RELAY2_WASH:
		ClearTitle();
	  DrawTextsize96(172, 5, TEXT96_AUTO_ALARM, DRAW_IMAGE_ENABLE);     
	break;
	case STATE_CONFIG_RELAY_RELAY2_ALARM:
		ClearTitle();
	   DrawTextsize96(172, 5, TEXT96_ALARM_OFF, DRAW_IMAGE_ENABLE);         
	break;
	case STATE_CONFIG_RELAY_RELAY3_WASH:
		ClearTitle();
	   DrawTextsize96(172, 5, TEXT96_SET_OPERATION, DRAW_IMAGE_ENABLE); 
	break;
	case STATE_CONFIG_RELAY_RELAY3_ALARM:
		ClearTitle();
	   DrawTextsize96(172, 5, TEXT96_MANUAL_OPERATION, DRAW_IMAGE_ENABLE); 
	break;	
	}
}


void RedrawViewArea(void)  
{
	
	switch(state)
	{
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
		ClearViewArea();
		DrawLineRectangle(58, 87, 60, 272, BROWN); 
		DrawLineRectangle(48, 87, 432, 89, BROWN); 
		DrawLineRectangle(421, 87, 423, 272, BROWN); 
		DrawLineRectangle(48, 270, 432, 272, BROWN); 
	
		DrawLineRectangle(48, 179, 58, 180, BROWN); 
		DrawLineRectangle(421, 179, 432, 180, BROWN); 
	
   if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
	    DrawTextETC(360, 50, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);	
	 }
	 else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	 {
	    DrawTextETC(370, 50, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
	 }
	break;
	case STATE_ALARM:
		ClearViewArea();
    DrawTextsize120(50, 90, TEXT120_LIMIT_HIGH, DRAW_IMAGE_ENABLE);
		DrawTextsize120(50, 140, TEXT120_LIMIT_LOW, DRAW_IMAGE_ENABLE);
		DrawTextsize120(50, 190, TEXT120_ZERO_ALARM, DRAW_IMAGE_ENABLE);
	
	  if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
		DrawTextETC(360, 90, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
		DrawTextETC(360, 140, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
		DrawUnit(360, 190, UNIT_MIN, DRAW_IMAGE_ENABLE);
		}
		else if (currentData.Device_Selector_Mode&SENSOR_2_MODE)
		{
      DrawTextETC(360, 90, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
	  	DrawTextETC(360, 140, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
	  	DrawUnit(360, 190, UNIT_MIN, DRAW_IMAGE_ENABLE);
    }
		if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{
		sprintf(strBuffer, "%d.%d", tempConfigData.alarmConfig.highLimit/1000, (tempConfigData.alarmConfig.highLimit/100)%10);
		DrawMediumNumber(200, 90, strBuffer, YELLOW);
			
		DrawLineRectangle(180, 83, 330, 123, BROWN);		
			
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit%100);
		DrawMediumNumber(200+(22*3), 90, strBuffer, BROWN);
			
		sprintf(strBuffer, "%d.%03d", tempConfigData.alarmConfig.lowLimit/1000, tempConfigData.alarmConfig.lowLimit%1000);
		DrawMediumNumber(200, 140, strBuffer, BROWN);		
			
		DrawLineRectangle(180, 133, 330, 173, BROWN);
		}
		else if (currentData.Device_Selector_Mode&SENSOR_2_MODE)
		{
			sprintf(strBuffer, "%02d.%d", tempConfigData.alarmConfig.highLimit/1000, (tempConfigData.alarmConfig.highLimit/100)%10);
		DrawMediumNumber(200, 90, strBuffer, YELLOW);
		DrawLineRectangle(180, 83, 350, 123, BROWN);
		
		sprintf(strBuffer, "%03d", tempConfigData.alarmConfig.highLimit%100);
		DrawMediumNumber(200+(22*3), 90, strBuffer, BROWN);
			
		sprintf(strBuffer, "%02d.%03d", tempConfigData.alarmConfig.lowLimit/1000, tempConfigData.alarmConfig.lowLimit%1000);
		DrawMediumNumber(200, 140, strBuffer, BROWN);		
		DrawLineRectangle(180, 133, 350, 173, BROWN);
		}
		
		sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm);
		DrawMediumNumber(200+22, 190, strBuffer, BROWN);
		DrawLineRectangle(180, 183, 350, 223, BROWN);
		
	break;
	case STATE_DIAGNOSTIC: 
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
		{
		//	DrawTextsize96(30, 70, TEXT96_SENSOR1_WHITE, DRAW_IMAGE_ENABLE);
			DrawTextsize96(150, 70, TEXT96_CI, DRAW_IMAGE_ENABLE); 
			DrawTextsize96(246, 70, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
		//	DrawTextsize96(180, 205, 0, DRAW_IMAGE_ENABLE); 
			sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
			DrawMediumNumber(150, 115, strBuffer, BROWN);
			
			DrawTextETC(260, 110, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
			
			sprintf(strBuffer,"%2d.%02d", currentData.S1mA/100, currentData.S1mA%100);
			DrawMediumNumber(150, 160, strBuffer, BROWN);
			DrawTextETC(260, 160, TEXT_ETC_MA, DRAW_IMAGE_ENABLE);
		}	
		else if( currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{
			    DrawTextsize96(150, 70, TEXT96_CI2, DRAW_IMAGE_ENABLE); DrawTextsize96(246, 70, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
			    sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
			    DrawMediumNumber(150, 115, strBuffer, BROWN);
		    	DrawTextETC(285, 115, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
			
			    sprintf(strBuffer,"%3d.%01d", currentData.S2mV/10, currentData.S2mV%10);
			    DrawMediumNumber(150, 160, strBuffer, BROWN);
			    DrawTextETC(260, 160, TEXT_ETC_MV, DRAW_IMAGE_ENABLE);
			
		}
	break;
	
	case STATE_CONFIG_OUTPUT:
		ClearViewArea();
		 if(currentData.Device_Selector_Mode == SENSOR_1_MODE ) 	{ 
			 DrawTextETC(370, 70, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
			 DrawTextETC(370, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
		 }
		 else if(currentData.Device_Selector_Mode == SENSOR_2_MODE) {
		 	DrawTextETC(370, 70, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
			DrawTextETC(370, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
     }
	  DrawTextsize96(100, 70, TEXT96_4MA, DRAW_IMAGE_ENABLE);	  
		sprintf(strBuffer, "%d.%d", configData.outputConfig.output4mA/1000,(configData.outputConfig.output4mA/100)%10);		
		DrawMediumNumber(230, 70, strBuffer, YELLOW);
		sprintf(strBuffer, "%02d",configData.outputConfig.output4mA%100);
		DrawMediumNumber(230+66, 70, strBuffer, BROWN);
	
		DrawLineRectangle(210, 63, 370, 103, BROWN);
	
		DrawTextsize96(100, 120, TEXT96_20MA, DRAW_IMAGE_ENABLE);
	  if(currentData.Device_Selector_Mode == SENSOR_1_MODE ) 	{ sprintf(strBuffer, "%1d.%03d", configData.outputConfig.output20mA/1000,configData.outputConfig.output20mA%1000);	}
    else if( currentData.Device_Selector_Mode == SENSOR_2_MODE) { sprintf(strBuffer, "%02d.%03d", configData.outputConfig.output20mA/1000,configData.outputConfig.output20mA%1000);	} 		
		DrawMediumNumber(230, 120, strBuffer, BROWN);
	
		DrawLineRectangle(210, 113, 370, 153, BROWN);
	
		DrawTextsize96(100, 170, TEXT96_4MA, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%05d", tempConfigData.outputConfig.outputReal4mA);		DrawMediumNumber(230, 170, strBuffer, BROWN);
		DrawLineRectangle(210, 163, 370, 203, BROWN);
		DrawTextsize96(100, 220, TEXT96_20MA, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%05d", tempConfigData.outputConfig.outputReal20mA);		DrawMediumNumber(230, 220, strBuffer, BROWN);
		DrawLineRectangle(210, 213, 370, 253, BROWN);
	
		if(currentData.holdState)
			DrawTextETC(10, 220, TEXT_ETC_HOLD_W, DRAW_IMAGE_ENABLE);		  
		else
			DrawTextETC(10, 220, TEXT_ETC_RUN_W, DRAW_IMAGE_ENABLE);
		
	break;
	case STATE_CONFIG_COMM:
		ClearViewArea();
		DrawTextsize120(70, 70, TEXT120_COM_METHOD, DRAW_IMAGE_ENABLE);
		DrawMediumNumber(230+66, 70, (char*)comModeText[tempConfigData.modbusConfig.mode], YELLOW);
		DrawLineRectangle(210, 63, 382, 103, BROWN);
		DrawTextsize120(70, 120, TEXT120_COM_BAUD, DRAW_IMAGE_ENABLE);
		DrawMediumNumber(230, 120, (char*)comBaudrateText[tempConfigData.modbusConfig.baudrate], BROWN);
		DrawLineRectangle(210, 113, 382, 153, BROWN);
		DrawTextsize120(70, 170, TEXT120_MODBUSID, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%3d", tempConfigData.modbusConfig.modbusSlaveAddr+1);		DrawMediumNumber(230+66, 170, strBuffer, BROWN);
		DrawLineRectangle(210, 163, 382, 203, BROWN);
		DrawTextsize120(70, 220, TEXT120_COM_DATABIT, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%d", tempConfigData.modbusConfig.databit+8);		DrawMediumNumber(230+110, 220, strBuffer, BROWN);
		DrawLineRectangle(210, 213, 382, 253, BROWN);
	break;
	case STATE_CONFIG_RELAY:
	break;
	case STATE_CONFIG_TIME:
		ClearViewArea();
		DrawUnit(150, 100, UNIT_YEAR, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%4d", tempConfigTime.tm_year);		DrawMediumNumber(49, 100, strBuffer, YELLOW);
		DrawLineRectangle(39, 93, 144, 133, BROWN);
		DrawUnit(275, 100, UNIT_MONTH, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%2d", tempConfigTime.tm_mon+1);		DrawMediumNumber(219, 100, strBuffer, BROWN);
		DrawLineRectangle(209, 93, 270, 133, BROWN);
		DrawUnit(400, 100, UNIT_DAY, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%2d", tempConfigTime.tm_mday);		DrawMediumNumber(343, 100, strBuffer, BROWN);
		DrawLineRectangle(336, 93, 394, 133, BROWN);
	
		DrawUnit(150, 200, UNIT_HOUR, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%2d", tempConfigTime.tm_hour);		DrawMediumNumber(93, 200, strBuffer, BROWN);
		DrawLineRectangle(83, 193, 144, 233, BROWN);
		DrawUnit(275, 200, UNIT_MIN, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%2d", tempConfigTime.tm_min);		DrawMediumNumber(219, 200, strBuffer, BROWN);
		DrawLineRectangle(209, 193, 270, 233, BROWN);
		DrawUnit(400, 200, UNIT_SEC, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%2d", tempConfigTime.tm_sec);		DrawMediumNumber(343, 200, strBuffer, BROWN);
		DrawLineRectangle(336, 193, 394, 233, BROWN);
	break;
	case STATE_CONFIG_ADJUST:
	break;
	case STATE_CALIB_ZERO:  
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE )  
		{
			zerointVal = tempConfigData.calibrationConfig.S1zeroCal;
			DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
		
			 DrawTextETC(402, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
			
			if(zerointVal < 0 )
			{
          zerointVal = zerointVal * (-1);
      }
			if(tempConfigData.calibrationConfig.S1zeroCal < 0 )
			{
          sprintf(strBuffer, "-%d.%d", zerointVal/100, (zerointVal/10)%10);		
				  DrawMediumNumber(270, 120, strBuffer, YELLOW);
          sprintf(strBuffer, "%d",zerointVal%10);
		      DrawMediumNumber(270+(22*4), 120, strBuffer, BROWN) ;    
      }
		
			else 
			{
           sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal/100, (tempConfigData.calibrationConfig.S1zeroCal/10)%10);				
		     	 DrawMediumNumber(270, 120, strBuffer, YELLOW);
			     sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal%10);
			     DrawMediumNumber(270+(22*4), 120, strBuffer, BROWN);
       }
			
			DrawLineRectangle(240, 113, 400, 154, BROWN);
		}		
		
	   else if ( currentData.Device_Selector_Mode == SENSOR_2_MODE)  
			{
            zerointVal = tempConfigData.calibrationConfig.S2zeroCal;
		      	DrawTextsize180(50, 120, TEXT180_S1_ZERO_CAL, DRAW_IMAGE_ENABLE);
		
			      DrawTextETC(402, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
			
			if(zerointVal < 0 )
			{
          zerointVal = zerointVal * (-1);
      }
			if(tempConfigData.calibrationConfig.S2zeroCal < 0 )
			{
          sprintf(strBuffer, "-%d.%d", zerointVal/1000, (zerointVal/100)%10);		
				  DrawMediumNumber(248, 120, strBuffer, YELLOW);
          sprintf(strBuffer, "%02d",zerointVal%100);
		      DrawMediumNumber(248+(22*4), 120, strBuffer, BROWN);
      }
		
			else 
			{
           sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal/1000, (tempConfigData.calibrationConfig.S2zeroCal/100)%10);				
		     	 DrawMediumNumber(248, 120, strBuffer, YELLOW);
			     sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal%100);
			     DrawMediumNumber(248+(22*4), 120, strBuffer, BROWN);
       }
			
			DrawLineRectangle(230, 113, 400, 154, BROWN);
      }
	break;
	case STATE_CALIB_MANUAL:  
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE )   
		{
			DrawTextsize180(50, 120, TEXT180_S1_MANUAL_CAL, DRAW_IMAGE_ENABLE);
			
		  DrawTextETC(400, 120, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
			
			sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1manualCal/100, (tempConfigData.calibrationConfig.S1manualCal/10)%10);	
			DrawMediumNumber(270, 120, strBuffer, YELLOW);
			sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal%10);
			DrawMediumNumber(270+(22*3), 120, strBuffer, BROWN);
			DrawLineRectangle(240, 113, 390, 154, BROWN);
			
		}		
		else if (currentData.Device_Selector_Mode == SENSOR_2_MODE)   
		{	
       DrawTextsize180(50, 120, TEXT180_S1_MANUAL_CAL, DRAW_IMAGE_ENABLE);
			
		  DrawTextETC(400, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
			
			sprintf(strBuffer, "%02d.%d", tempConfigData.calibrationConfig.S2manualCal/1000, (tempConfigData.calibrationConfig.S2manualCal/100)%10);	
			DrawMediumNumber(255, 120, strBuffer, YELLOW);
			sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2manualCal%100);
			DrawMediumNumber(255+(22*4), 120, strBuffer, BROWN);
			DrawLineRectangle(240, 113, 390, 154, BROWN);			
		}
		
	break;
	case STATE_CALIB_AUTO:   
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{			
			
			if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	{ 
				
				DrawTextsize180(50, 120, TEXT180_S1_TEMP_CAL, DRAW_IMAGE_ENABLE);
		    DrawTextETC(400, 120, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
		
				tempIntVal = compareSignValue;
		    if(tempIntVal < 0)
			  tempIntVal =  tempIntVal * (-1);
      if(compareSignValue < 0)
				sprintf(strBuffer, "-%02d.%02d", tempIntVal/100, tempIntVal%100);
			else
				sprintf(strBuffer, "%02d.%02d", compareSignValue/100, compareSignValue%100);			  
			  DrawMediumNumber(260, 120, strBuffer, YELLOW);
				sprintf(strBuffer, "%02d", compareSignValue%100);
		    DrawMediumNumber(260+(22*3), 120, strBuffer, BROWN);
		     DrawLineRectangle(240, 113, 390, 154, BROWN);
				
       }
	    else if(currentData.Device_Selector_Mode&SENSOR_2_MODE) { 
      DrawTextsize180(50, 120, TEXT180_S1_AUTO_CAL_VAULE, DRAW_IMAGE_ENABLE);
      DrawTextETC(402, 120, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
				
			sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1Value/100, (tempConfigData.calibrationConfig.S1Value/10)%10);	
			DrawMediumNumber(270, 120, strBuffer, YELLOW);
			sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1Value%10);
			DrawMediumNumber(270+(22*3), 120, strBuffer, BROWN);
			DrawLineRectangle(240, 113, 390, 154, BROWN);

       }						
		}
		
		else
		{

		}
	break;
	case STATE_CALIB_S2_CYCLE:
		ClearViewArea();
		//DrawTextsize180(50, 100, TEXT180_S2_PERIOD, DRAW_IMAGE_ENABLE);
		sprintf(strBuffer, "%3d", tempConfigData.calibrationConfig.S2cycle);	
		DrawMediumNumber(270, 100, strBuffer, YELLOW);
		DrawLineRectangle(250, 93, 356, 133, BROWN);
	break;
	case STATE_CALIB_LOG:
		ClearViewArea();
	
		TFT_Fill(5, 102, 475, 104, BROWN);
		TFT_Fill(5, 152, 475, 154, BROWN);
		TFT_Fill(5, 202, 475, 204, BROWN);
		TFT_Fill(5, 252, 475, 254, BROWN);
	break;
	
	case STATE_CONFIG_RELAY_RELAY1:
	break;
	case STATE_CONFIG_RELAY_RELAY2:
	break;
	case STATE_CONFIG_RELAY_RELAY3:
	break;
	case STATE_CONFIG_ADJUST_GRADIENT:  
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
		{
			DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS1/100, tempConfigData.adjustConfig.gradientS1%100);
			DrawMediumNumber(250, 165, strBuffer, YELLOW);
			DrawLineRectangle(230, 152, 358, 207, BROWN);
		}
		else if (currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{
			DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS1/100, tempConfigData.adjustConfig.gradientS1%100);
			DrawMediumNumber(250, 165, strBuffer, YELLOW);
			DrawLineRectangle(230, 152, 358, 207, BROWN);
		}		
		DrawTextsize96(90, 165, TEXT96_GRADIENT, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST_OFFSET:  
		ClearViewArea();
	  offsetintVal = tempConfigData.adjustConfig.offsetS1;
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
		{
			DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
			
			if(offsetintVal < 0){
       offsetintVal = offsetintVal * (-1);
       }
	  	if( tempConfigData.adjustConfig.offsetS1 < 0)
		  { 
		    sprintf(strBuffer, "-%d.%02d", offsetintVal/100, offsetintVal%100);
		    DrawMediumNumber(250, 165, strBuffer, YELLOW);
  	  }
	  	else {
        sprintf(strBuffer, " %d.%02d", offsetintVal/100, offsetintVal%100);
		    DrawMediumNumber(250, 165, strBuffer, YELLOW);
      }
			  DrawLineRectangle(230, 152, 368, 207, BROWN);
		}		
		else if (currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{
			DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
			if(offsetintVal < 0){
       offsetintVal = offsetintVal * (-1);
       }
	  	if( tempConfigData.adjustConfig.offsetS1 < 0)
		  { 
		    sprintf(strBuffer, "-%d.%02d", offsetintVal/100, offsetintVal%100);
		    DrawMediumNumber(250, 165, strBuffer, YELLOW);
  	  }
	  	else {
        sprintf(strBuffer, "%d.%02d", offsetintVal/100, offsetintVal%100);
		    DrawMediumNumber(250, 165, strBuffer, YELLOW);
      }
			DrawLineRectangle(220, 152, 358, 207, BROWN);
		}		
		DrawTextsize96(90, 165, TEXT96_OFFSET, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST_FILTER:  
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
		{
			DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS1);
			DrawMediumNumber(250, 165, strBuffer, YELLOW);
			DrawLineRectangle(230, 152, 336, 207, BROWN);
		}		
		else if(currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{
			DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS1);
			DrawMediumNumber(250, 165, strBuffer, YELLOW);
			DrawLineRectangle(230, 152, 336, 207, BROWN);
		}
		
		DrawTextsize96(90, 165, TEXT96_FILTER, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_ADJUST_FACTORYRESET:
		ClearViewArea();
		DrawTextETC(115, 130, TEXT_ETC_F_RESET_MESSAGE, DRAW_IMAGE_ENABLE);
	break;
	
	case STATE_CONFIG_RELAY_RELAY1_WASH:  
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{
			DrawTextsize55(90, 100, TEXT55_WASH, DRAW_IMAGE_ENABLE);	DrawTextsize55(150, 100, TEXT55_CYCLE, DRAW_IMAGE_ENABLE);	DrawUnit(400, 100, UNIT_MIN, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashCycle);		
			DrawMediumNumber(250, 100, strBuffer, YELLOW);
			DrawLineRectangle(230, 93, 336, 133, BROWN);
			DrawTextsize55(90, 150, TEXT55_WASH, DRAW_IMAGE_ENABLE);	DrawTextsize55(150, 150, TEXT55_TIME, DRAW_IMAGE_ENABLE);DrawUnit(400, 150, UNIT_MIN, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashTime);		
			DrawMediumNumber(250, 150, strBuffer, BROWN);
			DrawLineRectangle(230, 143, 336, 183, BROWN);
		}		
		else   
		{
		}
	break;
	case STATE_CONFIG_RELAY_RELAY1_ALARM:  
		ClearViewArea();
		if(tempConfigData.relayConfig.relay1manual)			
		     DrawTextETC(200, 130, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
		else
			   DrawTextETC(200, 130, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_RELAY_RELAY2_WASH:  
		ClearViewArea();
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
		{
			if(tempConfigData.relayConfig.relay2AutoAlarm)
			DrawTextsize55(200, 130, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
		  else
			DrawTextsize55(200, 130, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
					}
		
		else
		{
		}
	break;
	case STATE_CONFIG_RELAY_RELAY2_ALARM:
		ClearViewArea();
		if(tempConfigData.relayConfig.relay2AlarmOFF)
			DrawTextsize55(200, 130, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
		else
			DrawTextsize55(200, 130, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
	break;
	case STATE_CONFIG_RELAY_RELAY3_WASH:
		ClearViewArea();
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)  // // TEXT96_CONFIG DrawTextsize96(90, 200, TEXT96_FILTER, DRAW_IMAGE_ENABLE);
		{
			DrawTextsize96(70, 100, TEXT96_CONFIG, DRAW_IMAGE_ENABLE);	DrawTextsize55(150, 100, TEXT55_CYCLE, DRAW_IMAGE_ENABLE);DrawUnit(350, 100, UNIT_MIN, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashCycle);		
			DrawMediumNumber(250, 100, strBuffer, YELLOW);
			DrawLineRectangle(230, 93, 336, 133, BROWN);
			DrawTextsize96(70, 150, TEXT96_CONFIG, DRAW_IMAGE_ENABLE);	DrawTextsize55(150, 150, TEXT55_TIME, DRAW_IMAGE_ENABLE);DrawUnit(350, 150, UNIT_MIN, DRAW_IMAGE_ENABLE);
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashTime);		
			DrawMediumNumber(250, 150, strBuffer, BROWN);
			DrawLineRectangle(230, 143, 336, 183, BROWN);
		}		
		else
		{
		}
	break;
	case STATE_CONFIG_RELAY_RELAY3_ALARM:
		ClearViewArea();
		if(tempConfigData.relayConfig.relay3manual)
			 DrawTextETC(200, 130, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
		else
			 DrawTextETC(200, 130, TEXT_ETC_STOP_Y, DRAW_IMAGE_ENABLE);
	break;	
	}
}
void RedrawBottomArea(void)   
{
	switch(state)
	{
	case STATE_MAIN:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
		DrawTextETC(425, 237, TEXT_ETC_MA, DRAW_IMAGE_ENABLE);
		DrawBottomLine();
		RedrawTime();
	break;
	case STATE_MENU:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
		DrawTextsize96(0, 240, TEXT96_CONFIG, DRAW_IMAGE_ENABLE);
		DrawTextsize96(96, 240, TEXT96_CALIBRATION, DRAW_IMAGE_ENABLE);
		DrawTextsize96(192, 240, TEXT96_TREND, DRAW_IMAGE_ENABLE);
		DrawTextsize96(288, 240, TEXT96_ALARM, DRAW_IMAGE_ENABLE);
		DrawTextsize96(384, 240, TEXT96_DIAGNOSIS, DRAW_IMAGE_ENABLE);
		DrawLineRectangle(0+(96*0), 238, 96+(0*96), 272, YELLOW);
	break;
	case STATE_CONFIG:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
		DrawTextsize96(0, 240, TEXT96_ANALOG, DRAW_IMAGE_ENABLE);
		DrawTextsize55(116, 240, TEXT55_COMMUNICATION, DRAW_IMAGE_ENABLE);
		DrawTextsize96(192, 240, TEXT96_RELAY, DRAW_IMAGE_ENABLE);
		DrawTextsize96(288, 240, TEXT96_TIME, DRAW_IMAGE_ENABLE);
		DrawTextsize96(384, 240, TEXT96_ADJUST, DRAW_IMAGE_ENABLE);
		DrawLineRectangle(0+(96*0), 238, 96+(0*96), 272, YELLOW);
	break;
	case STATE_CALIB:
		ClearBottomArea();
		if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
		{
			TFT_Fill(0, 234, 480, 236, BROWN);
			DrawTextsize96(12, 240, TEXT96_ZERO_CALIBRATION, DRAW_IMAGE_ENABLE);
			DrawTextsize96(132, 240, TEXT96_MANUAL_CALIBRATION, DRAW_IMAGE_ENABLE);
			DrawTextsize96(252, 240, TEXT96_AUTO_CALIBRATION, DRAW_IMAGE_ENABLE);
			DrawTextsize96(372, 240, TEXT96_CALIBRATION_LOG, DRAW_IMAGE_ENABLE);
			DrawLineRectangle(0+(120*0), 238, 120+(0*120), 272, YELLOW);
		}
		else
		{
			TFT_Fill(0, 234, 480, 236, BROWN);
			DrawTextsize96(12, 240, TEXT96_ZERO_CALIBRATION, DRAW_IMAGE_ENABLE);
			DrawTextsize96(132, 240, TEXT96_MANUAL_CALIBRATION, DRAW_IMAGE_ENABLE);
			DrawTextsize96(252, 240, TEXT96_TEMP_CALIBRATION, DRAW_IMAGE_ENABLE);
			DrawTextsize96(372, 240, TEXT96_CALIBRATION_LOG, DRAW_IMAGE_ENABLE);
			DrawLineRectangle(0+(120*0), 238, 120+(0*120), 272, YELLOW);
		}
	break;
	case STATE_TREND:
		ClearBottomArea();
	break;
	case STATE_ALARM:
		ClearBottomArea();
	break;
	case STATE_DIAGNOSTIC:
		ClearBottomArea();
	break;
	
	case STATE_CONFIG_OUTPUT:
		ClearBottomArea();
	break;
	case STATE_CONFIG_COMM:
		ClearBottomArea();
	break;
	case STATE_CONFIG_RELAY:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
	  DrawTextsize55(50, 240, TEXT55_WASH, DRAW_IMAGE_ENABLE);
	  DrawTextsize55(210, 240, TEXT55_ALARM, DRAW_IMAGE_ENABLE);
		DrawTextsize96(352, 240, TEXT96_RELAY, DRAW_IMAGE_ENABLE);	
		DrawLineRectangle(0+(160*0), 238, 160+(0*160), 272, YELLOW);
	break;
	case STATE_CONFIG_TIME:
		ClearBottomArea();
	break;
	case STATE_CONFIG_ADJUST:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
		DrawTextsize96(12, 240, TEXT96_GRADIENT, DRAW_IMAGE_ENABLE);
		DrawTextsize96(132, 240, TEXT96_OFFSET, DRAW_IMAGE_ENABLE);
		DrawTextsize96(252, 240, TEXT96_FILTER, DRAW_IMAGE_ENABLE);
		DrawTextsize96(372, 240, TEXT96_FACTORY_RESET, DRAW_IMAGE_ENABLE);
		DrawLineRectangle(0+(120*0), 238, 120+(0*120), 272, YELLOW);
	break;
	case STATE_CALIB_ZERO:
		ClearBottomArea();
	break;
	case STATE_CALIB_AUTO:
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
	
	case STATE_CONFIG_RELAY_RELAY1:  
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
	   DrawTextsize96(75, 240, TEXT96_AUTO_WASHING, DRAW_IMAGE_ENABLE);		
	   DrawTextsize96(310, 240, TEXT96_MANUAL_WASH, DRAW_IMAGE_ENABLE);
	
			DrawLineRectangle(0+(240*0), 238, 240+(0*240), 272, YELLOW);
	break;
	case STATE_CONFIG_RELAY_RELAY2:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
	    DrawTextsize96(75, 240, TEXT96_AUTO_ALARM, DRAW_IMAGE_ENABLE);
	    DrawTextsize96(310, 240, TEXT96_ALARM_OFF, DRAW_IMAGE_ENABLE);
			DrawLineRectangle(0+(240*0), 238, 240+(0*240), 272, YELLOW);
	break;
	case STATE_CONFIG_RELAY_RELAY3:
		ClearBottomArea();
		TFT_Fill(0, 234, 480, 236, BROWN);
	    DrawTextsize96(75, 240, TEXT96_SET_OPERATION, DRAW_IMAGE_ENABLE);
	    DrawTextsize96(310, 240, TEXT96_MANUAL_OPERATION, DRAW_IMAGE_ENABLE);
			DrawLineRectangle(0+(240*0), 238, 240+(0*240), 272, YELLOW);
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
	
	case STATE_CONFIG_RELAY_RELAY1_WASH:
		ClearBottomArea();
	break;
	case STATE_CONFIG_RELAY_RELAY1_ALARM:
		ClearBottomArea();
	break;
	case STATE_CONFIG_RELAY_RELAY2_WASH:
		ClearBottomArea();
	break;
	case STATE_CONFIG_RELAY_RELAY2_ALARM:
		ClearBottomArea();
	break;
	case STATE_CONFIG_RELAY_RELAY3_WASH:
		ClearBottomArea();
	break;
	case STATE_CONFIG_RELAY_RELAY3_ALARM:
		ClearBottomArea();
	break;	
	}
}

void RedrawMainValue(void)
{
	DrawBottomCurrent(currentData.current4_20mA);
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
	{
		// 잔류염소 표시
		sprintf(strBuffer,"%1d.%02d", currentData.S1PPM/100, currentData.S1PPM%100);
		DrawLargeNumber(200, 90, strBuffer);
		
		// 온도 표시
		sprintf(strBuffer, "%02d.%02d ", currentData.temperature/100, currentData.temperature%100);
		DrawMediumNumber(235, 170, strBuffer, BROWN);

		// 온도기호 ('C) 표시
		DrawTextETC(363, 170, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
		
	}
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
		sprintf(strBuffer,"%02d.%03d", currentData.S2PPM/1000, currentData.S2PPM%1000);
		DrawLargeNumber(140, 110, strBuffer);
		
		//DrawTextETC(363, 170, TEXT_ETC_TEMPUNIT, DRAW_IMAGE_ENABLE);
		//sprintf(strBuffer, "%02d.%02d", currentData.temperature/100, currentData.temperature%100);
		//DrawMediumNumber(235, 170, strBuffer, BROWN);
	}
}

void RedrawMainView(void)
{
	ClearViewArea();
		
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE)	
	DrawTextETC(360, 105, TEXT_ETC_MGL, DRAW_IMAGE_ENABLE);
	
	if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
	{
	  DrawTextETC(360, 125, TEXT_ETC_NTU, DRAW_IMAGE_ENABLE);
	}
	RedrawMainValue();
}

