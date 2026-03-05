#include "MicroSd.h"


FATFS fs;     
FIL fsrc;       
FRESULT res;
UINT br;

uint8_t textFileBuffer[22];  
char filenameBuffer[25];
char filenameBuffer2[22];
uint16_t WirteSDCard_flag = 0;
uint32_t reseult = 1520;

void MicroSD_Write (void)
{
	 uint16_t i = 0;
	
   f_mount(0,&fs);	

	  currentTime = Time_GetCalendarTime();
	  sprintf(filenameBuffer, "0:/%02d%02d%02d%02d.txt",currentTime.tm_mon+1,currentTime.tm_mday,currentTime.tm_hour,currentTime.tm_min);
	
	  res = f_open( &fsrc , filenameBuffer , FA_CREATE_NEW | FA_WRITE);	
 
  	if (res == FR_OK )
    { 
			if(WirteSDCard_flag == 1) {
			for(i =0 ; i< 60 ; i++) {	
			   if(currentData.Device_Selector_Mode&SENSOR_1_MODE ) {
		     	sprintf(filenameBuffer2, " Logdata: %1d.%02d mg/L, T:%d.%d C\r\n",PPmSD_card_Data[i]/100, PPmSD_card_Data[i]%100, TEMP_card_Data[i]/10,  TEMP_card_Data[i]%10);	
			   }
			   else if(currentData.Device_Selector_Mode&SENSOR_2_MODE ) {
			    sprintf(filenameBuffer2, " Logdata: %02d.%03d NTU, T:%d.%d\r\n",PPmSD_card_Data[i]/1000, PPmSD_card_Data[i]%1000, TEMP_card_Data[i]/10,  TEMP_card_Data[i]%10);	
				 }
     
      res = f_write(&fsrc, filenameBuffer2, sizeof(textFileBuffer), &br); 			
      }
			WirteSDCard_flag = 0;
		}
     
      f_close(&fsrc);      
    }
    else if ( res == FR_EXIST )
    {
	 
    }

	  
}


FRESULT scan_files (char* path)
{
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {
               
            }
        }
    }

    return res;
}

int SD_TotalSize(void)
{
    FATFS *fs;
    DWORD fre_clust;        

    res = f_getfree("0:", &fre_clust, &fs);  
    if ( res==FR_OK ) 
    {
	   
		
	  return ENABLE;
	}
	else 
	  return DISABLE;   
}	 

