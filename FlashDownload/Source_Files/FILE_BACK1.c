
//===========================================
#include "WATERBEE_1.h"
#include "WATERBEE_2.h"
#include "WATERBEE_3.h"
#include "WATERBEE_4.h"
#include "WATERBEE_5.h"
#include "WATERBEE_6.h"

#define BLOCK_NO	41 

void MAKE_file(void)
{
	int i,j;

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	Deley_10msec(100);

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	INDLED_OFF;
	for(i=0,j=0 ; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_WATERBEE_1[i*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_WATERBEE_2[j*2048]), WriteReadAddr, 1);
	}


	Deley_10msec(100);

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i=0,j=0 ; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_WATERBEE_3[i*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 48 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_WATERBEE_4[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);


	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+2;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i=0,j=0 ; i < 24 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_WATERBEE_5[i*2048]), WriteReadAddr, 1);
	}

	for(j=0 ; i < 37 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_WATERBEE_6[j*2048]), WriteReadAddr, 1);
	}

	INDLED_ON;
}

