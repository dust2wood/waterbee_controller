
//===========================================
#include "8x10_BLUE_GRAY.h"
#include "8x10_BLUE_White.h"
#include "12x16_BLK_GRAY.h"
#include "12x16_BLK_W.h"
#include "12x16_BLUE_GRAY.h"
#include "12x16_BLUE_W.h"
#include "18x23_BLK_GRAY.h"
#include "18x23_BLK_WHITE.h"
#include "23x28_BLK_GRAY.h"
#include "23x28_BLK_WHITE.h"

#include "8x10_BLK_White.h"

#include "33x46_BLK_White.h"
//#include "36x46_BLK_White.h"
//#include "42x54_BLK_White.h"


#define BLOCK_NO	30

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
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_8x10_BLUE_White[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 4 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_8x10_BLUE_GRAY[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 7 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_12x16_BLK_W[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 10 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_12x16_BLK_GRAY[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 13 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_12x16_BLUE_W[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_12x16_BLUE_GRAY[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 28 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_18x23_BLK_WHITE[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 36 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_18x23_BLK_GRAY[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_23x28_BLK_WHITE[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 60 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_23x28_BLK_GRAY[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 62 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_8x10_BLK_White[j*2048]), WriteReadAddr, 1);
	}







	/* Erase the NAND Block */
	WriteReadAddr.Block = 31;//===============
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);
	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	for(i = 0, j=0 ; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_33x46_BLK_WHITE[j*2048]), WriteReadAddr, 1);
	}


	INDLED_ON;
}

