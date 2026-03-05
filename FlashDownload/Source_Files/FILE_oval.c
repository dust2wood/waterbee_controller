
//===========================================
#include "back_oval_100x30.h"
#include "back_oval_140x42.h"

#include "back_oval2.h"
#include "back_oval2_c.h"
#include "back_oval2_hr.h"
#include "back_oval2_mgl.h"
#include "back_oval2_min.h"
#include "back_oval2_ntu.h"
#include "back_oval2_uscm.h"
#include "back_oval3_mgl.h"
#include "back_oval3_ntu.h"
#include "back_oval3_min.h"
#include "back_oval4.h"
#include "back_oval6.h"
#include "back_oval4_mgl.h"
#include "back_oval4_ntu.h"

#include "back_oval3.h"
#include "back_oval3_uscm.h"
#include "back_oval4_uscm.h"



#define BLOCK_NO	44

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
	for(i = 0, j=0 ; i < 3 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval_100x30[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 9 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval_140x42[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 20 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 31 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2_c[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 42 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2_hr[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 53 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2_mgl[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 64 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2_min[j*2048]), WriteReadAddr, 1);
	}



	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i = 0,j=0 ; i < 11 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2_ntu[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 22 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval2_uscm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 27 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval3_mgl[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 32 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval3_ntu[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 37 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval3_min[j*2048]), WriteReadAddr, 1);
	}

	// i ÇÏ³ª ºó´Ù...
	for( j=0, i=38; i < 44 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval4[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 47 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval6[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 53 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval4_mgl[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 59 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval4_ntu[j*2048]), WriteReadAddr, 1);
	}




	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+2;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for(i = 0,j=0 ; i < 5 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 10 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval3_uscm[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 16 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_back_oval4_uscm[j*2048]), WriteReadAddr, 1);
	}


	INDLED_ON;
}

