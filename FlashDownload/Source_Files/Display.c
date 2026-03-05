#include "Display.h"
 	
void DrawGraph(uint32_t cur)
{
	uint32_t i = 0;

	if(currentData.Device_Selector_Mode &SENSOR_1_MODE)
	  {
	    for(i = 1; i < 61 ; ++i)
    	{
		    TFT_Line(GRAPH_X_START + ((i-1)*6), GRAPH_Y_ZERO - ((trendValue[compareValue][i-1] * 180) / 200), 
		    GRAPH_X_START + (i*6), GRAPH_Y_ZERO - ((trendValue[compareValue][i] * 180) / 200), BLUE);
	    }
    }
		else if (currentData.Device_Selector_Mode &SENSOR_2_MODE)
		{
			 for(i = 1; i < 61 ; ++i)
    	{				
		    TFT_Line(GRAPH_X_START + ((i-1)*6), GRAPH_Y_ZERO - (((trendValue[compareValue][i-1]/100)* 180) / 200), 
		    GRAPH_X_START + (i*6), GRAPH_Y_ZERO - (((trendValue[compareValue][i]/100) * 180) / 200), BLUE);
	    }
		}
}

void DrawGraphText(uint32_t cur)
{
	   if(currentData.Device_Selector_Mode &SENSOR_1_MODE)
	    {
	       sprintf(strBuffer, "%1d.%02d", trendValue[compareValue][cursor]/100, trendValue[compareValue][cursor]%100);
	    }
	 	 else if (currentData.Device_Selector_Mode &SENSOR_2_MODE)
		 {
		    	sprintf(strBuffer, "%02d.%03d", trendValue[compareValue][cursor]/1000, trendValue[compareValue][cursor]%1000);
	   }			
	   DrawMediumNumber(240, 50, strBuffer, WHITE);
}

void ClearGraphArea(void)
{
	TFT_Fill(60, 89, 421, 270, BLACK);
}

void DrawCalibLog(uint32_t cur)
{
	int16_t Calibration_Trans_compare_Value = 0;
  int Calibration_Trans_vaule = 0;	
	int i = 0;
	
	sprintf(strBuffer, "%2d", cur+1);	DrawMediumNumber(5, 70, strBuffer, WHITE);
	sprintf(strBuffer, "%2d", cur+2);	DrawMediumNumber(5, 120, strBuffer, WHITE);
	sprintf(strBuffer, "%2d", cur+3);	DrawMediumNumber(5, 170, strBuffer, WHITE);
	sprintf(strBuffer, "%2d", cur+4);	DrawMediumNumber(5, 220, strBuffer, WHITE);
	sprintf(strBuffer, "%04d-%02d-%02d", calibLog[0].calibrationTime.tm_year, calibLog[0].calibrationTime.tm_mon+1, calibLog[0].calibrationTime.tm_mday);	DrawSmallNumber(75, 70-10, strBuffer);
	sprintf(strBuffer, "%02d:%02d:%02d", calibLog[0].calibrationTime.tm_hour, calibLog[0].calibrationTime.tm_min, calibLog[0].calibrationTime.tm_sec);	DrawSmallNumber(75+15, 70+10, strBuffer);
	sprintf(strBuffer, "%04d-%02d-%02d", calibLog[1].calibrationTime.tm_year, calibLog[1].calibrationTime.tm_mon+1, calibLog[1].calibrationTime.tm_mday);	DrawSmallNumber(75, 120-10, strBuffer);
	sprintf(strBuffer, "%02d:%02d:%02d", calibLog[1].calibrationTime.tm_hour, calibLog[1].calibrationTime.tm_min, calibLog[1].calibrationTime.tm_sec);	DrawSmallNumber(75+15, 120+10, strBuffer);
	sprintf(strBuffer, "%04d-%02d-%02d", calibLog[2].calibrationTime.tm_year, calibLog[2].calibrationTime.tm_mon+1, calibLog[2].calibrationTime.tm_mday);	DrawSmallNumber(75, 170-10, strBuffer);
	sprintf(strBuffer, "%02d:%02d:%02d", calibLog[2].calibrationTime.tm_hour, calibLog[2].calibrationTime.tm_min, calibLog[2].calibrationTime.tm_sec);	DrawSmallNumber(75+15, 170+10, strBuffer);
	sprintf(strBuffer, "%04d-%02d-%02d", calibLog[3].calibrationTime.tm_year, calibLog[3].calibrationTime.tm_mon+1, calibLog[3].calibrationTime.tm_mday);	DrawSmallNumber(75, 220-10, strBuffer);
	sprintf(strBuffer, "%02d:%02d:%02d", calibLog[3].calibrationTime.tm_hour, calibLog[3].calibrationTime.tm_min, calibLog[3].calibrationTime.tm_sec);	DrawSmallNumber(75+15, 220+10, strBuffer);

  for (i = 0 ; i< 4 ; i++)
	{
     Calibration_Trans_compare_Value =  0x80 & (int16_t)calibLog[i].calibrationValue;
	   if(Calibration_Trans_compare_Value != 0)
	  	Calibration_Trans_vaule = (int16_t)calibLog[i].calibrationValue *(-1);
		 
	  if(currentData.Device_Selector_Mode &SENSOR_1_MODE)
	  {
      if((int16_t)calibLog[i].calibrationValue < 0 ) { sprintf(strBuffer, "-%1d.%02d", Calibration_Trans_vaule/100, Calibration_Trans_vaule%100); }	        		
	    else{ sprintf(strBuffer, " %1d.%02d", calibLog[i].calibrationValue/100, calibLog[i].calibrationValue%100);}
		 }			  		
	  else if (currentData.Device_Selector_Mode &SENSOR_2_MODE)
		{
        if((int16_t)calibLog[i].calibrationValue < 0 ) { sprintf(strBuffer, "-%1d.%03d", Calibration_Trans_vaule/1000, Calibration_Trans_vaule%1000);	}		
	    else{ sprintf(strBuffer, " %1d.%03d", calibLog[i].calibrationValue/1000, calibLog[i].calibrationValue%1000);	}			 
    }
		DrawMediumNumber(350, 70+(50 * i), strBuffer, WHITE);
 }
		
	for (i = 0 ; i<4 ; i++)
	{  
		  if(calibLog[i].calibrationMethod == 0 )
			{
          calibLog[i].calibrationMethod = 7;
      }
      DrawTextsize96(245,  70+(50 * i), calibLog[i].calibrationMethod, DRAW_IMAGE_ENABLE);
  }
}

void DrawRelay1Wash(uint32_t cur, uint32_t color)
{
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		if(cur==0){ sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashCycle);
	                    DrawMediumNumber(250, 100, strBuffer, color);
     }	
		else { sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashTime);	
                     DrawMediumNumber(250, 150, strBuffer, color);
    }		
	}
	else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
	{
	}
	else
	{
		if(cur==0)
		{
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashCycle);		
			DrawMediumNumber(250, 100, strBuffer, color);
		}
		else if(cur == 1)
		{
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay1WashTime);		
			DrawMediumNumber(250, 150, strBuffer, color);
		}
		else
		{
			if(compareValue)
			{
				if(color == WHITE){}
					//DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_W, DRAW_IMAGE_ENABLE);
				else{}
					//DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_Y, DRAW_IMAGE_ENABLE);
			}
			else
			{
				if(color == WHITE){}
					//DrawTextsize260(110, 200, TEXT260_S2_PRIME_W, DRAW_IMAGE_ENABLE);
				else{}
					//DrawTextsize260(110, 200, TEXT260_S2_PRIME_Y, DRAW_IMAGE_ENABLE);
			}
		} 
	}
}


void DrawRelay2Alarm(uint32_t cur, uint32_t color)
{	
	if((currentData.Device_Selector_Mode == SENSOR_1_MODE)||(currentData.Device_Selector_Mode == SENSOR_2_MODE))
	{
		if(cur==0)
		{
			DrawTextsize55(200, 130, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
		}
		else
		{
			DrawTextsize55(200, 130, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
		}
	}
	else
	{
		if(cur==0)
		{
			//sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay2WashCycle);		
			//DrawMediumNumber(250, 100, strBuffer, color);
		}
		else if(cur == 1)
		{
			//sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay2WashTime);		
			//DrawMediumNumber(250, 150, strBuffer, color);
		}
		else
		{
			if(compareValue)
			{
				if(color == WHITE)
					DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_W, DRAW_IMAGE_ENABLE);
				else
					DrawTextsize260(110, 200, TEXT260_S2_PRIMEEXE_Y, DRAW_IMAGE_ENABLE);
			}
			else
			{
				if(color == WHITE)
					DrawTextsize260(110, 200, TEXT260_S2_PRIME_W, DRAW_IMAGE_ENABLE);
				else
					DrawTextsize260(110, 200, TEXT260_S2_PRIME_Y, DRAW_IMAGE_ENABLE);
			}
		}
	}
	
	
}

void DrawRelay3Wash(uint32_t cur, uint32_t color)
{
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		if(cur==0)
		{
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashCycle);		
			DrawMediumNumber(250, 100, strBuffer, color);
		}
		else
		{
			sprintf(strBuffer, "%3d", tempConfigData.relayConfig.relay3WashTime);		
			DrawMediumNumber(250, 150, strBuffer, color);
		}
	}
	
	else
	{

	}
}

void DrawRelayAlarm(uint32_t val)
{
	if(val)
		DrawTextsize55(200, 130, TEXT55_ENABLE_YELLOW, DRAW_IMAGE_ENABLE);
	else
		DrawTextsize55(200, 130, TEXT55_DISABLE_YELLOW, DRAW_IMAGE_ENABLE);
}
//-----------------------------------------------------------------------------------------------------------------//
void DrawAdjustFilter(uint32_t cur, uint32_t sen, uint32_t color)
{
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS1);
		DrawMediumNumber(250, 165, strBuffer, color);
	}
	else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
	{
		sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS2);
		DrawMediumNumber(250, 165, strBuffer, color);
	}
	else
	{
		if(cur == 0)
		{
			if(currentData.Device_Selector_Mode&SENSOR_1_MODE)
			{
				if(color == YELLOW)
					DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
				else
					 DrawTextsize96(90, 100, TEXT96_NORAMAL, DRAW_IMAGE_ENABLE);
				DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
			}
			else if(currentData.Device_Selector_Mode&SENSOR_2_MODE)
			{
				if(color == YELLOW)
					DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
				else
					DrawTextsize96(90, 100, TEXT96_CI2, DRAW_IMAGE_ENABLE);
			}
		}
		else
		{
			if(sen == 0)
			{
				sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS1);
				DrawMediumNumber(250, 200, strBuffer, color);
			}
			else
			{				
				sprintf(strBuffer, "%3d", tempConfigData.adjustConfig.filterS2);
				DrawMediumNumber(250, 200, strBuffer, color);
			}
		} 
	}
}
//----------------------------------------------------------------------------------

int32_t offsetintVal2 = 0;

void DrawAdjustOffset(uint32_t cur, uint32_t sen, uint32_t color)
{
	
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		 offsetintVal = tempConfigData.adjustConfig.offsetS1;
		 offsetintVal2 = tempConfigData.adjustConfig.offsetS1;
		 if(offsetintVal2 < 0){
       offsetintVal2 = offsetintVal2 * (-1);
    }
		if( offsetintVal < 0)
		{ 
		    sprintf(strBuffer, "-%d.%02d", offsetintVal2/100, offsetintVal2%100);
		    DrawMediumNumber(250, 165, strBuffer, color);
  	}
		else {
        sprintf(strBuffer, " %d.%02d", offsetintVal2/100, offsetintVal2%100);
		    DrawMediumNumber(250, 165, strBuffer, color);
    }		
	}
}


void DrawAdjustGradient(uint32_t cur, uint32_t sen, uint32_t color)
{
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS1/100, tempConfigData.adjustConfig.gradientS1%100);
		DrawMediumNumber(250, 165, strBuffer, color);
	}
	else if(currentData.Device_Selector_Mode == SENSOR_12_MODE)
	{
		sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS2/100, tempConfigData.adjustConfig.gradientS2%100);
		DrawMediumNumber(250, 165, strBuffer, color);
	}
	else
	{
		if(cur == 0)
		{
			if(sen == 0)
			{
				if(color == YELLOW)
					DrawTextsize96(90, 100, TEXT96_NORAMAL, DRAW_IMAGE_ENABLE);
				else
				//	DrawTextsize96(90, 100, TEXT96_SENSOR1_WHITE, DRAW_IMAGE_ENABLE);
				  DrawTextsize96(90, 100, TEXT96_CI, DRAW_IMAGE_ENABLE);
			}
			else
			{
				if(color == YELLOW)
					DrawTextsize96(90, 100, TEXT96_NORAMAL, DRAW_IMAGE_ENABLE);
				else
					DrawTextsize96(90, 100, TEXT96_NORAMAL, DRAW_IMAGE_ENABLE);
			}
		}
		else
		{
			if(sen == 0)
			{
				sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS1/100, tempConfigData.adjustConfig.gradientS1%100);
				DrawMediumNumber(250, 200, strBuffer, color);
			}
			else
			{				
				sprintf(strBuffer, "%d.%02d", tempConfigData.adjustConfig.gradientS2/100, tempConfigData.adjustConfig.gradientS2%100);
				DrawMediumNumber(250, 200, strBuffer, color);
			}
		}
	}
}

void DrawAdjustFactoryReset(uint32_t cur)
{
}

void DrawCalibAuto(uint32_t cur, uint32_t color)   
{
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE)
	{
		
		 tempIntVal = compareSignValue;
			if(tempIntVal < 0)
			tempIntVal =  tempIntVal * (-1);
			
			switch(cur)
		{
		case 0:
			if(compareSignValue < 0)
				sprintf(strBuffer, "-%02d.%02d", tempIntVal/100, tempIntVal%100);
			else
				sprintf(strBuffer, "%02d.%02d", compareSignValue/100, compareSignValue%100);			  
			  DrawMediumNumber(260, 120, strBuffer, color);
				sprintf(strBuffer, "%02d", compareSignValue%100);
		    DrawMediumNumber(260+(22*3), 120, strBuffer, WHITE);
		break;
		case 1:
				if(compareSignValue < 0)
				sprintf(strBuffer, "-%02d.%02d", tempIntVal/100, tempIntVal%100);
			  else
				sprintf(strBuffer, "%02d.%02d", compareSignValue/100, compareSignValue%100);
			  
			  DrawMediumNumber(260, 120, strBuffer, WHITE);
				sprintf(strBuffer, "%02d", compareSignValue%100);
		  	DrawMediumNumber(260+(22*3), 120, strBuffer, color);
		break;
		case 2:
		break;
		}
	}
	else if(currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		switch(cur)
		{
		case 0:
			sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1Value/1000, (tempConfigData.calibrationConfig.S1Value/100)%10);	
			DrawMediumNumber(270, 120, strBuffer, color);
			sprintf(strBuffer, "%2d", tempConfigData.calibrationConfig.S1Value%100);
			DrawMediumNumber(270+(22*3), 120, strBuffer, WHITE);
		break;
		case 1:
			sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1Value/1000, (tempConfigData.calibrationConfig.S1Value/100)%10);	
			DrawMediumNumber(270, 120, strBuffer, WHITE);
			sprintf(strBuffer, "%2d", tempConfigData.calibrationConfig.S1Value%100);
			DrawMediumNumber(270+(22*3), 120, strBuffer, color);
		break;
		case 2:
		break;
	  }
	}
	else
	{
		
	}
}

void DrawCalibManual(uint32_t cur, uint32_t color) 
{
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
	{
		switch(cur)
		{
		case 0:
			sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1manualCal/100, (tempConfigData.calibrationConfig.S1manualCal/10)%10);	
			DrawMediumNumber(270, 120, strBuffer, color);
			sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal%10);  
		 // sprintf(strBuffer, "%d", (tempConfigData.calibrationConfig.S1manualCal%10));  
			DrawMediumNumber(270+(22*3), 120, strBuffer, WHITE);
		break;
		case 1:
			sprintf(strBuffer, "%d.%d", tempConfigData.calibrationConfig.S1manualCal/100, (tempConfigData.calibrationConfig.S1manualCal/10)%10);	
			DrawMediumNumber(270, 120, strBuffer, WHITE);                                               
			sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1manualCal%10);
			DrawMediumNumber(270+(22*3), 120, strBuffer, color);
		break;
		case 2:
			/*
			tempIntVal = compareSignValue;
			if(tempIntVal < 0)
				tempIntVal =  tempIntVal * (-1);
			if(compareSignValue < 0)
				sprintf(strBuffer, "-%02d.%02d", tempIntVal/100, tempIntVal%100);
			else
				sprintf(strBuffer, "%02d.%02d", compareSignValue/100, compareSignValue%100);
			DrawMediumNumber(270, 180, strBuffer, color);
		*/
		break;
		}
	}	
	else if(currentData.Device_Selector_Mode == SENSOR_2_MODE)   
	{
		switch(cur)
		{
		case 0:
			sprintf(strBuffer, "%02d.%d", tempConfigData.calibrationConfig.S2manualCal/1000, (tempConfigData.calibrationConfig.S2manualCal/100)%10);	
			DrawMediumNumber(255, 120, strBuffer, color);
			sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2manualCal%100); 
		 // sprintf(strBuffer, "%d", (tempConfigData.calibrationConfig.S1manualCal%10));  
			DrawMediumNumber(255+(22*4), 120, strBuffer, WHITE);
		break;
		case 1:
			sprintf(strBuffer, "%02d.%d", tempConfigData.calibrationConfig.S2manualCal/1000, (tempConfigData.calibrationConfig.S2manualCal/100)%10);	
			DrawMediumNumber(255, 120, strBuffer, WHITE);                                               
			sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2manualCal%100);
			DrawMediumNumber(255+(22*4), 120, strBuffer, color);
		break;
		case 2:
		
		break;
		}
	}
}

void DrawCalibZero(uint32_t cur, uint32_t color) 
{	
	 		
	if(currentData.Device_Selector_Mode == SENSOR_1_MODE )
	{
     zerointVal =  tempConfigData.calibrationConfig.S1zeroCal;
	
		if(zerointVal < 0){
       zerointVal = zerointVal * (-1);
    }			
		if(cur == 0)  
		{			
			if( tempConfigData.calibrationConfig.S1zeroCal < 0)
			{ 								
		    sprintf(strBuffer, "-%d.%d", zerointVal/100, (zerointVal/10)%10);		
			 	DrawMediumNumber(270, 120, strBuffer, color);				
		    sprintf(strBuffer, "%d",zerointVal%10);
		    DrawMediumNumber(270+(22*4), 120, strBuffer, WHITE);			
      }			
			else
			{					
		    	sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal/100, (tempConfigData.calibrationConfig.S1zeroCal/10)%10);	
	    		DrawMediumNumber(270, 120, strBuffer, color);				  
		    	sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal%10);
		    	DrawMediumNumber(270+(22*4), 120, strBuffer, WHITE);				
			}
		}
		else if(cur == 1) 
		{
			if( tempConfigData.calibrationConfig.S1zeroCal < 0)
			{
          sprintf(strBuffer, "-%d.%d", zerointVal/100, (zerointVal/10)%10);	
		    	DrawMediumNumber(270, 120, strBuffer, WHITE);
		    	sprintf(strBuffer, "%d", zerointVal%10);
		    	//DrawMediumNumber(270+(22*4), 120, strBuffer, color);
      }			
			else
			{
				  sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S1zeroCal/100, (tempConfigData.calibrationConfig.S1zeroCal/10)%10);	
	    		DrawMediumNumber(270, 120, strBuffer, WHITE);				  
		      sprintf(strBuffer, "%d", tempConfigData.calibrationConfig.S1zeroCal%10);
		    	//DrawMediumNumber(270+(22*4), 120, strBuffer, color);				
			}
			 DrawMediumNumber(270+(22*4), 120, strBuffer, color);
		}		
	}
	// Å¹µµ°è -----------------------------------------------------------------------//
	else if(currentData.Device_Selector_Mode == SENSOR_2_MODE)
	{
		zerointVal =  tempConfigData.calibrationConfig.S2zeroCal;
	
		if(zerointVal < 0){
       zerointVal = zerointVal * (-1);
    }	
		if(cur == 0)
		{			
			if( tempConfigData.calibrationConfig.S2zeroCal < 0)
			{ 								
		    sprintf(strBuffer, "-%d.%d", zerointVal/1000, (zerointVal/100)%10);		
			 	DrawMediumNumber(248, 120, strBuffer, color);				
		    sprintf(strBuffer, "%02d",zerointVal%100);
		    //DrawMediumNumber(270+(22*3), 100, strBuffer, WHITE);				
      }			
			else
			{					
		    	sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal/1000, (tempConfigData.calibrationConfig.S2zeroCal/100)%10);	
	    		DrawMediumNumber(248, 120, strBuffer, color);				  
		    	sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal%100);
		    //	DrawMediumNumber(270+(22*3), 100, strBuffer, WHITE);				
			}
			  DrawMediumNumber(248+(22*4), 120, strBuffer, WHITE);		
		}
		else if(cur == 1)
		{
			if( tempConfigData.calibrationConfig.S2zeroCal < 0)
			{
          sprintf(strBuffer, "-%d.%d", zerointVal/1000, (zerointVal/100)%10);	
		    	DrawMediumNumber(248, 120, strBuffer, WHITE);
		    	sprintf(strBuffer, "%02d", zerointVal%100);
		    	DrawMediumNumber(248+(22*4), 120, strBuffer, color);
      }			
			else
			{				  
		    	sprintf(strBuffer, " %d.%d", tempConfigData.calibrationConfig.S2zeroCal/1000, (tempConfigData.calibrationConfig.S2zeroCal/100)%10);	
			    DrawMediumNumber(248, 120, strBuffer, WHITE);
			    sprintf(strBuffer, "%02d", tempConfigData.calibrationConfig.S2zeroCal%100);
			    DrawMediumNumber(248+(22*4), 120, strBuffer, color);
			}
		}
	}
}

void DrawOutputConfig(uint32_t cur, uint32_t color)
{
	switch(cur)
	{
	case 0:
		sprintf(strBuffer, "%d.%d", configData.outputConfig.output4mA/1000, (configData.outputConfig.output4mA/100)%10);	
		DrawMediumNumber(230, 70, strBuffer, color);
		sprintf(strBuffer, "%02d", configData.outputConfig.output4mA%100);
		DrawMediumNumber(230+(22*3), 70, strBuffer, WHITE);
	break;
	case 1:
		sprintf(strBuffer, "%d.%d",configData.outputConfig.output4mA/1000, (configData.outputConfig.output4mA/100)%10);	
		DrawMediumNumber(230, 70, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", configData.outputConfig.output4mA%100);
		DrawMediumNumber(230+(22*3), 70, strBuffer, color);
	break;
	case 2:
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE ){sprintf(strBuffer, "%d.%d",configData.outputConfig.output20mA/1000, (configData.outputConfig.output20mA/100)%10);	} 
		else if (currentData.Device_Selector_Mode == SENSOR_2_MODE){sprintf(strBuffer, "%02d.%d",configData.outputConfig.output20mA/1000, (configData.outputConfig.output20mA/100)%10);}
		DrawMediumNumber(230, 120, strBuffer, color);
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE){sprintf(strBuffer, "%02d", configData.outputConfig.output20mA%100);	} 
		//else if (currentData.Device_Selector_Mode == SENSOR_2_MODE){sprintf(strBuffer, "%02d", configData.outputConfig.output20mA%100);}		
		 DrawMediumNumber(230+(22*3), 120, strBuffer, WHITE);
	break;
	case 3:
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE ){sprintf(strBuffer, "%d.%d",configData.outputConfig.output20mA/1000, (configData.outputConfig.output20mA/100)%10);	} 
		else if (currentData.Device_Selector_Mode == SENSOR_2_MODE){sprintf(strBuffer, "%02d.%d",configData.outputConfig.output20mA/1000, (configData.outputConfig.output20mA/100)%10);}
		DrawMediumNumber(230, 120, strBuffer, WHITE);
		if(currentData.Device_Selector_Mode == SENSOR_1_MODE || currentData.Device_Selector_Mode == SENSOR_2_MODE ){sprintf(strBuffer, "%02d", configData.outputConfig.output20mA%100);	} 
		//else if (currentData.Device_Selector_Mode == SENSOR_2_MODE){sprintf(strBuffer, "%02d", configData.outputConfig.output20mA%100);}		
		DrawMediumNumber(230+(22*3), 120, strBuffer, color);
	break;
	case 4:
		sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal4mA/100);
		DrawMediumNumber(230, 170, strBuffer, color);
		sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal4mA%100);
		DrawMediumNumber(230+(22*3), 170, strBuffer, WHITE);
	break;
	case 5:
		sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal4mA/100);
		DrawMediumNumber(230, 170, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal4mA%100);
		DrawMediumNumber(230+(22*3), 170, strBuffer, color);
	break;
	case 6:
		sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal20mA/100);
		DrawMediumNumber(230, 220, strBuffer, color);
		sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal20mA%100);
		DrawMediumNumber(230+(22*3), 220, strBuffer, WHITE);
	break;
	case 7:
		sprintf(strBuffer, "%03d", tempConfigData.outputConfig.outputReal20mA/100);
		DrawMediumNumber(230, 220, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", tempConfigData.outputConfig.outputReal20mA%100);
		DrawMediumNumber(230+(22*3), 220, strBuffer, color);
	break;
	case 8:
		if(compareValue)
		{
			if(color == WHITE)
				DrawTextETC(10, 220, TEXT_ETC_HOLD_W, DRAW_IMAGE_ENABLE);
			else
				DrawTextETC(10, 220, TEXT_ETC_HOLD_Y, DRAW_IMAGE_ENABLE);
			
		}
		else
		{
			if(color == WHITE)
				DrawTextETC(10, 220, TEXT_ETC_RUN_W, DRAW_IMAGE_ENABLE);
			else
				DrawTextETC(10, 220, TEXT_ETC_RUN_Y, DRAW_IMAGE_ENABLE);
		}
	break;
	}
}

void DrawAlarmConfig(uint32_t cur, uint32_t color)
{
	if(currentData.Device_Selector_Mode&SENSOR_1_MODE )
	{
	  switch(cur)
   	{
  	case 0:
		sprintf(strBuffer, "%d.%d", tempConfigData.alarmConfig.highLimit/1000, (tempConfigData.alarmConfig.highLimit/100)%10);
		DrawMediumNumber(200, 90, strBuffer, color);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit%100);
		DrawMediumNumber(200+(22*3), 90, strBuffer, WHITE);
	  break;
	  case 1:
		sprintf(strBuffer, "%d.%d", tempConfigData.alarmConfig.highLimit/1000, (tempConfigData.alarmConfig.highLimit/100)%10);
		DrawMediumNumber(200, 90, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit%100);
		DrawMediumNumber(200+(22*3), 90, strBuffer, color);
  	break;
	  case 2:
		sprintf(strBuffer, "%d.%d", tempConfigData.alarmConfig.lowLimit/1000, (tempConfigData.alarmConfig.lowLimit/100)%10);
		DrawMediumNumber(200, 140, strBuffer, color);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit%100);
		DrawMediumNumber(200+(22*3), 140, strBuffer, WHITE);
	  break;
	  case 3:
		sprintf(strBuffer, "%d.%d", tempConfigData.alarmConfig.lowLimit/1000, (tempConfigData.alarmConfig.lowLimit/100)%10);
		DrawMediumNumber(200, 140, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit%100);
		DrawMediumNumber(200+(22*3), 140, strBuffer, color);
	  break;
	  case 4:
		sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm);
		DrawMediumNumber(200+22, 190, strBuffer, color);
	  break;
	}
}
else if(currentData.Device_Selector_Mode&SENSOR_2_MODE )
	{
		switch(cur)
   	{
  	case 0:
		sprintf(strBuffer, "%02d.%d", tempConfigData.alarmConfig.highLimit/1000, (tempConfigData.alarmConfig.highLimit/100)%10);
		DrawMediumNumber(200, 90, strBuffer, color);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit%100);
		DrawMediumNumber(200+(22*4), 90, strBuffer, WHITE);
	  break;
	  case 1:
		sprintf(strBuffer, "%02d.%d", tempConfigData.alarmConfig.highLimit/1000, (tempConfigData.alarmConfig.highLimit/100)%10);
		DrawMediumNumber(200, 90, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.highLimit%100);
		DrawMediumNumber(200+(22*4), 90, strBuffer, color);
  	break;
	  case 2:
		sprintf(strBuffer, "%02d.%d", tempConfigData.alarmConfig.lowLimit/1000, (tempConfigData.alarmConfig.lowLimit/100)%10);
		DrawMediumNumber(200, 140, strBuffer, color);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit%100);
		DrawMediumNumber(200+(22*4), 140, strBuffer, WHITE);
	  break;
	  case 3:
		sprintf(strBuffer, "%02d.%d", tempConfigData.alarmConfig.lowLimit/1000, (tempConfigData.alarmConfig.lowLimit/100)%10);
		DrawMediumNumber(200, 140, strBuffer, WHITE);
		sprintf(strBuffer, "%02d", tempConfigData.alarmConfig.lowLimit%100);
		DrawMediumNumber(200+(22*4), 140, strBuffer, color);
	  break;
	  case 4:
		sprintf(strBuffer, "%3d", tempConfigData.alarmConfig.zeroAlarm);
		DrawMediumNumber(200+22, 190, strBuffer, color);
	  break;
	}		
}


}
void DrawTimeConfig(uint32_t cur, uint32_t color)
{
	switch(cur)
	{
	case 0:
		sprintf(strBuffer, "%4d", tempConfigTime.tm_year);		DrawMediumNumber(49, 100, strBuffer, color);
	break;
	case 1:
		sprintf(strBuffer, "%2d", tempConfigTime.tm_mon+1);		DrawMediumNumber(219, 100, strBuffer, color);
	break;
	case 2:
		sprintf(strBuffer, "%2d", tempConfigTime.tm_mday);		DrawMediumNumber(343, 100, strBuffer, color);
	break;
	case 3:
		sprintf(strBuffer, "%2d", tempConfigTime.tm_hour);		DrawMediumNumber(93, 200, strBuffer, color);
	break;
	case 4:
		sprintf(strBuffer, "%2d", tempConfigTime.tm_min);		DrawMediumNumber(219, 200, strBuffer, color);
	break;
	case 5:
		sprintf(strBuffer, "%2d", tempConfigTime.tm_sec);		DrawMediumNumber(343, 200, strBuffer, color);
	break;
	}
}
void DrawCommConfig(uint32_t cur, uint32_t color)
{
	switch(cur)
	{
	case 0:
		DrawMediumNumber(230+66, 70, (char*)comModeText[tempConfigData.modbusConfig.mode], color);   
	break;
	case 1:
		DrawMediumNumber(230, 120, (char*)comBaudrateText[tempConfigData.modbusConfig.baudrate], color);   
	break;
	case 2:
		sprintf(strBuffer, "%3d", tempConfigData.modbusConfig.modbusSlaveAddr+1);		DrawMediumNumber(230+66, 170, strBuffer, color);  
	break;
	case 3:
		sprintf(strBuffer, "%d", tempConfigData.modbusConfig.databit+8);		DrawMediumNumber(230+110, 220, strBuffer, color); 
	break;
	}
}

void DrawBottomCurrent(uint32_t current)
{
	sprintf(strBuffer,"4-20" );//"%2d.%01d", current/10, current %10);
	DrawSmallNumber(340, 245, strBuffer);
}

