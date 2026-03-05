#include "Main.h"

void NAND_ERASE_Block(uint16_t blockSize)
{
	uint16_t i = 0;
	uint16_t start = 0;
	NAND_ADDRESS WriteReadAddr = {0, 0, 0};
	WriteReadAddr.Block = start;
	WriteReadAddr.Page = 0;
	for(i = 0 ; i < blockSize ; ++i)
	{
		WriteReadAddr.Block = start + i;
		FSMC_NAND_EraseBlock(WriteReadAddr);
	}
}

