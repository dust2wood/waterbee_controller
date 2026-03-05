
//===========================================
#include "BELL_30x30.h"
#include "CAUTION_40x32.h"
#include "Cl_40x40.h"
#include "SD_30x30.h"
#include "WORKING_35x30.h"

#define BLOCK_ICON		0

void MAKE_file(void)
{
	int x1,x2, length;

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_ICON;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);


	//==========================================
	// WRITE TO NAND FLASH - Cl
	//==========================================
	INDLED_OFF;


	length = sizeof(CI_40x40);
	for (x1=0;x1<length;x1++) {
		imageBuffer[x1]=CI_40x40[x1];
	}

	length = sizeof(BELL_30x30);
	for (x2=0;x2<length; x1++,x2++) {
		imageBuffer[x1]=BELL_30x30[x2];
	}

	length = sizeof(SD_30x30);
	for (x2=0;x2<length; x1++,x2++) {
		imageBuffer[x1]=SD_30x30[x2];
	}

	length = sizeof(CAUTION_40x32);
	for (x2=0;x2<length; x1++,x2++) {
		imageBuffer[x1]=CAUTION_40x32[x2];
	}

	length = sizeof(IMG_WORKING_35x30);
	for (x2=0;x2<length; x1++,x2++) {
		imageBuffer[x1]=IMG_WORKING_35x30[x2];
	}	

	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	page = 10;	// 7.
	for(i = 0 ; i < page ; i++)
	{
		WriteReadAddr.Page = i;
		FSMC_NAND_WriteSmallPage(&(imageBuffer[i * 2048]), WriteReadAddr, 1);
	}
	INDLED_ON;
}

