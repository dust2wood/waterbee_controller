
//===========================================
#include "2ch_cl.c"
#include "2ch_ec.c"
#include "2ch_home_number.c"
#include "2ch_ph.c"
#include "2ch_temp_number.c"
#include "2ch_tu.c"
#include "2ch_small_unit_ec.c"
#include "2ch_small_unit_ph.c"
#include "2ch_unit_cl.c"
#include "2ch_unit_ec.c"
#include "2ch_unit_ph.c"
#include "2ch_unit_tu.c"

#include "2ch_set_cl1.c"
#include "2ch_set_cl2.c"
#include "2ch_set_ec1.c"
#include "2ch_set_ec2.c"
#include "2ch_set_ph1.c"
#include "2ch_set_ph2.c"
#include "2ch_set_tu1.c"
#include "2ch_set_tu2.c"

#include "4ch_cl.c"
#include "4ch_ec.c"
#include "4ch_home_number.c"
#include "4ch_ph.c"
#include "4ch_temp_number.c"
#include "4ch_tu.c"
#include "4ch_small_unit_ec.c"
#include "4ch_small_unit_ph.c"
#include "4ch_unit_cl.c"
#include "4ch_unit_ec.c"
#include "4ch_unit_ph.c"
#include "4ch_unit_tu.c"

#include "4ch_set_cl1.c"
#include "4ch_set_cl2.c"
#include "4ch_set_ec1.c"
#include "4ch_set_ec2.c"
#include "4ch_set_ph1.c"
#include "4ch_set_ph2.c"
#include "4ch_set_tu1.c"
#include "4ch_set_tu2.c"



#define BLOCK_NO	80

void MAKE_file(void)
{
	int page,j, length;

	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	page = 0;

	Deley_10msec(100);

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);

	//FSMC_NAND_WriteSmallPage(NandBuffer,WriteReadAddr ,1);
	//==========================================
	// WRITE TO NAND FLASH
	//==========================================
	INDLED_OFF;
	for(j=0 ; page < 3 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_cl[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 6 ; page++, j++)
	{
		WriteReadAddr.Page = page;
			FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_ec[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(100);

	for( j=0; page < 18 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_home_number[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 21 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_ph[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 25 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_temp_number[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 27 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_tu[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 29 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_small_unit_ec[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 31 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_small_unit_ph[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 33 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_unit_cl[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 36 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_unit_ec[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 38 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_unit_ph[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 40 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_unit_tu[j*2048]), WriteReadAddr, 1);
	}

	//====================================
	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+1;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	page = 0;

	Deley_10msec(100);

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);



	for( j=0; page < 8 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_cl1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 16 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_cl2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 24 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_ec1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 32 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_ec2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 40 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_ph1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 48 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_ph2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 56 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_tu1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 64 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_2ch_set_tu2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);


	//====================================
	//====================================
	//====================================
	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+2;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	page = 0;

	Deley_10msec(100);

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);


	for(page = 0, j=0 ; page < 3 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_cl[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 6 ; page++, j++)
	{
		WriteReadAddr.Page = page;
			FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_ec[j*2048]), WriteReadAddr, 1);
	}
	Deley_10msec(100);

	for( j=0; page < 16 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_home_number[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 19 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_ph[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 22 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_temp_number[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 24 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_tu[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 26 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_small_unit_ec[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 28 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_small_unit_ph[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 30 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_unit_cl[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 33 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_unit_ec[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 35 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_unit_ph[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 37 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_unit_tu[j*2048]), WriteReadAddr, 1);
	}

	//====================================
	/* Erase the NAND Block */
	WriteReadAddr.Block = BLOCK_NO+3;
	WriteReadAddr.Page = 0;
	WriteReadAddr.Zone = 0;

	page = 0;

	Deley_10msec(100);

	FSMC_NAND_EraseBlock(WriteReadAddr);
	Deley_10msec(100);


	Deley_10msec(100);

	for( j=0; page < 4 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_cl1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 8 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_cl2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 12 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_ec1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 16 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_ec2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 20 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_ph1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 24 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_ph2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 28 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_tu1[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);

	for( j=0; page < 32 ; page++, j++)
	{
		WriteReadAddr.Page = page;
		FSMC_NAND_WriteSmallPage((uint8_t *)&(IMG_4ch_set_tu2[j*2048]), WriteReadAddr, 1);
	}

	Deley_10msec(100);





	INDLED_ON;
}

