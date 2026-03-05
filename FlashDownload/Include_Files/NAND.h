#ifndef NAND_H
#define NAND_H
#include "Main.h"


#ifndef NEW_BOARD
	#define NAND_FLASH_START_ADDR    ((uint32_t)0x80000000)//40000)
#else
	#define NAND_FLASH_START_ADDR    ((uint32_t)0x70040000)//40000)
#endif


#define CMD_AREA                   (uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<17)  /* A17 = ALE high */

#define DATA_AREA                  ((uint32_t)0x00000000)


#define   K9F1208U  		0x76EC
#define   K9F1G08U  	0xF1EC
#define   K9F2G08U  	0xDAEC
#define   K9F4G08U  	0xDCEC
#define   K9F8G08U  	0xD3EC
#define   K9GAG08U  	0xD5EC

/* FSMC NAND memory command */
#define	NAND_CMD_READ_1             ((uint8_t)0x00)
#define	NAND_CMD_READ_TRUE          ((uint8_t)0x30)

#define	NAND_CMD_RDCOPYBACK         ((uint8_t)0x00)
#define	NAND_CMD_RDCOPYBACK_TRUE    ((uint8_t)0x35)

#define NAND_COMM_SPAREREAD	        0x50 // NAND Read spare area

#define NAND_CMD_PAGEPROGRAM        ((uint8_t)0x80)
#define NAND_CMD_PAGEPROGRAM_TRUE   ((uint8_t)0x10)

#define NAND_CMD_COPYBACKPGM        ((uint8_t)0x85)
#define NAND_CMD_COPYBACKPGM_TRUE   ((uint8_t)0x10)
	
#define NAND_CMD_ERASE0             ((uint8_t)0x60)
#define NAND_CMD_ERASE1             ((uint8_t)0xD0)  

#define NAND_CMD_READID             ((uint8_t)0x90)	
#define NAND_CMD_STATUS             ((uint8_t)0x70)
#define NAND_CMD_RESET              ((uint8_t)0xFF)

#define NAND_CMD_CACHEPGM           ((uint8_t)0x80)
#define NAND_CMD_CACHEPGM_TRUE      ((uint8_t)0x15)

#define NAND_CMD_RANDOMIN           ((uint8_t)0x85)
#define NAND_CMD_RANDOMOUT          ((uint8_t)0x05)
#define NAND_CMD_RANDOMOUT_TRUE     ((uint8_t)0xE0)

#define NAND_CMD_CACHERD_START      ((uint8_t)0x00)
#define NAND_CMD_CACHERD_START2     ((uint8_t)0x31)
#define NAND_CMD_CACHERD_EXIT       ((uint8_t)0x34)

/* NAND memory status */
#define NAND_VALID_ADDRESS         ((uint32_t)0x00000100)
#define NAND_INVALID_ADDRESS       ((uint32_t)0x00000200)
#define NAND_TIMEOUT_ERROR         ((uint32_t)0x00000400)
#define NAND_BUSY                  ((uint32_t)0x00000000)
#define NAND_ERROR                 ((uint32_t)0x00000001)
#define NAND_READY                 ((uint32_t)0x00000040)

/* for K9F1G08 */
#define NAND_PAGE_SIZE             ((uint16_t)0x0800) /* 2 * 1024 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE            ((uint16_t)0x0040) /* 64 pages per block */
#define NAND_ZONE_SIZE             ((uint16_t)0x0400) /* 1024 Block per zone */
#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0040) /* last 64 bytes as spare area */
#define NAND_MAX_ZONE              ((uint16_t)0x0001) /* 1 zones of 1024 block */

#define BLOCK2PAGE(BlockNum) (BlockNum * NandInfo.pageperblock)
#define ADDR_1st_CYCLE(ADDR)       (uint8_t)((ADDR)& 0xFF)               /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00) >> 8)      /* 2nd addressing cycle */
#define ADDR_3rd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF0000) >> 16)   /* 3rd addressing cycle */
#define ADDR_4th_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF000000) >> 24) /* 4th addressing cycle */

#define ROW_ADDRESS (Address.Page + (Address.Block + (Address.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)


typedef struct _tagNand
{
	uint16_t id;
	uint32_t pagesize;
	uint32_t blocksize;
	uint32_t pageperblock;
	uint32_t blockcnt;
	uint32_t sparesize;
	uint32_t maxbadblockcnt;
	uint32_t rowaddrlen;
	uint32_t datastartblock; //file system area
	uint32_t datablockcnt;
	uint8_t eccbit;
}NANDTYPE;

typedef struct
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
}
NAND_IDTypeDef;
typedef struct 
{
  uint16_t Zone;
  uint16_t Block;
  uint16_t Page;
} 
NAND_ADDRESS;

extern const NANDTYPE nand_db[11];

void FSMC_NAND_ReadID(NAND_IDTypeDef* NAND_ID);
uint32_t FSMC_NAND_WriteSmallPage(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumPageToWrite);
uint32_t FSMC_NAND_ReadSmallPage (uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumPageToRead);
uint32_t FSMC_NAND_WriteSpareArea(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumSpareAreaTowrite);
uint32_t FSMC_NAND_ReadSpareArea(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumSpareAreaToRead);
uint32_t FSMC_NAND_EraseBlock(NAND_ADDRESS Address);
uint32_t FSMC_NAND_Reset(void);
uint32_t FSMC_NAND_GetStatus(void);
uint32_t FSMC_NAND_ReadStatus(void);
uint32_t FSMC_NAND_AddressIncrement(NAND_ADDRESS* Address);

void FSMC_NAND_Test(void);

#endif
















