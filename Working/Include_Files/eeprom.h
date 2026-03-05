#ifndef eeprom_H
#define eeprom_H

#include "Main.h"


#define WREN_EEPROM			0x0006
#define WRDI_EEPROM			0x0004
#define RDSR_EEPROM			0x0005
#define WRSR_EEPROM			0x0001
#define READ_EEPROM			0x0003
#define WRITE_EEPROM		0x0002
#define DUMMY_EEPROM		0xFFFF
#define STATUS_REG_EEPROM	0x0000

#define ADDRESS_HIGH(w)	 ((w>>8) & 0x00ff)
#define ADDRESS_LOW(w)	 ((w) & 0x00ff)

#define EEPROM_CS_ON    GPIO_ResetBits(GPIO_SPI_CS, GPIO_SPI_NCS_PIN);
#define EEPROM_CS_OFF   GPIO_SetBits(GPIO_SPI_CS, GPIO_SPI_NCS_PIN);

void TEST_SPI_Send_byte (void);
void InitEeprom(void);
uint16_t SPI_Send_byte (uint16_t data);
//uint16_t SPI_Receive_byte(void);
void WriteEnableEeprom(void);
void WriteDisableEeprom(void);
uint16_t ReadStatusRegisterEeprom(void);
void WriteStatusRegisterEeprom(uint16_t StatusRegister);
uint16_t ReadEeprom(uint16_t Address);
void WriteEeprom(uint16_t Address, uint16_t Data);
void PWriteEeprom(uint16_t Page, uint16_t *StartAddr);
uint16_t MpuToEEprom (uint16_t *MsgData , uint16_t Start_Address , uint16_t Byte_Counter);
uint16_t EEpromToMpu (uint16_t ReadStart_Address , uint16_t Byte_Counter);
void EEpromData_BufferClear (void);
void Clear_EEprom_Data (void);

#endif


