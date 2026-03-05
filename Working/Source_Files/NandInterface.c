#include "NandInterface.h"
				 /*
void NAND_ReadTextETC(uint32_t image)
{
	if(image < 12)
		NAND_ReadData(imageBuffer, 832, 22);
	else
		
	NAND_ReadData(imageBuffer, 853, 17);
}
				   
void NAND_ReadTextsize260(uint32_t image)
{
	uint32_t page = image * 15600;
	page = page / 2048;
	page += (64 * 20);
	NAND_ReadData(imageBuffer, page, 9);
}
				   
void NAND_ReadTextsize180(uint32_t image)
{
	uint32_t page = image * 10800;
	page = page / 2048;
	page += (64 * 10);
	NAND_ReadData(imageBuffer, page, 7);
}

void NAND_ReadTextsize120(uint32_t image)
{
	uint32_t page = image * 7200;
	page = page / 2048;
	page += (64 * 8);     // 64pages = 1block 
	NAND_ReadData(imageBuffer, page, 5);
}

void NAND_ReadTextsize96(uint32_t image)
{
	uint32_t page = image * 5760;       
	page = page / 2048;
	page += (64 * 3);                         
	NAND_ReadData(imageBuffer, page, 4);
}

void NAND_ReadTextsize55(void)
{
	NAND_ReadData(imageBuffer, 128, 18);
}

void NAND_ReadUnit(void)
{
	NAND_ReadData(imageBuffer, 64, 8);  
}
*/
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

