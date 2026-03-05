#ifndef NAND_INTERFACE_H
#define NAND_INTERFACE_H
#include "Main.h"

void NAND_ReadTextETC(uint32_t image);
void NAND_ReadTextsize260(uint32_t image);
void NAND_ReadTextsize180(uint32_t image);
void NAND_ReadTextsize120(uint32_t image);
void NAND_ReadTextsize96(uint32_t image);
void NAND_ReadTextsize55(void);
void NAND_ReadICON(void);
void NAND_ReadUnit(void);
void NAND_ReadSNumber(void);
void NAND_ReadMNumberW(void);
void NAND_ReadMNumberY(void);
void NAND_ReadLNumber(void);
void NAND_ReadData(uint8_t* buf, uint32_t startPage, uint32_t pageSize);
#endif




