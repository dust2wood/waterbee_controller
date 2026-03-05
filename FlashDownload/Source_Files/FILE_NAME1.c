
//===========================================
#include "name_back.h"
#include "analog.h"
#include "comm.h"
#include "autowash.h"
#include "manwash.h"
#include "alarm.h"
#include "autoalarm.h"
#include "alarm_off.h"
#include "act_set.h"
#include "act_manual.h"
#include "time.h"
#include "slope.h"
#include "offset.h"
#include "filter.h"
#include "init.h"
#include "zero.h"
#include "span.h"
#include "temp.h"
#include "hystory.h"
#include "time_range.h"
#include "nongdo_range.h"
#include "value_scale.h"
#include "diag.h"
#include "buffer.h"


#include "cl1.h"
#include "cl2.h"
#include "o2.h"
#include "elec.h"
#include "home.h"


#include "zero_W.h"
#include "span_W.h"
#include "temp_W.h"


#define BLOCK_NO	60

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
	for(i = 0, j=0 ; i < 4 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_name_back[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 8 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_analog[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_comm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_autowash[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_manwash[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_alarm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 28 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_autoalarm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 32 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_alarm_off[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 36 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_act_set[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 40 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_act_manual[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 44 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_time[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_slope[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 52 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_offset[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 56 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_filter[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 60 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_init[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 64 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_zero[j*2048]), WriteReadAddr, 1);
	}




	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i = 0, j=0 ; i < 4 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_span[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 8 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_temp[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_hystory[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_time_range[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_nongdo_range[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_value_scale[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 28 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_diag[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 32 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_buffer[j*2048]), WriteReadAddr, 1);
	}

	//=====================================

	for( j=0; i < 36 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_CL1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 40 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_CL2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 44 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_o2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_elec[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 52 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_zero_W[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 56 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_span_W[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 60 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_temp_W[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 64 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_home[j*2048]), WriteReadAddr, 1);
	}


	INDLED_ON;
}

