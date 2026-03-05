#include "eeprom.h"


struct EEPROM_DATA  EEpromdata;


void InitEeprom(void)
{	
	while( ReadStatusRegisterEeprom() & 0x0001);
	
	WriteEnableEeprom();
	WriteStatusRegisterEeprom(STATUS_REG_EEPROM);
}


void TEST_SPI_Send_byte (void)
{	
   uint16_t data = 0x0F;
	
   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
   SPI_I2S_SendData(SPI2,data);	
     
}

uint16_t SPI_Send_byte (uint16_t data)
{	
   uint16_t Read;
	
   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
   SPI_I2S_SendData(SPI2,data);
	
   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);
   Read = SPI_I2S_ReceiveData(SPI2);
 	
   return Read;  
   
}


void WriteEnableEeprom(void)
{
	EEPROM_CS_ON;
	SPI_Send_byte(WREN_EEPROM);
	EEPROM_CS_OFF;
}

void WriteDisableEeprom(void)
{
	EEPROM_CS_ON;
	SPI_Send_byte(WRDI_EEPROM);
	EEPROM_CS_OFF;
}

uint16_t ReadStatusRegisterEeprom(void)
{
	uint16_t Out;
	EEPROM_CS_ON;
	SPI_Send_byte(RDSR_EEPROM);
	Out = SPI_Send_byte(DUMMY_EEPROM);
	EEPROM_CS_OFF;
	return Out;
}


void WriteStatusRegisterEeprom(uint16_t StatusRegister)
{
	EEPROM_CS_ON;
	SPI_Send_byte(WRSR_EEPROM);
  SPI_Send_byte(StatusRegister);
	EEPROM_CS_OFF;
	while(ReadStatusRegisterEeprom() & 0x0001);
}



uint16_t ReadEeprom(uint16_t Address)
{
	uint16_t Out;

	EEPROM_CS_ON;

	SPI_Send_byte(READ_EEPROM);
	SPI_Send_byte(ADDRESS_HIGH(Address));
	SPI_Send_byte(ADDRESS_LOW(Address));
	Out = SPI_Send_byte(DUMMY_EEPROM);

	EEPROM_CS_OFF;

	return Out;
}


void WriteEeprom(uint16_t Address, uint16_t Data)
{
	EEPROM_CS_ON;

	SPI_Send_byte(WRITE_EEPROM);            
	SPI_Send_byte(ADDRESS_HIGH(Address));     
	SPI_Send_byte(ADDRESS_LOW(Address));
	SPI_Send_byte(Data);                    

	EEPROM_CS_OFF;

	while(ReadStatusRegisterEeprom() & 0x0001);
}

void PWriteEeprom(uint16_t Page, uint16_t *StartAddr)
{
	uint16_t Address,n;
	Address = Page * 64;

	EEPROM_CS_ON;
	SPI_Send_byte(WRITE_EEPROM);
	SPI_Send_byte(ADDRESS_HIGH(Address));
	SPI_Send_byte(ADDRESS_LOW(Address));
	for(n=0;n<64;n++){
		SPI_Send_byte(StartAddr[n]);
	}

	EEPROM_CS_OFF;

	while(ReadStatusRegisterEeprom() & 0x0001);
}



uint16_t MpuToEEprom (uint16_t *MsgData , uint16_t Start_Address , uint16_t Byte_Counter)
{
   uint16_t i;
   uint16_t WriteSucess =0;

   for(i=0;i<Byte_Counter;i++){
		        WriteEnableEeprom();
		    	  WriteEeprom(Start_Address, *MsgData);
			      Start_Address++;
			      *MsgData++;
		     }
   WriteSucess = 1;

   return WriteSucess;
}

uint16_t EEpromToMpu (uint16_t ReadStart_Address , uint16_t Byte_Counter)
{
   uint16_t i;

   for(i=0;i<Byte_Counter; i++){
       EEpromdata.ReadEEpromData_Buffer[i] = 0x00;
   }

   for(i=0;i<Byte_Counter;i++){

			EEpromdata.ReadEEpromData_Buffer[i] = ReadEeprom(ReadStart_Address);
			ReadStart_Address++;
		}

   return *(EEpromdata.ReadEEpromData_Buffer);
}



void EEpromData_BufferClear (void)
{
   uint16_t i ;
	
	for(i = 0 ; i < 120 ; i ++)
	{
    EEpromdata.ReadEEpromData_Buffer[i] = 0;	
    EEpromdata.WriteEEpromData_Buffer[i] = 0;		
  }		
	for(i = 0 ; i < 4 ; i++)
	{
    EEpromdata.EEPROM_DATA_Confirm[i] = 0x00;
  }
}


void Clear_EEprom_Data (void)
{
	 uint16_t i ;
   for(i = 0 ; i < 120 ; i ++)
  	{
      EEpromdata.ReadEEpromData_Buffer[i] = 0;	
      EEpromdata.WriteEEpromData_Buffer[i] = 0;		
    }	
		MpuToEEprom(EEpromdata.WriteEEpromData_Buffer,0,120);
		EEpromToMpu(0,120);
}

