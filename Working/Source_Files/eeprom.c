#include "eeprom.h"


struct EEPROM_DATA EEpromdata;

void InitEeprom(void) {
return;
    while (ReadStatusRegisterEeprom() & 0x0001);

    WriteEnableEeprom();
    WriteStatusRegisterEeprom(STATUS_REG_EEPROM);
}
/*
void TEST_SPI_Send_byte0(void) {
    uint16_t data = 0x0F;

return;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, data);

}

uint16_t SPI_Send_byte0(uint16_t data) {
    uint16_t Read;

return;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, data);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    Read = SPI_I2S_ReceiveData(SPI2);

    return Read;

}

void WriteEnableEeprom0(void) {
return;
    EEPROM_CS_ON;
    SPI_Send_byte(WREN_EEPROM);
    EEPROM_CS_OFF;
}

void WriteDisableEeprom0(void) {
return;
    EEPROM_CS_ON;
    SPI_Send_byte(WRDI_EEPROM);
    EEPROM_CS_OFF;
}

uint16_t ReadStatusRegisterEeprom0(void) {
    uint16_t Out;

return;
    EEPROM_CS_ON;
    SPI_Send_byte(RDSR_EEPROM);
    Out = SPI_Send_byte(DUMMY_EEPROM);
    EEPROM_CS_OFF;
    return Out;
}

void WriteStatusRegisterEeprom0(uint16_t StatusRegister) {
return;
    EEPROM_CS_ON;
    SPI_Send_byte(WRSR_EEPROM);
    SPI_Send_byte(StatusRegister);
    EEPROM_CS_OFF;
    while (ReadStatusRegisterEeprom() & 0x0001);
}

uint16_t ReadEeprom(uint16_t Address) {
    uint16_t Out;

return;
    EEPROM_CS_ON;

    SPI_Send_byte(READ_EEPROM);
    SPI_Send_byte(ADDRESS_HIGH(Address));
    SPI_Send_byte(ADDRESS_LOW(Address));
    Out = SPI_Send_byte(DUMMY_EEPROM);

    EEPROM_CS_OFF;

    return Out;
}

void WriteEeprom(uint16_t Address, uint16_t Data) {
return;
    EEPROM_CS_ON;

    SPI_Send_byte(WRITE_EEPROM);
    SPI_Send_byte(ADDRESS_HIGH(Address));
    SPI_Send_byte(ADDRESS_LOW(Address));
    SPI_Send_byte(Data);

    EEPROM_CS_OFF;

    while (ReadStatusRegisterEeprom() & 0x0001);
}

void PWriteEeprom(uint16_t Page, uint16_t *StartAddr) {
    uint16_t Address, n;
    Address = Page * 64;

return;
    EEPROM_CS_ON;
    SPI_Send_byte(WRITE_EEPROM);
    SPI_Send_byte(ADDRESS_HIGH(Address));
    SPI_Send_byte(ADDRESS_LOW(Address));
    for (n = 0; n < 64; n++) {
        SPI_Send_byte(StartAddr[n]);
    }

    EEPROM_CS_OFF;

    while (ReadStatusRegisterEeprom() & 0x0001);
}

uint16_t MpuToEEprom(uint16_t *MsgData, uint16_t Start_Address, uint16_t Byte_Counter) {
    uint16_t i;
    uint16_t WriteSucess = 0;

return;
    for (i = 0; i < Byte_Counter; i++) {
        WriteEnableEeprom();
        WriteEeprom(Start_Address, *MsgData);
        Start_Address++;
        *MsgData++;
    }
    WriteSucess = 1;

    return WriteSucess;
}

uint16_t EEpromToMpu(uint16_t ReadStart_Address, uint16_t Byte_Counter) {
    uint16_t i;

return;
    for (i = 0; i < Byte_Counter; i++) {
        EEpromdata.ReadEEpromData_Buffer[i] = 0x00;
    }

    for (i = 0; i < Byte_Counter; i++) {

        EEpromdata.ReadEEpromData_Buffer[i] = ReadEeprom(ReadStart_Address);
        ReadStart_Address++;
    }

    return *(EEpromdata.ReadEEpromData_Buffer);
}

void EEpromData_BufferClear(void) {
    uint16_t i;

    for (i = 0; i < 120; i++) {
        EEpromdata.ReadEEpromData_Buffer[i] = 0;
        EEpromdata.WriteEEpromData_Buffer[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        EEpromdata.EEPROM_DATA_Confirm[i] = 0x00;
    }
}


void Clear_EEprom_Data0(void) {
    uint16_t i;

return;
    for (i = 0; i < 120; i++) {
        EEpromdata.ReadEEpromData_Buffer[i] = 0;
        EEpromdata.WriteEEpromData_Buffer[i] = 0;
    }
    MpuToEEprom(EEpromdata.WriteEEpromData_Buffer, 0, 120);
    EEpromToMpu(0, 120);
}
*/

//========================================================
#ifdef 0

#include <stm32f10x_i2c.h>

void AT24CXXX_EEPROM_Write(uint8_t Address, uint16_t addr, uint8_t* pBuffer,  uint16_t NumByteToWrite)
{
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	
	/* Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send slave address for write */
	I2C_Send7bitAddress(I2C1,Address, I2C_Direction_Transmitter);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, addr >> 8);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, addr & 0x00FF);
	
	/* Test on EV6 and clear it */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, *pBuffer);
	pBuffer++;
	NumByteToWrite--;
	
	/* While there is data to be written */
	while (NumByteToWrite--)
	{
	while ((I2C_GetLastEvent(I2C1) & 0x04) != 0x04);  /* Poll on BTF */
	
	/* Send the current byte */
	I2C_SendData(I2C1, *pBuffer);
	
	/* Point to the next byte to be written */
	pBuffer++;
	}
	
	/* Test on EV8_2 and clear it, BTF = TxE = 1, DR and shift registers are empty */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void AT24CXXX_EEPROM_Read(uint8_t Address, uint16_t addr, uint8_t* pBuffer,  uint16_t NumByteToWrite)
{
	__IO uint32_t temp;
	
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	
	/* Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send slave address for write */
	I2C_Send7bitAddress(I2C1,Address, I2C_Direction_Transmitter);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, addr >> 8);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, addr & 0x00FF);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	
	/* Test on EV5 and clear it */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send EEPROM address for read */
	I2C_Send7bitAddress(I2C1,Address, I2C_Direction_Receiver);
	
	/* Wait until ADDR is set */
	while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR));
	
	/* Clear ACK */
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	__disable_irq();
	
	/* Clear ADDR flag */
	temp = I2C1->SR2;
	
	/* Program the STOP */
	I2C_GenerateSTOP(I2C1, ENABLE);
	__enable_irq();
	
	while ((I2C_GetLastEvent(I2C1) & 0x0040) != 0x000040); /* Poll on RxNE */
	
	/* Read the data */
	*pBuffer = I2C_ReceiveData(I2C1);
	
	/* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
	while ((I2C1->CR1&0x200) == 0x200);
	
	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}
#endif
//====================================


#define ADDR_EEPROM		0xA0


uint16_t MpuToEEprom(uint16_t *MsgData, uint16_t Start_Address, uint16_t Byte_Counter) {
    uint16_t i,j;
    uint16_t WriteSucess = 0;


//	AT24CXXX_EEPROM_Write(ADDR_EEPROM, Start_Address, (uint8_t *)MsgData,  Byte_Counter);

    for (i = Start_Address, j=0; i < Start_Address+Byte_Counter; i++,j++) {
        EEpromdata.ReadEEpromData_Buffer[i] = EEpromdata.WriteEEpromData_Buffer[j];
    }
	Write_SetHistory();


/*    for (i = 0; i < Byte_Counter; i++) {
        WriteEnableEeprom();
        WriteEeprom(Start_Address, *MsgData);
        Start_Address++;
        *MsgData++;
    }*/
    WriteSucess = 1;

    return WriteSucess;
}

uint16_t EEpromToMpu(uint16_t ReadStart_Address, uint16_t Byte_Counter) {
    uint16_t i;


//    for (i = 0; i < Byte_Counter; i++) {
//        EEpromdata.ReadEEpromData_Buffer[i] = 0x00;
//    }

// юс╫ц╥н....
//    for (i = 0; i < 110; i++) {
//        EEpromdata.ReadEEpromData_Buffer[i] = EEpromdata.WriteEEpromData_Buffer[i];
//    }

//	AT24CXXX_EEPROM_Read(ADDR_EEPROM, ReadStart_Address, (uint8_t *)EEpromdata.ReadEEpromData_Buffer,  Byte_Counter);
Read_SetHistory();

/*    for (i = 0; i < Byte_Counter; i++) {

        EEpromdata.ReadEEpromData_Buffer[i] = ReadEeprom(ReadStart_Address);
        ReadStart_Address++;
    }
  */
    return *(EEpromdata.ReadEEpromData_Buffer);
}



void Clear_EEprom_Data(void) {
    uint16_t i;

    for (i = 0; i < 120; i++) {
        EEpromdata.ReadEEpromData_Buffer[i] = 0;
        EEpromdata.WriteEEpromData_Buffer[i] = 0;
    }
    MpuToEEprom(EEpromdata.WriteEEpromData_Buffer, 0, 120);
    EEpromToMpu(0, 120);
}
