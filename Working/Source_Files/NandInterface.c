#include "NandInterface.h"

void NAND_ReadICON(void)
{
	NAND_ReadData(imageBuffer, 0, 7); 
}

void NAND_ReadSNumber(void)
{
	NAND_ReadData(imageBuffer, 960, 4); 
}

void NAND_ReadMNumberW(void)
{
	NAND_ReadData(imageBuffer, 1024, 7); 
}

void NAND_ReadMNumberY(void)
{
	NAND_ReadData(imageBuffer, 1088, 7); 
}

void NAND_ReadLNumber(void)
{
	NAND_ReadData(imageBuffer, 1152, 20); 
}

void NAND_ReadData(uint8_t* buf, uint32_t startPage, uint32_t pageSize)
{
	NAND_ADDRESS WriteReadAddr = {0, 0, 0};
	uint32_t i = startPage;
	uint32_t j = 0;

	while(j < pageSize)
	{
		WriteReadAddr.Block = i / 64;
		WriteReadAddr.Page = i % 64;
		FSMC_NAND_ReadSmallPage(&(buf[j*NAND_PAGE_SIZE]),WriteReadAddr ,1);
		i++;
		j++;
	}
}

