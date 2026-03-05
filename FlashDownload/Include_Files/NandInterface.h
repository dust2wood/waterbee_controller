#ifndef NAND_INTERFACE_H
#define NAND_INTERFACE_H

extern uint8_t tmpBuffer[51200];

#define TEXTARRAY_DANGER						0
#define TEXTARRAY_CAUTION						1
#define TEXTARRAY_GOOD							2
#define TEXTARRAY_STANDBY					3
#define TEXTARRAY_RUN							4
#define TEXTARRAY_BLOCK						5
#define TEXTARRAY_INVALID						6
#define TEXTARRAY_LINEOPEN					7
#define TEXTARRAY_LINESHORT					9


void NAND_DISPLAY(void);
void NAND_WRITE_DATA2(void);
void NAND_WRITE_DATA(void);
void NAND_ERASE_Block(uint16_t blockSize);
void TempDrawPrototype(void);
void NAND_WRITE_DATA3(void);
void NAND_DISPLAY2(void);
void NAND_WRITE_DATA4(void);
void NAND_DISPLAY3(void);
void NAND_WRITE_DATA5(void);
void TEMPERASEBLOCK(void);
void NAND_WRITE2_DATA(void);
void NAND_WRITE2_DATA2(void);
void NAND_DISPLAY5(void);
void NAND_WRITEPhaseText(void);
void NAND_WRITETextArray(void);
void NAND_DisplayPhaseText(uint16_t x, uint16_t y, uint16_t DrawFlag);
void NAND_DisplayStatusText(uint16_t targetPoint, uint16_t DrawText);
void NAND_WRITEModeText(void);
void NAND_DisplayModeText(uint16_t DrawFlag);
#endif




