
//===========================================

#include "set_set1.h"
#include "set_set2.h"
#include "set_alarm1.h"
#include "set_alarm2.h"
#include "set_cal1.h"
#include "set_cal2.h"
#include "set_diag1.h"
#include "set_diag2.h"
#include "set_trand1.h"
#include "set_trand2.h"

#include "set2_analog1.h"
#include "set2_analog2.h"
#include "set2_calib1.h"
#include "set2_calib2.h"
#include "set2_comm1.h"
#include "set2_comm2.h"
#include "set2_relay1.h"
#include "set2_relay2.h"
#include "set2_time1.h"
#include "set2_time2.h"

#include "set3_log1.h"
#include "set3_log2.h"
#include "set3_span1.h"
#include "set3_span2.h"
#include "set3_temp1.h"
#include "set3_temp2.h"
#include "set3_zero1.h"
#include "set3_zero2.h"

#include "set3_buff1.h"
#include "set3_buff2.h"

#include "set4_filter1.h"
#include "set4_filter2.h"
#include "set4_init1.h"
#include "set4_init2.h"
#include "set4_offset1.h"
#include "set4_offset2.h"
#include "set4_slope1.h"
#include "set4_slope2.h"

//#include "set_1.h"
//#include "set_2.h"


#define BLOCK_NO	50

void MAKE_file(void)
{
	int i,j, length;

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;


	Deley_10msec(100);
	FSMC_NAND_EraseBlock(WriteReadAddr);

	Deley_10msec(200);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);
	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	for(i = 0, j=0 ; i < 2 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_set1[i*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 4 ; i++,j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_set2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 6 ; i++,j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_alarm1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 8 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_alarm2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 10 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_cal1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_cal2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 14 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_diag1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_diag2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 18 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_trand1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_trand2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 23 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_analog1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 26 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_analog2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 29 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_calib1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 32 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_calib2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 35 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_comm1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 38 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_comm2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 41 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_relay1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 44 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_relay2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 47 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_time1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 50 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set2_time2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);



	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	Deley_10msec(100);
	
	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i = 0, j=0 ; i < 3 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_log1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 6 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_log2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 9 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_span1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_span2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 15 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_temp1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 18 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_temp2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 21 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_zero1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_zero2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 27 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_filter1[j*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 30 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_filter2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 33 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_init1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 36 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_init2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 39 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_offset1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 42 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_offset2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 45 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_slope1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set4_slope2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);


	for(j=0 ; i < 51 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_buff1[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);

	for(j=0 ; i < 54 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set3_buff2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(1);





	INDLED_ON;
}

