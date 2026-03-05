
//===========================================
#include "set9_time_range1.h"
#include "set9_time_range2.h"
#include "set9_nongdo_range1.h"
#include "set9_nongdo_range2.h"

#include "set10_sensor_ok.h"
#include "set10_sensor_error1.h"
#include "set10_sensor_error2.h"
#include "set10_sensor_error3.h"

#include "set11_run1.h"
#include "set11_run2.h"
#include "set11_stop1.h"
#include "set11_stop2.h"
#include "set11_manwash.h"
#include "set11_running.h"

#include "set12_ph4_1.h"
#include "set12_ph4_2.h"
#include "set12_ph7_1.h"
#include "set12_ph7_2.h"

#include "buffer_W.h"


#define BLOCK_NO	54

void MAKE_file(void)
{
	int i,j, length;

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;


	Deley_10msec(100);

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);
	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	for(i = 0, j=0 ; i < 7 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set9_time_range1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 14 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set9_time_range2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 21 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set9_nongdo_range1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 28 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set9_nongdo_range1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 32 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set10_sensor_ok[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 36 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set10_sensor_error1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 40 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set10_sensor_error2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 44 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set10_sensor_error3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 51 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set11_run1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 58 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set11_run2[j*2048]), WriteReadAddr, 1);
	}




	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	Deley_10msec(100);
	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i = 0, j=0 ; i < 7 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set11_stop1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 14 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set11_stop2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set11_manwash[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 27 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set11_running[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 34 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set12_ph4_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 41 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set12_ph4_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set12_ph7_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 55 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set12_ph7_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 59 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_buffer_W[j*2048]), WriteReadAddr, 1);
	}


	INDLED_ON;
}

