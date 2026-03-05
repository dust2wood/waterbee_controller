#include "MicroSd.h"


FATFS fs;
FIL fsrc;
FRESULT res;
UINT br;

//uint8_t textFileBuffer[22];
char filenameBuffer[25];
char WriteBuffer[45];
uint16_t WirteSDCard_flag = 0;
uint32_t reseult = 1520;


uint8_t  SD_WriteMin=0;

// sd 카드에 한달에 한개 데이터 생성하고 1분에 60개씩 저장한다.
// 하루에 24*60*60=86400 개 생성됨
// 한달에 2592000 개, * 약 30bytes = 약 76MB

// 16GB 이면 16*1024/76 = 215 달 = 18년


// 테스트는 한시간에 한개씩 만든다., 원래는 한달에 한개 파일이다.
//#define TEST_SD_CARD_HOUR


void MicroSD_Write(void) {
    uint16_t i = 0;

    currentTime = Time_GetCalendarTime();

	// 1분에 한개 저장하자.
#ifdef SAVE_1_MINUTE
	if (currentTime.tm_min==SD_WriteMin) return;

#else	// 3분마다 저장하기
	if (SD_WriteMin<57) {
		if (currentTime.tm_min!=SD_WriteMin+3) return;
	}
	else {
		if (SD_WriteMin==57) 
		{
			if (currentTime.tm_min!=0) return;
		}
		if (SD_WriteMin==58) 
		{
			if (currentTime.tm_min!=1) return;
		}
		if (SD_WriteMin==59) 
		{
			if (currentTime.tm_min!=2) return;
		}
	}
#endif

	SD_WriteMin = currentTime.tm_min;


    f_mount(0, &fs);

//    sprintf(filenameBuffer, "0:/20%04d-%02d-%02d %02d%02d.txt", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min);
//	sprintf(filenameBuffer, "0:/%02d%02d%02d%02d.txt", currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min);

#ifdef TEST_SD_CARD_HOUR
	// 테스트로 한시간에 한개씩만든다.
    sprintf(filenameBuffer, "20%02d%02d%02d%02d.txt", currentTime.tm_year-2000, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour);
#else
	// 한달단위로 해야하는데 테스트로 한시간에 한개씩만든다.
    sprintf(filenameBuffer, "20%02d%02d.txt", currentTime.tm_year-2000, currentTime.tm_mon + 1);
#endif


	// 해야하는거, 데이터를  60개 write 하는데 알맞는 데이터를 write해야한다.
//	aaa


	// 먼저 파일이 있으면 이어쓰기한다.
	// 없으면 새로 만든다.
    res = f_open(&fsrc, filenameBuffer, FA_OPEN_EXISTING | FA_WRITE);

    if (res == FR_OK) {
		f_lseek(&fsrc, fsrc.fsize);
	}
	else if (res == FR_NO_FILE) 	 // FR_NO_FILE
	    res = f_open(&fsrc, filenameBuffer, FA_CREATE_NEW | FA_WRITE);


    if (res == FR_OK || res == FR_EXIST) {
//        if (WirteSDCard_flag == 1) 
		  {

#ifdef SAVE_SECOND
           for (i = 0; i < 60; i++) 
#else 
			i =1;
#endif
		   {

#ifndef SENSOR_PH_EC				
				if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
					if (TEMP_card_Data[i]<0) 	f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%02d, mg/L, -%2d.%d, C\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i, PPmSD_card_Data[i] / 100, PPmSD_card_Data[i] % 100, abs(TEMP_card_Data[i]) / 100, (abs(TEMP_card_Data[i])/10) % 10);
                    else 						f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%02d, mg/L, %2d.%d, C\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i, PPmSD_card_Data[i] / 100, PPmSD_card_Data[i] % 100,     TEMP_card_Data[i]  / 100,     (TEMP_card_Data[i]/10)  % 10);
//                    sprintf(WriteBuffer, "%4d-%02d,%02d, %02d:%02d:%02d, %1d.%02d mg/L, T:%2d.%d C\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, PPmSD_card_Data[i] / 100, PPmSD_card_Data[i] % 100, TEMP_card_Data[i] / 10, TEMP_card_Data[i] % 10);
//	                res = f_write(&fsrc, WriteBuffer, sizeof (WriteBuffer), &br);

                } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
                    f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%03d, NTU\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i, PPmSD_card_Data[i] / 1000, PPmSD_card_Data[i] % 1000);
//                    sprintf(WriteBuffer, "%4d-%02d,%02d, %02d:%02d:%02d, %02d.%03d NTU\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, PPmSD_card_Data[i] / 1000, PPmSD_card_Data[i] % 1000);
//    	            res = f_write(&fsrc, WriteBuffer, sizeof (WriteBuffer), &br);
                }
#else
				if (currentData.Device_Selector_Mode & SENSOR_1_MODE) {
					if (TEMP_card_Data[i]<0) 	f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%02d, pH, -%2d.%d, C\r\n", \
									currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i, \
									PPmSD_card_Data[i] / 100, PPmSD_card_Data[i] % 100, abs(TEMP_card_Data[i]) / 10, abs(TEMP_card_Data[i]) % 10);

					else 						f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%02d, pH, %2d.%d, C\r\n", 					   \
									currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i,  \
									PPmSD_card_Data[i] / 100, PPmSD_card_Data[i] % 100,     TEMP_card_Data[i]  / 10,     TEMP_card_Data[i]  % 10);
//                    sprintf(WriteBuffer, "%4d-%02d,%02d, %02d:%02d:%02d, %1d.%02d mg/L, T:%2d.%d C\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, PPmSD_card_Data[i] / 100, PPmSD_card_Data[i] % 100, TEMP_card_Data[i] / 10, TEMP_card_Data[i] % 10);
//	                res = f_write(&fsrc, WriteBuffer, sizeof (WriteBuffer), &br);

                } else if (currentData.Device_Selector_Mode & SENSOR_2_MODE) {
					if (TEMP_card_Data[i]<0) 	f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%01d, uS/cm, -%2d.%d, C\r\n", currentTime.tm_year, currentTime.tm_mon + 1, \
									currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i, PPmSD_card_Data[i] / 10, PPmSD_card_Data[i] % 10,				  \
									abs(TEMP_card_Data[i]) / 10, abs(TEMP_card_Data[i]) % 10);
					else 						f_printf(&fsrc, "%4d-%02d-%02d %02d:%02d:%02d, %1d.%01d, uS/cm, %2d.%d, C\r\n", currentTime.tm_year, currentTime.tm_mon + 1, 	\
									currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, i, PPmSD_card_Data[i] / 10, PPmSD_card_Data[i] % 10,					\
									TEMP_card_Data[i]  / 10,     TEMP_card_Data[i]  % 10);


//                    sprintf(WriteBuffer, "%4d-%02d,%02d, %02d:%02d:%02d, %02d.%03d NTU\r\n", currentTime.tm_year, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, PPmSD_card_Data[i] / 1000, PPmSD_card_Data[i] % 1000);
//    	            res = f_write(&fsrc, WriteBuffer, sizeof (WriteBuffer), &br);
                }

#endif


            }
            WirteSDCard_flag = 0;
        }

        f_close(&fsrc);
    } else if (res == FR_EXIST) {

    }


}

FRESULT scan_files(char* path) {
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof (lfn);
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

int SD_TotalSize(void) {
    FATFS *fs;
    DWORD fre_clust;

    res = f_getfree("0:", &fre_clust, &fs);
    if (res == FR_OK) {


        return ENABLE;
    } else
        return DISABLE;
}


extern int16_t trandData[6][61] ;
extern uint8_t trandData_TIME[6][61][6] ;


void Read_Tranddata (unsigned char no)
{
    uint16_t i = 0, j=0, k;

	TCHAR buf[480];
	UINT len;

	// 먼저 파일이 있으면 .
	// 없으면 새로 만든다.
    res = f_open(&fsrc, filenameBuffer, FA_OPEN_EXISTING | FA_READ);

	if (res == FR_NO_FILE) 	 // FR_NO_FILE
	{
	    res = f_open(&fsrc, filenameBuffer, FA_CREATE_NEW | FA_READ);
	    if (res == FR_OK || res == FR_EXIST) {
			f_write (&fsrc, trandData[no], 480, &len);
	    }
	}

    else if (res == FR_OK || res == FR_EXIST) {
		f_read (&fsrc, buf, 480, &len);

		// 데이터를 저장한다.
		for (i=0, j=0;i<480; j++) {
			trandData_TIME[no][j][0]=buf[i++];
			trandData_TIME[no][j][1]=buf[i++];
			trandData_TIME[no][j][2]=buf[i++];
			trandData_TIME[no][j][3]=buf[i++];
			trandData_TIME[no][j][4]=buf[i++];
			trandData_TIME[no][j][5]=buf[i++];

			k = buf[i++];
			trandData[no][j]= (k<<8) | buf[i++];
		}
    }
    f_close(&fsrc);
}


// trand data read	
void MicroSD_Trand_Read(void) {

    f_mount(0, &fs);

	// 1번 트렌드 파일 
    sprintf(filenameBuffer, "TRAND1.txt");
	Read_Tranddata(0);


	// 6번 트렌드 파일 
    sprintf(filenameBuffer, "TRAND6.txt");
	Read_Tranddata(1);

	// 12번 트렌드 파일 
    sprintf(filenameBuffer, "TRAND12.txt");
	Read_Tranddata(2);

	// 24번 트렌드 파일 
    sprintf(filenameBuffer, "TRAND24.txt");
	Read_Tranddata(3);

	// 168번 트렌드 파일 
    sprintf(filenameBuffer, "TRAND168.txt");
	Read_Tranddata(4);

	// 336번 트렌드 파일 
    sprintf(filenameBuffer, "TRAND336.txt");
	Read_Tranddata(5);

}


extern char flag_write_trand1, flag_write_trand6, flag_write_trand12, flag_write_trand24, flag_write_trand168, flag_write_trand336;


void Write_TrandData(unsigned char no)
{
    uint16_t i = 0;

	UINT len;

	// 먼저 파일이 있으면 .
	// 없으면 새로 만든다.
    res = f_open(&fsrc, filenameBuffer, FA_OPEN_EXISTING | FA_WRITE);

    if (res == FR_OK) {
		f_lseek(&fsrc, 0);
	}
	else if (res == FR_NO_FILE) 	 // FR_NO_FILE
	    res = f_open(&fsrc, filenameBuffer, FA_CREATE_NEW | FA_WRITE);

    if (res == FR_OK || res == FR_EXIST) {
		for (i=0;i<60;i++) {
			f_write (&fsrc, trandData_TIME[no][i], 6, &len);
			f_write (&fsrc, &trandData[no][i], 2, &len);
		}
    }
    f_close(&fsrc);
}


void MicroSD_Trand_Write(void) {
    uint16_t i = 0;

//	TCHAR buf[120];
//	UINT len;

    f_mount(0, &fs);


	if (flag_write_trand1==1) {
		flag_write_trand1=0;
		// 1번 트렌드 파일 
	    sprintf(filenameBuffer, "TRAND1.txt");
		Write_TrandData(0);
	}


	if (flag_write_trand6==1) {
		flag_write_trand6=0;
		// 1번 트렌드 파일 
	    sprintf(filenameBuffer, "TRAND6.txt");
		Write_TrandData(1);
	}


	if (flag_write_trand12==1) {
		flag_write_trand12=0;
		// 1번 트렌드 파일 
	    sprintf(filenameBuffer, "TRAND12.txt");
		Write_TrandData(2);
	}

	if (flag_write_trand24==1) {
		flag_write_trand24=0;
		// 1번 트렌드 파일 
	    sprintf(filenameBuffer, "TRAND24.txt");
		Write_TrandData(3);
	}


	if (flag_write_trand168==1) {
		flag_write_trand168=0;
		// 1번 트렌드 파일 
	    sprintf(filenameBuffer, "TRAND168.txt");
		Write_TrandData(4);
	}


	if (flag_write_trand336==1) {
		flag_write_trand336=0;
		// 1번 트렌드 파일 
	    sprintf(filenameBuffer, "TRAND336.txt");
		Write_TrandData(5);
	}
}

//==============================================
// 교정이력
//==============================================


void Read_SetHistory (void)
{
    uint16_t i = 0, j=0, k;

	TCHAR buf[480];
	UINT len;

    f_mount(0, &fs);

	// 먼저 파일이 있으면 .
	// 없으면 새로 만든다.
    res = f_open(&fsrc, "Setting.dat", FA_OPEN_EXISTING | FA_READ);

	if (res == FR_NO_FILE) 	 // FR_NO_FILE
	{
//	    res = f_open(&fsrc, filenameBuffer, FA_CREATE_NEW | FA_READ);
//	    if (res == FR_OK || res == FR_EXIST) {
//			f_write (&fsrc, trandData[no], 480, &len);
//	    }
	}

    else if (res == FR_OK || res == FR_EXIST) {
		f_read (&fsrc, EEpromdata.ReadEEpromData_Buffer, 120, &len);
    }
    f_close(&fsrc);
}


void Write_SetHistory(void)
{
    uint16_t i = 0;

	UINT len;


    f_mount(0, &fs);


	// 먼저 파일이 있으면 .
	// 없으면 새로 만든다.
    res = f_open(&fsrc,  "Setting.dat", FA_OPEN_EXISTING | FA_WRITE);

    if (res == FR_OK) {
		f_lseek(&fsrc, 0);
	}
	else if (res == FR_NO_FILE) 	 // FR_NO_FILE
	    res = f_open(&fsrc, "Setting.dat", FA_CREATE_NEW | FA_WRITE);

    if (res == FR_OK || res == FR_EXIST) {
		f_write (&fsrc, EEpromdata.ReadEEpromData_Buffer, 120, &len);
    }
    f_close(&fsrc);
}


