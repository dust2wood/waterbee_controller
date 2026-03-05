
//===========================================
#include "Bgraph1_1.h"
#include "Bgraph1_2.h"

#include "trend_hour1.h"
#include "trend_hour2.h"
#include "trend_hour3.h"
#include "trend_hour4.h"
#include "trend_hour5.h"
#include "trend_hour6.h"



#define BLOCK_NO	70

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
	for(i = 0, j=0 ; i < 40 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Bgraph1_1[j*2048]), WriteReadAddr, 1);
	}



	/* Erase the NAND Block */
	WriteReadAddr.Block =71;  //===============
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for( i=0, j=0; i < 40 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Bgraph1_2[j*2048]), WriteReadAddr, 1);
	}







	/* Erase the NAND Block */
	WriteReadAddr.Block = 76;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);
	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	for(i = 0, j=0 ; i < 3 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_trend_hour1[j*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 6 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_trend_hour2[j*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 9 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_trend_hour3[j*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_trend_hour4[j*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 15 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_trend_hour5[j*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 18 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_trend_hour6[j*2048]), WriteReadAddr, 1);
	}



	INDLED_ON;
}

