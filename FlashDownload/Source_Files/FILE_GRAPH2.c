
//===========================================
#include "graph1_1.h"
#include "graph1_2.h"
#include "graph1_3.h"

#include "graph2_1.h"
#include "graph2_2.h"
#include "graph2_3.h"
#include "graph2_4.h"

#include "graph3_1.h"
#include "graph3_2.h"
#include "graph3_3.h"
#include "graph3_4.h"

#include "graph4_1.h"
#include "graph4_2.h"
#include "graph4_3.h"


#define BLOCK_NO	70

void MAKE_file(void)
{
	int i,j, length;


	/* Erase the NAND Block */
	WriteReadAddr.Block = 72;  //===============
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for( i=0, j=0; i < 13 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph1_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 26 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph1_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 39 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph1_3[j*2048]), WriteReadAddr, 1);
	}



	/* Erase the NAND Block */
	WriteReadAddr.Block = 73;  //===============
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for( i=0, j=0; i < 13 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph2_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 26 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph2_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 39 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph2_3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 52 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph2_4[j*2048]), WriteReadAddr, 1);
	}





	/* Erase the NAND Block */
	WriteReadAddr.Block = 74;  //===============
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for( i=0, j=0; i < 13 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph3_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 26 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph3_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 39 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph3_3[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 52 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph3_4[j*2048]), WriteReadAddr, 1);
	}





	/* Erase the NAND Block */
	WriteReadAddr.Block = 75;  //===============
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	for( i=0, j=0; i < 13 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph4_1[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 26 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph4_2[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 39 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_graph4_3[j*2048]), WriteReadAddr, 1);
	}




	INDLED_ON;
}

