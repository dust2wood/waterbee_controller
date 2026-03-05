
//===========================================

#include "set5_alarm1.h"
#include "set5_alarm2.h"
#include "set5_clean1.h"
#include "set5_clean2.h"
#include "set5_relay1.h"
#include "set5_relay2.h"

#include "set6_act_passive1.h"
#include "set6_act_passive2.h"
#include "set6_act_set1.h"
#include "set6_act_set2.h"

#include "set7_autowash1.h"
#include "set7_autowash2.h"
#include "set7_manwash1.h"
#include "set7_manwash2.h"

#include "set8_autoalarm1.h"
#include "set8_autoalarm2.h"
#include "set8_noalarm1.h"
#include "set8_noalarm2.h"


#define BLOCK_NO	52

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
	for(i = 0, j=0 ; i < 4 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set5_alarm1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 8 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set5_alarm2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set5_clean1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set5_clean2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set5_relay1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set5_relay2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 31 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set6_act_passive1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 38 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set6_act_passive2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 45 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set6_act_set1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 52 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set6_act_set2[j*2048]), WriteReadAddr, 1);
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
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set7_autowash1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 14 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set7_autowash2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 21 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set7_manwash1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 28 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set7_manwash2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 35 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set8_autoalarm1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 42 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set8_autoalarm2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 49 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set8_noalarm1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 56 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_set8_noalarm2[j*2048]), WriteReadAddr, 1);
	}



	INDLED_ON;
}

