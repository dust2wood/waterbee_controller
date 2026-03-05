
//===========================================
#include "ethernet.h"
#include "ethernet2.h"
#include "gateway.h"
#include "ipaddr.h"
#include "port.h"
#include "subnet.h"



#define BLOCK_NO	77

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
	for(i = 0, j=0 ; i < 2 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Ethernet[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; i < 6 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Ethernet2[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(100);

	for( j=0; i < 8 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_IPaddr[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 10 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Gateway[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);
	for( j=0; i < 12 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Subnet[j*2048]), WriteReadAddr, 1);
	}

	for( j=0; i < 14 ; i++, j++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_Port[j*2048]), WriteReadAddr, 1);
	}


	INDLED_ON;
}

