#include "NANDDisplay.h"


uint8_t imageBuffer[43200] = { 0 , };
uint16_t* GlobalMemory;

void DrawTextETC(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	const uint32_t imageData[13][3] = {{0, 70, 30},{2100, 40, 30},{3300, 70, 30},{5400, 40, 30},{6600, 40, 30},{7800, 70, 30},{9900, 70, 30},
										{12000, 70, 30},{14100, 70, 30},{16200, 70, 30},{18300, 70, 30},{20400, 70, 30},{985, 250, 60}};

	if(image > TEXT_ETC_F_RESET_MESSAGE)
		return;
	
	NAND_ReadTextETC(image);
	if(draw == DRAW_IMAGE_ENABLE)
		TFT_DrawImage(x, y, x+imageData[image][1], y+imageData[image][2], &GlobalMemory[imageData[image][0]], DRAW_NORMAL);
	else
		TFT_Fill(x, y, x+imageData[image][1], y+imageData[image][2], CYAN);
}

void DrawTextsize260(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	uint32_t imageStartAddr = 0;
	if(image > TEXT260_S2_PRIME_ING_Y)
		return;
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadTextsize260(image);
		imageStartAddr = image * 7800;
		imageStartAddr = imageStartAddr % 1024;
		TFT_DrawImage(x, y, x+260, y+30, &GlobalMemory[imageStartAddr], DRAW_NORMAL);
	}
	else
		TFT_Fill(x, y, x+260, y+30, CYAN);
}

void DrawTextsize180(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	uint32_t imageStartAddr = 0;
	if(image > TEXT180_S1_Current_Measure)
		return;
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadTextsize180(image);
		imageStartAddr = image * 5400;
		imageStartAddr = imageStartAddr % 1024;
		TFT_DrawImage(x, y, x+180, y+30, &GlobalMemory[imageStartAddr], DRAW_NORMAL);
	}
	else
		TFT_Fill(x, y, x+180, y+30, CYAN);	  
}

void DrawTextsize120(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	uint32_t imageStartAddr = 0;
	if(image > TEXT120_ZERO_ALARM)
		return;
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadTextsize120(image);
		imageStartAddr = image * 3600;
		imageStartAddr = imageStartAddr % 1024;
		TFT_DrawImage(x, y, x+120, y+30, &GlobalMemory[imageStartAddr], DRAW_NORMAL);
	}
	else
		TFT_Fill(x, y, x+120, y+30, CYAN);
}

void DrawTextsize96(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	uint32_t imageStartAddr = 0;
	//if(image > TEXT96_FILTER)
	//if(image > TEXT96_CI3)
	if(image > TEXT96_MANUAL_OPERATION)
		return;
	
	if( image == 0 )
	{
      image = 0; 
  }
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadTextsize96(image);
		imageStartAddr = image * 2880;
		imageStartAddr = imageStartAddr % 1024;
		TFT_DrawImage(x, y, x+96, y+30, &GlobalMemory[imageStartAddr], DRAW_NORMAL);
	}
	else
		TFT_Fill(x, y, x+96, y+30, CYAN);
}

void DrawTextsize55(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	if(image > TEXT55_CYCLE)
		return;
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadTextsize55(); 
		TFT_DrawImage(x, y, x+55, y+30, &GlobalMemory[1650 * image], DRAW_NORMAL); 
		                                                                                                                                  
	}
	else
		TFT_Fill(x, y, x+55, y+30, CYAN);
}

void DrawUnit(uint32_t x, uint32_t y, uint32_t image, uint32_t draw)
{
	if(image > UNIT_SEC)
		return;
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadUnit();
		TFT_DrawImage(x, y, x+30, y+30, &GlobalMemory[900 * image], DRAW_NORMAL);
	}
	else
		TFT_Fill(x, y, x+30, y+30, CYAN);
}

void DrawIcon(uint32_t icon, uint32_t draw)
{
	const uint16_t axis[4][3] = {{1,1,0},{390,1,1600},{435,1,3200},{70,110,4800}};

	if(icon > ICON_CAUTION)
		return;
	
	if(draw == DRAW_IMAGE_ENABLE)
	{
		NAND_ReadICON();
		TFT_DrawImage(axis[icon][0], axis[icon][1], axis[icon][0]+40, axis[icon][1]+40, &GlobalMemory[axis[icon][2]], DRAW_NORMAL);
	}
	else
		TFT_Fill(axis[icon][0], axis[icon][1], axis[icon][0]+40, axis[icon][1]+40, CYAN);
}

void DrawSmallNumber(uint32_t x, uint32_t y, char* str)
{
	uint32_t i = 0;
	NAND_ReadSNumber();
	while(str[i] != 0)
	{
		if(str[i] > 0x2F && str[i] < 0x3A)
			TFT_DrawImage(x, y, x+15, y+17, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 15 * 17 * 2]), DRAW_NORMAL);
		else if(str[i] == ' ')
			TFT_DrawImage(x, y, x+15, y+17, (uint16_t*)&(imageBuffer[13 * 15 * 17 * 2]), DRAW_NORMAL);
		else if(str[i] == '-')
			TFT_DrawImage(x, y, x+15, y+17, (uint16_t*)&(imageBuffer[10 * 15 * 17 * 2]), DRAW_NORMAL);
		else if(str[i] == ':')
			TFT_DrawImage(x, y, x+15, y+17, (uint16_t*)&(imageBuffer[11* 15 * 17 * 2]), DRAW_NORMAL);
		else if(str[i] == '.')
			TFT_DrawImage(x, y, x+15, y+17, (uint16_t*)&(imageBuffer[12 * 15 * 17 * 2]), DRAW_NORMAL);
		else
			return;
		x += 15;
		++i;
	}
}
void DrawMediumNumber(uint32_t x, uint32_t y, char* str, uint16_t color)
{
	uint32_t i = 0;
	uint16_t* buffer;
	
	if(color == YELLOW)
	{
		NAND_ReadMNumberY();
		buffer = (uint16_t*)YellowMinus;
	}
	else
	{
		NAND_ReadMNumberW();
		buffer = (uint16_t*)WhiteMinus;
	}
	while(str[i] != 0)
	{
		if(str[i] > 0x2F && str[i] < 0x3A)
			TFT_DrawImage(x, y, x+22, y+26, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 22 * 26 * 2]), DRAW_NORMAL);
		else if(str[i] == '.')
			TFT_DrawImage(x, y, x+22, y+26, (uint16_t*)&(imageBuffer[10 * 22 * 26 * 2]), DRAW_NORMAL);
		else if(str[i] == ' ')
			TFT_DrawImage(x, y, x+22, y+26, (uint16_t*)&(imageBuffer[11 * 22 * 26 * 2]), DRAW_NORMAL);
		else if(str[i] == '-')
			TFT_DrawImage(x, y, x+22, y+26, (uint16_t*)(buffer), DRAW_NORMAL);
		else
			return;
		x += 22;
		++i;
	}
}
void DrawLargeNumber(uint32_t x, uint32_t y, char* str)
{
	uint32_t i = 0;
	NAND_ReadLNumber();
	while(str[i] != 0)
	{
		if(str[i] > 0x2F && str[i] < 0x3A)
			TFT_DrawImage(x, y, x+36, y+47, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 36 * 47 * 2]), DRAW_NORMAL);
		else if(str[i] == '.')
			TFT_DrawImage(x, y, x+36, y+47, (uint16_t*)&(imageBuffer[10 * 36 * 47 * 2]), DRAW_NORMAL);
		else if(str[i] == ' ')
			TFT_DrawImage(x, y, x+36, y+47, (uint16_t*)&(imageBuffer[11 * 36 * 47 * 2]), DRAW_NORMAL);
		else
			return;
		x += 36;
		++i;
	}
}

void DrawLineRectangle(uint32_t x, uint32_t y, uint32_t xEnd, uint32_t yEnd, uint16_t color)
{
	if(yEnd < 2 || xEnd < 2)
		return;
	
	TFT_Fill(x, y, xEnd, y+1, color);
	TFT_Fill(x, y, x+1, yEnd, color);
	TFT_Fill(xEnd-1, y, xEnd, yEnd, color);
	TFT_Fill(x, yEnd-1, xEnd, yEnd, color);
}

void DrawBottomLine(void)
{
	TFT_Fill(169, 235, 171, 272, BROWN);
	TFT_Fill(309, 235, 311, 272, BROWN);
}

void ClearTitleArea(void)
{
	TFT_Fill(0, 0, 480, 41, CYAN);
}
void ClearTitle(void)
{
	TFT_Fill(110, 0, 290, 41, CYAN);
}
void ClearViewArea(void)
{
	TFT_Fill(0, 43, 480, 234, CYAN);
}
void ClearBottomArea(void)
{
	TFT_Fill(0, 234, 480, 272, CYAN);
}
void TFTDrawImage(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t* Image, uint16_t Flag)
{
    uint32_t i, j, n = (xEnd - xStart) * (yEnd - yStart);
	uint32_t x =  xEnd - xStart , y = yEnd - yStart;

	TFT_WR_REG(SET_CMD_ADDR);	
	TFT_WR_Data(xStart>>8);	    
	TFT_WR_Data(xStart&0x00ff);
	TFT_WR_Data((xEnd-1)>>8);	    
	TFT_WR_Data((xEnd-1)&0x00ff);
	
    TFT_WR_REG(SET_PAGE_ADDR);	
	TFT_WR_Data(yStart>>8);	    
	TFT_WR_Data(yStart&0x00ff);
	TFT_WR_Data((yEnd-1)>>8);	    
	TFT_WR_Data((yEnd-1)&0x00ff);
	
	TFT_WR_REG(WRITE_MEM_START);
	
	
	if(Flag == DRAW_REVERSE)
	{
		for(i = 0 ; i < n ; ++i)
			TFT_WR_Data(~Image[i]);
	}
	else if(Flag == DRAW_ARROW_CROSS)
	{
		for(i = 0 ; i < y ; ++i)
		{
			for(j = x ; j > 0 ; --j)
			{
				TFT_WR_Data(Image[(i*x)+j]);
			}
		}
	}
	else if(Flag == (DRAW_REVERSE | DRAW_ARROW_CROSS) )
	{
		for(i = 0 ; i < y ; ++i)
		{
			for(j = x ; j > 0 ; --j)
			{
				TFT_WR_Data( ~(Image[(i*x)+j]) );
			}
		}
	}
	else
	{
		for(i = 0 ; i < n ; ++i)
			TFT_WR_Data(Image[i]);
	}
}

