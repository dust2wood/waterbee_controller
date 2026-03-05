
//===========================================
#include "mgl_1.h"
#include "mgl_2.h"
#include "mgl_3.h"
#include "NTU_1.h"
#include "NTU_3.h"
#include "mV_1.h"
#include "uscm.h"
#include "uscm_3.h"
#include "4mA.h"
#include "20mA.h"
#include "year.h"
#include "mon.h"
#include "day.h"
#include "hour.h"
#include "min.h"
#include "sec.h"
#include "range1.h"
#include "range2.h"
#include "value_max.h"
#include "value_min.h"
#include "zero_alarm.h"
#include "ph.h"
#include "ph4.h"
#include "ph7.h"
#include "ph10.h"
#include "comm_method.h"
#include "comm_number.h"
#include "comm_baud.h"
#include "comm_databit.h"

#include "message5.h"

#include "set_period.h"
#include "set_time.h"

#include "4_20mA.h"
#include "RS_232.h"
#include "RS_485.h"


#include "message1.h"
#include "message2.h"
#include "message3.h"
#include "message4.h"
#include "message6_1.h"
#include "message6_2.h"
#include "back_graph_1.h"
#include "back_graph_2.h"
#include "back_graph_3.h"



#define BLOCK_NO	62

void MAKE_file(void)
{
	int i,j, length;

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);
	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	for(i = 0, j=0 ; i < 2 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_mgl_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 4 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_mgl_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 5 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_mgl_3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 7 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_NTU_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 8 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_NTU_3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 9 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_mV_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_uscm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 13 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_uscm_3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 14 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4mA[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 15 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_20mA[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_year[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 17 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_mon[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 18 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_day[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 19 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_hour[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_min[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 21 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_sec[j*2048]), WriteReadAddr, 1);
	}





	for( j=0; i < 22 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_range1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 23 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_range2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_value_max[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 25 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_value_min[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 26 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_zero_alarm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 27 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_ph[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 28 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_ph4[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 29 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_ph7[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 31 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_ph10[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 33 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_comm_method[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 35 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_comm_number[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 37 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_comm_baud[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 39 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_comm_databit[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 41 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message5[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 43 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_period[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 45 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set_time[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 51 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4_20mA[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 57 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_RS_232[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 63 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_RS_485[j*2048]), WriteReadAddr, 1);
	}




	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;	// 63
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);



	for( i=0, j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 32 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 64 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message4[j*2048]), WriteReadAddr, 1);
	}






	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+2;		 // 64
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);


	for( i=0, j=0; i < 19 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message6_1[j*2048]), WriteReadAddr, 1);
	}
	for( j=0; i < 38 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_message6_2[j*2048]), WriteReadAddr, 1);
	}


	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+3;		 // 65
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);


	for( i=0, j=0; i < 19 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_graph_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 38 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_graph_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 57 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_graph_3[j*2048]), WriteReadAddr, 1);
	}


	INDLED_ON;
}

