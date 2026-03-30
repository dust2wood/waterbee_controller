#include "Main.h"


void NAND_Read_SET_ICON(unsigned short no)
{
	NAND_ReadData(imageBuffer, 0, 7); 
}


//=========================================================

//extern uint8_t imageBuffer[40960];
extern uint8_t imageBuffer[47104];
//extern uint8_t imageBuffer[48000];


//=========================================

void NAND_ReadData2(uint8_t* buf, uint32_t BLOCK, uint32_t startPage, uint32_t pageSize)
{
	NAND_ADDRESS WriteReadAddr = {0, 0, 0};
	uint32_t i = startPage;
	uint32_t j = 0;

	while(j < pageSize)
	{
		WriteReadAddr.Block = BLOCK+(i/64);
		WriteReadAddr.Page = (i%64);

		FSMC_NAND_ReadSmallPage(&(buf[j*NAND_PAGE_SIZE]),WriteReadAddr ,1);
		i++;
		j++;
	}
}

//=========================================
// NUMERIC
//=========================================

void NEMERIC_8X10_BK_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 8 * 10 * 2;
	NAND_ReadData2(imageBuffer, 30, 60 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+8, y+10, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_8X10_BU_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 8 * 10 * 2;
	NAND_ReadData2(imageBuffer, 30, 0 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+8, y+10, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_8X10_BU_G(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 8 * 10 * 2;
	NAND_ReadData2(imageBuffer, 30, 2 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+8, y+10, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

//========================================

void NEMERIC_12X16_BK_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 12 * 16 * 2;
	NAND_ReadData2(imageBuffer, 30, 4 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+12, y+16, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_12X16_BK_G(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 12 * 16 * 2;
	NAND_ReadData2(imageBuffer, 30, 7 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+12, y+16, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_12X16_BU_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 12 * 16 * 2;
	NAND_ReadData2(imageBuffer, 30, 10 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+12, y+16, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_12X16_BU_G(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 12 * 16 * 2;
	NAND_ReadData2(imageBuffer, 30, 13 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+12, y+16, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

//====================================
void NEMERIC_18X23_BK_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 18 * 23 * 2;
	NAND_ReadData2(imageBuffer, 30, 20 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+18, y+23, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_18X23_BK_G(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 18 * 23 * 2;
	NAND_ReadData2(imageBuffer, 30, 28 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+18, y+23, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}


//====================================
void NEMERIC_23X28_BK_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 23 * 28 * 2;
	NAND_ReadData2(imageBuffer, 30, 36 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+23, y+28, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

void NEMERIC_23X28_BK_G(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 23 * 28 * 2;
	NAND_ReadData2(imageBuffer, 30, 48 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+23, y+28, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}


//====================================
void NEMERIC_33X46_BK_W(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	uint32_t off = (uint32_t)data * 33 * 46 * 2;
	NAND_ReadData2(imageBuffer, 31, 0 + off/NAND_PAGE_SIZE, 2);
	TFT_DrawNumeric(x, y, x+33, y+46, (uint16_t *)(&imageBuffer[off%NAND_PAGE_SIZE]), color);
}

//=====================================
void DrawTinyNumber(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
//    NAND_ReadSNumber();
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A) {
			if (color==BLACK)	NEMERIC_8X10_BK_W(x,y,str[i]-0x30, color);
			else 				NEMERIC_8X10_BU_W(x,y,str[i]-0x30, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 15 * 17 * 2]), color);
        }
		else if (str[i] == ' ')				{
			if (color==BLACK)	NEMERIC_8X10_BK_W(x,y,13, color);
			else 				NEMERIC_8X10_BU_W(x,y,13, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[13 * 15 * 17 * 2]), color);
        }
		else if (str[i] == '-')				{
			if (color==BLACK)	NEMERIC_8X10_BK_W(x,y,10, color);
			else             	NEMERIC_8X10_BU_W(x,y,10, color);
			//TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[10 * 15 * 17 * 2]), color);
        }
		else if (str[i] == ':')				{
			if (color==BLACK)	NEMERIC_8X10_BK_W(x,y,11, color);
			else             	NEMERIC_8X10_BU_W(x,y,11,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[11 * 15 * 17 * 2]), color);
        }
		else if (str[i] == '.')				{
			if (color==BLACK)	NEMERIC_8X10_BK_W(x,y,12, color);
			else             	NEMERIC_8X10_BU_W(x,y,12,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[12 * 15 * 17 * 2]), color);
        }
		else if (str[i] == 'C')				{
			if (color==BLACK)	NEMERIC_8X10_BK_W(x,y,14, color);
			else             	NEMERIC_8X10_BU_W(x,y,14,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[14 * 15 * 17 * 2]), color); // �µ� ǥ�� �߰�����.
		}
        else
            return;
        x += 12; // 15
        i++;
		if (i>10) return;

    }
}

void DrawSmallNumber(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
//    NAND_ReadSNumber();
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
			NEMERIC_12X16_BK_W(x,y,str[i]-0x30, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 15 * 17 * 2]), color);
        else if (str[i] == ' ')
			NEMERIC_12X16_BK_W(x,y,13, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[13 * 15 * 17 * 2]), color);
        else if (str[i] == '-')
            NEMERIC_12X16_BK_W(x,y,10, color);
			//TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[10 * 15 * 17 * 2]), color);
        else if (str[i] == ':')
            NEMERIC_12X16_BK_W(x,y,11,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[11 * 15 * 17 * 2]), color);
        else if (str[i] == '.')
            NEMERIC_12X16_BK_W(x,y,12,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[12 * 15 * 17 * 2]), color);
        else if (str[i] == 'C')
            NEMERIC_12X16_BK_W(x,y,14,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[14 * 15 * 17 * 2]), color); // �µ� ǥ�� �߰�����.
        else
            return;
        x += 12; // 15
        i++;
		if (i>10) return;
    }
}


void DrawMediumNumber(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
    uint16_t* buffer;


    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
			NEMERIC_18X23_BK_W(x,y,str[i]-0x30,color);
            //TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 22 * 26 * 2]), DRAW_REVERSE);
        else if (str[i] == '.')
			NEMERIC_18X23_BK_W(x,y,10,color);
            //TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*)&(imageBuffer[10 * 22 * 26 * 2]), DRAW_REVERSE);
        else if (str[i] == ' ')
			NEMERIC_18X23_BK_W(x,y,11,color);
            //TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*)&(imageBuffer[11 * 22 * 26 * 2]), DRAW_REVERSE);
        else if (str[i] == '-')
			NEMERIC_18X23_BK_W(x,y,12,color);	// ���� ,,,�߰��ؾ���
            //TFT_DrawImage(x, y, x + 22, y + 26, (uint16_t*) (buffer), DRAW_REVERSE);
        else
            return;
        x += 18;
        i++;
		if (i>10) return;
    }
}

void DrawXLargeNumber(uint32_t x, uint32_t y, char* str) {
    uint32_t i = 0;

    //NAND_ReadLNumber();

    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
			NEMERIC_33X46_BK_W(x,y,str[i]-0x30,DRAW_NORMAL);
            //TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 36 * 47 * 2]), DRAW_REVERSE);
        else if (str[i] == '.')
			NEMERIC_33X46_BK_W(x,y,10,DRAW_NORMAL);
            //TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[10 * 36 * 47 * 2]), DRAW_REVERSE);
        else if (str[i] == ' ')
			NEMERIC_33X46_BK_W(x,y,11,DRAW_NORMAL);
            //TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[11 * 36 * 47 * 2]), DRAW_REVERSE);
        else
            return;
        x += 42;
        i++;
		if (i>10) return;
    }
}


void DrawLargeNumber(uint32_t x, uint32_t y, char* str) {
    uint32_t i = 0;

    //NAND_ReadLNumber();

    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
			NEMERIC_23X28_BK_W(x,y,str[i]-0x30,DRAW_NORMAL);
            //TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 36 * 47 * 2]), DRAW_REVERSE);
        else if (str[i] == '.')
			NEMERIC_23X28_BK_W(x,y,10,DRAW_NORMAL);
            //TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[10 * 36 * 47 * 2]), DRAW_REVERSE);
        else if (str[i] == ' ')
			NEMERIC_23X28_BK_W(x,y,11,DRAW_NORMAL);
            //TFT_DrawImage(x, y, x + 36, y + 47, (uint16_t*)&(imageBuffer[11 * 36 * 47 * 2]), DRAW_REVERSE);
        else
            return;
        x += 23;
        i++;
		if (i>10) return;

    }
}


//=========================================
// FILE_OVAL
//=========================================

void Draw_Back_Oval_100x30(unsigned int x, unsigned int y)
{
// x=190, y=152
	NAND_ReadData2(	imageBuffer, 44,0,3);
	TFT_DrawImage(x, y,  x+100, y+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval_140x42(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 44,3,6);
	TFT_DrawImage(x, y,  x+140, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 44,9,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2_c(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 44,20,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2_hr(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 44,31,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2_mgl(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 44,42,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2_min(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 44,53,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2_ntu(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,0,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval2_uscm(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,11,11);
	TFT_DrawImage(x, y,  x+260, y+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


void Draw_Back_Oval3_mgl(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,22,5);
	TFT_DrawImage(x, y,  x+127, y+39,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval3_ntu(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,27,5);
	TFT_DrawImage(x, y,  x+127, y+39,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval3_min(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,32,5);
	TFT_DrawImage(x, y,  x+127, y+39,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval3(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 46,0,5);
	TFT_DrawImage(x, y,  x+127, y+39,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval3_uscm(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 46,5,5);
	TFT_DrawImage(x, y,  x+127, y+39,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


void Draw_Back_Oval4_uscm(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 46,10,6);
	TFT_DrawImage(x, y,  x+180, y+32,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


void Draw_Back_Oval4(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,38,11);
	TFT_DrawImage(x, y,  x+180, y+32,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval6(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 45,44,11);
	TFT_DrawImage(x, y,  x+90, y+32,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval4_mgl(unsigned int x, unsigned int y)	// used in ANALOG menu
{
	NAND_ReadData2(	imageBuffer, 45,47,6);
	TFT_DrawImage(x, y,  x+180, y+32,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void Draw_Back_Oval4_ntu(unsigned int x, unsigned int y)	// used in ANALOG menu
{
	NAND_ReadData2(	imageBuffer, 45,53,6);
	TFT_DrawImage(x, y,  x+180, y+32,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=========================================
// FILE_SET1
//=========================================
// IMG_set_set1
void display_set_set(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,0,2);
	else 			NAND_ReadData2(	imageBuffer, 50,2,2);
	TFT_DrawImage(  35, 229, 35+34, 229+34, (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_set_alarm2
void display_set_alarm(unsigned char select)
{
	if (select==0)  NAND_ReadData2(	imageBuffer, 50,4,2);
	else 			NAND_ReadData2(	imageBuffer, 50,6,2);
	TFT_DrawImage(  317, 229, 317+34, 229+34, (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_set_cal1
void display_set_cal(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,8,2);
	else 			NAND_ReadData2(	imageBuffer, 50,10,2);
	TFT_DrawImage(  129, 229, 129+34, 229+34, (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_set_diag2
void display_set_diag(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,12,2);
	else 			NAND_ReadData2(	imageBuffer, 50,14,2);
	TFT_DrawImage(  411, 229, 411+34, 229+34, (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set_trand2
void display_set_trand(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,16,2);
	else 			NAND_ReadData2(	imageBuffer, 50,18,2);
	TFT_DrawImage(  222, 229, 222+34, 229+34, (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//===================================

// IMG_set2_analog1
void display_set2_analog(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,20,3);
	else 			NAND_ReadData2(	imageBuffer, 50,23,3);
	TFT_DrawImage(  19, 225, 19+80, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set2_calib1
void display_set2_calib(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,26,3);
	else 			NAND_ReadData2(	imageBuffer, 50,29,3);
	TFT_DrawImage(   381, 225, 381+80, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set2_comm1
void display_set2_comm(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,32,3);
	else 			NAND_ReadData2(	imageBuffer, 50,35,3);
	TFT_DrawImage(  110, 225, 110+80, 225+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set2_relay1
void display_set2_relay(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,38,3);
	else 			NAND_ReadData2(	imageBuffer, 50,41,3);
	TFT_DrawImage( 200, 225, 200+80, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set2_time1
void display_set2_time(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 50,44,3);
	else 			NAND_ReadData2(	imageBuffer, 50,47,3);
	TFT_DrawImage(  291, 225, 291+80, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=====================================================


// IMG_set3_log1
void display_set3_log(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,0,3);
	else 			NAND_ReadData2(	imageBuffer, 51,3,3);
	TFT_DrawImage(  359, 225, 359+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set3_span1
void display_set3_span(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,6,3);
	else 			NAND_ReadData2(	imageBuffer, 51,9,3);
	TFT_DrawImage( 133, 225, 133+100, 225+30, (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set3_temp1
void display_set3_temp(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,12,3);
	else 			NAND_ReadData2(	imageBuffer, 51,15,3);
	TFT_DrawImage(  246, 225, 246+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_set3_zero1
void display_set3_zero(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,18,3);
	else 			NAND_ReadData2(	imageBuffer, 51,21,3);
	TFT_DrawImage(  20, 225, 20+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// ���۱���
void display_set3_buff(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,48,3);
	else 			NAND_ReadData2(	imageBuffer, 51,51,3);
	TFT_DrawImage(  20, 225, 20+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

//=============================================
// IMG_set4_filter1
void display_set4_filter(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,24,3);
	else 			NAND_ReadData2(	imageBuffer, 51,27,3);
	TFT_DrawImage(   246, 225, 246+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set4_init1
void display_set4_init(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,30,3);
	else 			NAND_ReadData2(	imageBuffer, 51,33,3);
	TFT_DrawImage(  359, 225, 359+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set4_offset1
void display_set4_offset(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,36,3);
	else 			NAND_ReadData2(	imageBuffer, 51,39,3);
	TFT_DrawImage(  133, 225, 133+100, 225+30, (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set4_slope1
void display_set4_slope(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 51,42,3);
	else 			NAND_ReadData2(	imageBuffer, 51,45,3);
	TFT_DrawImage(   20, 225, 20+100, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=========================================
// FILE_SET2
//=========================================
// IMG_set5_alarm1
void display_set5_alarm(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 52,0,4);
	else 			NAND_ReadData2(	imageBuffer, 52,4,4);
	TFT_DrawImage(  176, 225, 176+130, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set5_clean1
void display_set5_clean(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 52,8,8);
	else 			NAND_ReadData2(	imageBuffer, 52,12,8);
	TFT_DrawImage(   20, 225, 20+130, 225+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set5_relay1
void display_set5_relay(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 52,16,4);
	else 			NAND_ReadData2(	imageBuffer, 52,20,4);
	TFT_DrawImage(  330, 225, 330+130, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//============================================

// IMG_set6_act_set1
void display_set6_set(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 52,38,7);
	else 			NAND_ReadData2(	imageBuffer, 52,45,7);
	TFT_DrawImage( 20, 225, 20+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set6_act_passive1
void display_set6_passive(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 52,24,7);
	else 			NAND_ReadData2(	imageBuffer, 52,31,7);
	TFT_DrawImage(  249, 225, 249+212, 225+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//============================================
// IMG_set7_autowash1
void display_set7_autowash(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 53,0,7);
	else 			NAND_ReadData2(	imageBuffer, 53,7,7);
	TFT_DrawImage(   20, 225, 20+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set7_manwash1
void display_set7_manwash(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 53,14,7);
	else 			NAND_ReadData2(	imageBuffer, 53,21,7);
	TFT_DrawImage(  249, 225, 249+212, 225+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//============================================

// IMG_set8_autoalarm1
void display_set8_autoalarm(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 53,28,7);
	else 			NAND_ReadData2(	imageBuffer, 53,35,7);
	TFT_DrawImage(    20, 225, 20+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set8_noalarm1
void display_set8_noalarm(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 53,42,7);
	else 			NAND_ReadData2(	imageBuffer, 53,49,7);
	TFT_DrawImage( 249, 225, 249+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=========================================
// FILE_SET3
//=========================================
// IMG_set9_time range  �ð� ����
void display_set9_time_range(unsigned char select)
{
	if (select==0) 		NAND_ReadData2(	imageBuffer, 54,0,7);
	else 				NAND_ReadData2(	imageBuffer, 54,7,7);
	TFT_DrawImage( 20, 225, 20+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
// IMG_set9_nongdo_range �� ����
void display_set9_nongdo_range(unsigned char select)
{
	if (select==0) 		NAND_ReadData2(	imageBuffer, 54,14,7);
	else 				NAND_ReadData2(	imageBuffer, 54,21,7);
	TFT_DrawImage( 248, 225, 248+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}
//=============================================
// IMG_set10_sensor ���� �޴�
void display_set10_sensor(unsigned char select)
{
	if (select==0) 		NAND_ReadData2(	imageBuffer, 54,28,4);	// ����
	else if (select==1)	NAND_ReadData2(	imageBuffer, 54,32,4);	// �̻�, ���� ���� 
	else if (select==2)	NAND_ReadData2(	imageBuffer, 54,36,4);	// �̻�, ���� ����
	else 				NAND_ReadData2(	imageBuffer, 54,40,4);	// ���� ���� ����
	TFT_DrawImage( 173, 219, 173+143, 219+24,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

//=============================================
// IMG_set11_RUN
void display_set11_run(unsigned char select)
{
	if (select==0) 		NAND_ReadData2(	imageBuffer, 54,44,7);
	else 				NAND_ReadData2(	imageBuffer, 54,51,7);	
	TFT_DrawImage(    20, 225, 20+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set11_stop
void display_set11_stop(unsigned char select)
{
	if (select==0) 		NAND_ReadData2(	imageBuffer, 55,0,7);	
	else 				NAND_ReadData2(	imageBuffer, 55,7,7);	
	TFT_DrawImage( 249, 225, 249+212, 225+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set11_manwash
void display_set11_manwash(void)
{
	NAND_ReadData2(	imageBuffer, 55,14,2);
	TFT_DrawImage( 203, 74, 203+73, 74+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set11_running
void display_set11_running(void)
{
	NAND_ReadData2(	imageBuffer, 55,16,11);	
	TFT_DrawImage( 140, 140, 140+222, 140+48,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_set11_manwash
void display_Calib_Buff_ph4(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 55,27,7);
	else 			NAND_ReadData2(	imageBuffer, 55,34,7);
	TFT_DrawImage(   20, 225, 20+212, 225+30,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set11_running
void display_Calib_Buff_ph7(unsigned char select)
{
	if (select==0) 	NAND_ReadData2(	imageBuffer, 55,41,7);
	else 			NAND_ReadData2(	imageBuffer, 55,48,7);
	TFT_DrawImage(  249, 225, 249+212, 225+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=========================================
// FILE_NAME1
//=========================================
#define X_NAME_START	180
#define Y_NAME_START 	68

#define X_NAME_END	(180+120)
#define Y_NAME_END 	(68+30)


// IMG_name_back
void display_name_back(void)
{
	NAND_ReadData2(	imageBuffer, 60,0,4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_analog
void display_analog(void)
{
	NAND_ReadData2(	imageBuffer, 60, 4, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_comm
void display_comm(void)
{
	NAND_ReadData2(	imageBuffer, 60, 8, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_comm_method
void display_comm_method(void)
{
	NAND_ReadData2(	imageBuffer, 62, 31, 2);
	TFT_DrawImage( 47,124, 47+70, 124+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_comm_number
void display_comm_number(void)
{
	NAND_ReadData2(	imageBuffer, 62, 33, 2);
	TFT_DrawImage( 258,124, 258+70, 124+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_comm_baud
void display_comm_baud(void)
{
	NAND_ReadData2(	imageBuffer, 62, 35, 2);
	TFT_DrawImage( 47,189, 47+70, 189+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_comm_databit
void display_comm_databit(void)
{
	NAND_ReadData2(	imageBuffer, 62, 37, 2);
	TFT_DrawImage( 258,189, 258+70, 189+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//====================================


// IMG_comm_Ethernet
void display_Ethernet(unsigned int x, unsigned y)
{
	NAND_ReadData2(	imageBuffer, 77, 0, 2);	
	TFT_DrawImage( x, y, x+80, y+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void display_Ethernet2(void)
{
	NAND_ReadData2(	imageBuffer, 77, 2, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_START+120, Y_NAME_START+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_ip addr
void display_IP_addr(void)
{
	NAND_ReadData2(	imageBuffer, 77, 6, 2);
	TFT_DrawImage( 47,124, 47+70, 124+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_gateway
void display_gateway(void)
{
	NAND_ReadData2(	imageBuffer, 77, 8, 2);
	TFT_DrawImage( 258,124, 258+70, 124+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_subnet
void display_subnet(void)
{
	NAND_ReadData2(	imageBuffer, 77, 10, 2);
	TFT_DrawImage( 47,189, 47+88, 189+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_port
void display_port(void)
{
	NAND_ReadData2(	imageBuffer, 77, 12, 2);
	TFT_DrawImage( 258,189, 258+70, 189+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//======================================


// IMG_autowash
void display_autowash(void)
{
	NAND_ReadData2(	imageBuffer, 60, 12, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_manwash
void display_manwash(void)
{
	NAND_ReadData2(	imageBuffer, 60, 16, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_alarm
void display_alarm(void)
{
	NAND_ReadData2(	imageBuffer, 60, 20, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_autoalarm
void display_autoalarm(void)
{
	NAND_ReadData2(	imageBuffer, 60, 24, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_alarm_off
void display_alarm_off(void)
{
	NAND_ReadData2(	imageBuffer, 60, 28, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_act_set
void display_act_set(void)
{
	NAND_ReadData2(	imageBuffer, 60, 32, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_act_manual
void display_act_manual(void)
{
	NAND_ReadData2(	imageBuffer, 60, 36, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_time
void display_time(void)
{
	NAND_ReadData2(	imageBuffer, 60, 40, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_slope
void display_slope(void)
{
	NAND_ReadData2(	imageBuffer, 60, 44, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_offset
void display_offset(void)
{
	NAND_ReadData2(	imageBuffer, 60, 48, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_filter
void display_filter(void)
{
	NAND_ReadData2(	imageBuffer, 60, 52, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_init
void display_init(void)
{
	NAND_ReadData2(	imageBuffer, 60, 56, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_zero
void display_zero(void)
{
	NAND_ReadData2(	imageBuffer, 60, 60, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_buff
void display_buff(void)
{
	NAND_ReadData2(	imageBuffer, 61, 28, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_span
void display_span(void)
{
	NAND_ReadData2(	imageBuffer, 61, 0, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_temp
void display_temp(void)
{
	NAND_ReadData2(	imageBuffer, 61, 4, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

//=============================


//=============================

// IMG_zero
void display_zeroXY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 60, 60, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_span
void display_spanXY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 61, 0, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_temp
void display_tempXY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 61, 4, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

//======================================
// �α� �޼��� 

// IMG_zero ���α���
void display_zero_W_XY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 61, 48, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_span ���ұ���
void display_span_W_XY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 61, 52, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_temp �µ�����
void display_temp_W_XY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 61, 56, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_clear �����?
void display_clear_W_XY(unsigned int X, unsigned int Y)
{
	TFT_Fill( X, Y, X+120, Y+30, WHITE);
}

// IMG_buffer ���۱���
void display_buffer_W_XY(unsigned int X, unsigned int Y)
{
	NAND_ReadData2(	imageBuffer, 55, 55, 4);	
	TFT_DrawImage( X, Y, X+120, Y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=============================


// IMG_hystory
void display_hystory(void)
{
	NAND_ReadData2(	imageBuffer, 61, 8, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_time_range
void display_time_range(void)
{
	NAND_ReadData2(	imageBuffer, 61, 12, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_nongdo_range
void display_nongdo_range(void)
{
	NAND_ReadData2(	imageBuffer, 61, 16, 4);	
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_value_scale
void display_value_scale(void)
{
	NAND_ReadData2(	imageBuffer, 61, 20, 4);
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_diag
void display_diag(void)
{
	NAND_ReadData2(	imageBuffer, 61, 24, 4);
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_buffer
void display_buffer(void)
{
	NAND_ReadData2(	imageBuffer, 61, 28, 4);
	TFT_DrawImage( X_NAME_START, Y_NAME_START, X_NAME_END, Y_NAME_END,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//===============================================

// IMG_CL1,  �ܷ�����
void display_CL1(void)
{
	NAND_ReadData2(	imageBuffer, 61, 32, 4);
	TFT_DrawImage( 180,4, 180+120, 4+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_CL2,  Ź��
void display_CL2(void)
{
	NAND_ReadData2(	imageBuffer, 61, 36, 4);
	TFT_DrawImage( 180,4, 180+120, 4+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_o2,  ����̿�?
void display_O2(void)
{
	NAND_ReadData2(	imageBuffer, 61, 40, 4);
	TFT_DrawImage( 180,4, 180+120, 4+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_elec,  ����������
void display_elec(void)
{
	NAND_ReadData2(	imageBuffer, 61, 44, 4);
	TFT_DrawImage( 180,4, 180+120, 4+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_home,  Ȩ
void display_home(void)
{
	NAND_ReadData2(	imageBuffer, 61, 60, 4);
	TFT_DrawImage( 180,4, 180+120, 4+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=========================================
// FILE_ETC1
//=========================================

// IMG_mgl_1
void display_mgl_1(void)
{
	NAND_ReadData2(	imageBuffer, 62, 0, 2);
	TFT_DrawImage( 350,105, 350+62, 105+32,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_mgl_2
void display_mgl_2(void)
{
	NAND_ReadData2(	imageBuffer, 62, 2, 2);
	TFT_DrawImage( 195,160, 195+44, 160+24,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_mgl_3
void display_mgl_3(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 62, 4, 1);
	TFT_DrawImage( x,y, x+37, y+19,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_NTU_1
void display_NTU_1(void)
{
	NAND_ReadData2(	imageBuffer, 62, 5, 2);
	TFT_DrawImage( 365,135, 365+48, 135+22,   (uint16_t *)imageBuffer, DRAW_NORMAL);
//	TFT_DrawImage( 190, 160, 190+48, 157+22,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

void display_NTU_2(void)
{
	NAND_ReadData2(	imageBuffer, 62, 5, 2);
	TFT_DrawImage( 180, 160, 180+48, 160+22,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_NTU_3
void display_NTU_3(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 62, 7, 1);
//	TFT_DrawImage( 372,80, 372+29, 80+13,   (uint16_t *)imageBuffer, DRAW_NORMAL);
	TFT_DrawImage( x,y, x+29, y+13,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_mV_1
void display_mV_1(void)
{
	NAND_ReadData2(	imageBuffer, 62, 8, 1);
	TFT_DrawImage( 365,162, 365+31, 162+15,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_uscm
void display_uscm(void)
{
	NAND_ReadData2(	imageBuffer, 62, 9, 3);
	TFT_DrawImage( 330,110, 330+82, 110+32,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_uscm_3
void display_uscm3(void)
{
	NAND_ReadData2(	imageBuffer, 62, 12, 1);
	TFT_DrawImage( 358,163, 358+44, 163+19,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_4mA 2�� �׸���.
void display_4mA(void)
{
	NAND_ReadData2(	imageBuffer, 62, 13, 1);
	TFT_DrawImage( 46,125, 46+40, 125+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
	TFT_DrawImage( 258,125, 258+40, 125+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_20mA  2�� �׸���.
void display_20mA(void)
{
	NAND_ReadData2(	imageBuffer, 62, 14, 1);
	TFT_DrawImage(46,188, 46+40, 188+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
	TFT_DrawImage(258,188, 258+40, 188+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_year
void display_year(void)
{
	NAND_ReadData2(	imageBuffer, 62, 15,1);
	TFT_DrawImage( 56,125, 56+20, 125+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_mon
void display_mon(void)
{
	NAND_ReadData2(	imageBuffer, 62, 16,1);
	TFT_DrawImage( 199,125, 199+20, 125+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_day
void display_day(void)
{
	NAND_ReadData2(	imageBuffer, 62, 17,1);
	TFT_DrawImage( 340,125, 340+20, 125+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_hour
void display_hour(void)
{
	NAND_ReadData2(	imageBuffer, 62, 18,1);
	TFT_DrawImage( 56,188, 56+20, 188+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_day_trand
void display_day_trand(void)
{
	NAND_ReadData2(	imageBuffer, 62, 17,1);
	TFT_DrawImage( 424,60, 424+20, 60+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_hour_trand
void display_hour_trand(void)
{
	NAND_ReadData2(	imageBuffer, 62, 18,1);
	TFT_DrawImage( 424,60, 424+20, 60+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_min
void display_min(void)
{
	NAND_ReadData2(	imageBuffer, 62, 19,1);
	TFT_DrawImage( 199,188, 199+20, 188+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_sec
void display_sec(void)
{
	NAND_ReadData2(	imageBuffer, 62, 20,1);
	TFT_DrawImage( 340,188, 340+20, 188+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_range1
void display_range1(void)
{
	NAND_ReadData2(	imageBuffer, 62, 21, 1);
	TFT_DrawImage( 62,142, 62+60, 142+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_range2
void display_range2(void)
{
	NAND_ReadData2(	imageBuffer, 62, 22,1);
	TFT_DrawImage( 62,203, 62+60, 203+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_value_max
void display_value_max(void)
{
	NAND_ReadData2(	imageBuffer, 62, 23,1);
	TFT_DrawImage( 40,150, 40+50, 150+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_value_min
void display_value_min(void)
{
	NAND_ReadData2(	imageBuffer, 62, 24,1);
	TFT_DrawImage( 187,150, 187+50, 150+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_zero_alarm
void display_zero_alarm(void)
{
	NAND_ReadData2(	imageBuffer, 62, 25,1);
	TFT_DrawImage( 327,150, 327+50, 150+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_ph
void display_ph0(void)
{
	NAND_ReadData2(	imageBuffer, 62, 26, 1);
	TFT_DrawImage( 85,112, 85+34, 112+26,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_ph
void display_ph(void)
{
	NAND_ReadData2(	imageBuffer, 62, 26, 1);
	TFT_DrawImage( 85,112, 85+34, 112+26,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_ph4
void display_ph4(void)
{
	NAND_ReadData2(	imageBuffer, 62, 27, 1);
	TFT_DrawImage( 75,176, 75+41, 176+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_ph7
void display_ph7(void)
{
	NAND_ReadData2(	imageBuffer, 62, 28, 1);
	TFT_DrawImage( 75,176, 75+41, 176+21,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_ph10
void display_ph10(void)
{
	NAND_ReadData2(	imageBuffer, 62, 29, 2);
	TFT_DrawImage( 82,170, 82+49, 170+22,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_message5
void display_message_STOP(void)
{
//	TFT_Fill(122,126, 122+250,126+64,  WHITE);
	TFT_Fill(122,126, 122+250,126+70,  WHITE);

	NAND_ReadData2(	imageBuffer, 62, 39, 2);
	TFT_DrawImage( 211,148, 211+60, 148+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set_period
void display_set_period(void)
{
	NAND_ReadData2(	imageBuffer, 62, 41, 2);
	TFT_DrawImage( 63,144, 63+70, 144+24,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_set_time
void display_set_time(void)
{
	NAND_ReadData2(	imageBuffer, 62, 43, 2);
	TFT_DrawImage( 63,205, 63+70, 205+24,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

//===================================================

// IMG_4_20mA
void display_4_20mA(void)
{
	NAND_ReadData2(	imageBuffer, 62, 45, 6);
	TFT_DrawImage(322, 220,  322+140, 220+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_RS232
void display_RS_232(void)
{
	NAND_ReadData2(	imageBuffer, 62, 51, 6);
	TFT_DrawImage(322, 220,  322+140, 220+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_RS485
void display_RS_485(void)
{
	NAND_ReadData2(	imageBuffer, 62, 57, 6);
	TFT_DrawImage(322, 220,  322+140, 220+42,  (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//===================================================
// IMG_message1	, ����ȭ�� Ŭ����  
void display_message_clear(void)
{
	TFT_Fill(122,140, 122+250,140+64,  WHITE);

//	NAND_ReadData2(	imageBuffer, 63, 0, 16);
//	TFT_DrawImage( 122,140, 122+250,140+64,   (uint16_t *)imageBuffer, DRAW_WHITE);
}

// IMG_message1	, ����������
void display_message1(void)
{
	NAND_ReadData2(	imageBuffer, 63, 0, 16);
	TFT_DrawImage( 122,140, 122+250,140+64,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_message2, �ڵ�������
void display_message2(void)
{
	NAND_ReadData2(	imageBuffer, 63, 16, 16);
	TFT_DrawImage( 122,140, 122+250,140+64,    (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_message3, �ڵ��溸
void display_message3(void)
{
	NAND_ReadData2(	imageBuffer, 63, 32, 16);
	TFT_DrawImage( 128,140, 128+250,140+60,    (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// IMG_message4, �溸 ����
void display_message4(void)
{
	NAND_ReadData2(	imageBuffer, 63, 48, 16);
	TFT_DrawImage( 120,140, 120+250,140+64,    (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_message6, �ʱ�ȭ
void display_message6(void)
{
	NAND_ReadData2(	imageBuffer, 64, 0, 19);
	TFT_DrawImage( 70,130, 70+340, 130+55,   (uint16_t *)imageBuffer, DRAW_NORMAL);

	NAND_ReadData2(	imageBuffer, 64, 19, 19);
	TFT_DrawImage( 70,185, 70+340, 185+55,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_message6, �ʱ�ȭ
void display_trend_hour(unsigned char n)
{
	if (n==0) 		NAND_ReadData2(imageBuffer, 76, 0, 3);
	else if (n==1) 	NAND_ReadData2(imageBuffer, 76, 3, 3);
	else if (n==2) 	NAND_ReadData2(imageBuffer, 76, 6, 3);
	else if (n==3) 	NAND_ReadData2(imageBuffer, 76, 9, 3);
	else if (n==4) 	NAND_ReadData2(imageBuffer, 76, 12, 3);
	else if (n==5) 	NAND_ReadData2(imageBuffer, 76, 15, 3);

	TFT_DrawImage( 200,227, 200+80, 227+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// IMG_back_gtaph, �׷������?


extern uint16_t  trand_select_Y;
void display_back_graph(void)
{
#ifndef SENSOR_PH_EC
    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
	{
		if (trand_select_Y==0) {
			sprintf(strBuffer, "2   ");   DrawSmallNumber(29, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "1   ");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0 ");     DrawSmallNumber(29, 208, strBuffer, BLUE);
//			NAND_ReadData2(	imageBuffer, 72, 0, 13);
		}
		else if (trand_select_Y==1) {
			sprintf(strBuffer, "1   ");   DrawSmallNumber(29, 80,   strBuffer, BLUE);

			sprintf(strBuffer, ".");   DrawSmallNumber(38, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "5 ");   DrawSmallNumber(47, 144,  strBuffer, BLUE);

			sprintf(strBuffer, "0 ");   DrawSmallNumber(29, 208, strBuffer, BLUE);
//			NAND_ReadData2(	imageBuffer, 72, 13, 13);
		}
		else if (trand_select_Y==2) {
			sprintf(strBuffer, ".");   DrawSmallNumber(38, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "0");   DrawSmallNumber(29, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "5 ");   DrawSmallNumber(47, 80,   strBuffer, BLUE);

			sprintf(strBuffer, ".");   DrawSmallNumber(38, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "25");   DrawSmallNumber(47, 144,  strBuffer, BLUE);

			sprintf(strBuffer, "0 ");   DrawSmallNumber(29, 208, strBuffer, BLUE);
//			NAND_ReadData2(	imageBuffer, 72, 26, 13);
		}
	}
	else {
		if (trand_select_Y==0) {
//			NAND_ReadData2(	imageBuffer, 73, 0, 13);
			sprintf(strBuffer, "20  ");   DrawSmallNumber(29, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "10  ");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0 ");     DrawSmallNumber(29, 208, strBuffer, BLUE);
		}
		else if (trand_select_Y==1) {
//			NAND_ReadData2(	imageBuffer, 73, 13, 13);
			sprintf(strBuffer, "10  ");   DrawSmallNumber(29, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "5   ");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0 ");     DrawSmallNumber(29, 208, strBuffer, BLUE);
		}
		else if (trand_select_Y==2) {
//			NAND_ReadData2(	imageBuffer, 73, 26, 13);
			sprintf(strBuffer, "1   ");   DrawSmallNumber(29, 80,   strBuffer, BLUE);

			sprintf(strBuffer, ".");   DrawSmallNumber(38, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "5 ");   DrawSmallNumber(47, 144,  strBuffer, BLUE);

			sprintf(strBuffer, "0 ");   DrawSmallNumber(29, 208, strBuffer, BLUE);

		}
		else if (trand_select_Y==3) {
//			NAND_ReadData2(	imageBuffer, 73, 39, 13);
			sprintf(strBuffer, ".");   DrawSmallNumber(38, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "0");   DrawSmallNumber(29, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "5 ");   DrawSmallNumber(47, 80,   strBuffer, BLUE);

			sprintf(strBuffer, ".");   DrawSmallNumber(38, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "0");   DrawSmallNumber(29, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "25");   DrawSmallNumber(47, 144,  strBuffer, BLUE);

			sprintf(strBuffer, "0 ");   DrawSmallNumber(29, 208, strBuffer, BLUE);

		}
	}
	//TFT_DrawImage( 27,40, 27+50, 240,   (uint16_t *)imageBuffer, DRAW_NORMAL);

#else
    if (currentData.Device_Selector_Mode & SENSOR_1_MODE) 
	{
		if (trand_select_Y==0) {
//			NAND_ReadData2(	imageBuffer, 74, 0, 13);
			sprintf(strBuffer, "14 ");   DrawSmallNumber(41, 80,   strBuffer, BLUE);
			sprintf(strBuffer, " 7 ");   DrawSmallNumber(41, 144,  strBuffer, BLUE);
			sprintf(strBuffer, " 0 ");     DrawSmallNumber(41, 208, strBuffer, BLUE);

		}
		else if (trand_select_Y==1) {
//			NAND_ReadData2(	imageBuffer, 74, 13, 13);
			sprintf(strBuffer, "10 ");   DrawSmallNumber(41, 80,   strBuffer, BLUE);
			sprintf(strBuffer, " 5 ");   DrawSmallNumber(41, 144,  strBuffer, BLUE);
			sprintf(strBuffer, " 0 ");     DrawSmallNumber(41, 208, strBuffer, BLUE);
		}
		else if (trand_select_Y==2) {
//			NAND_ReadData2(	imageBuffer, 74, 26, 13);
			sprintf(strBuffer, " 8 ");   DrawSmallNumber(41, 80,   strBuffer, BLUE);
			sprintf(strBuffer, " 4 ");   DrawSmallNumber(41, 144,  strBuffer, BLUE);
			sprintf(strBuffer, " 0 ");     DrawSmallNumber(41, 208, strBuffer, BLUE);
		}
		else if (trand_select_Y==3) {
//			NAND_ReadData2(	imageBuffer, 74, 39, 13);
			sprintf(strBuffer, " 6 ");   DrawSmallNumber(41, 80,   strBuffer, BLUE);
			sprintf(strBuffer, " 3 ");   DrawSmallNumber(41, 144,  strBuffer, BLUE);
			sprintf(strBuffer, " 0 ");     DrawSmallNumber(41, 208, strBuffer, BLUE);
		}
	}
	else {
		if (trand_select_Y==0) {
//			NAND_ReadData2(	imageBuffer, 75, 0, 13);
			sprintf(strBuffer, "2000");   DrawSmallNumber(24, 80,   strBuffer, BLUE);
			sprintf(strBuffer, "1000");   DrawSmallNumber(24, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "  0 ");     DrawSmallNumber(24, 208, strBuffer, BLUE);
		}
		else if (trand_select_Y==1) {
//			NAND_ReadData2(	imageBuffer, 75, 13, 13);
			sprintf(strBuffer, "1000");   DrawSmallNumber(24, 80,   strBuffer, BLUE);
			sprintf(strBuffer, " 500");   DrawSmallNumber(24, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "  0 ");     DrawSmallNumber(24, 208, strBuffer, BLUE);
		}
		else if (trand_select_Y==2) {
//			NAND_ReadData2(	imageBuffer, 75, 26, 13);
			sprintf(strBuffer, " 500");   DrawSmallNumber(24, 80,   strBuffer, BLUE);
			sprintf(strBuffer, " 250");   DrawSmallNumber(24, 144,  strBuffer, BLUE);
			sprintf(strBuffer, "  0 ");     DrawSmallNumber(24, 208, strBuffer, BLUE);
		}
	}
	//TFT_DrawImage( 27,40, 27+50, 240,   (uint16_t *)imageBuffer, DRAW_NORMAL);

#endif


    TFT_Fill(GRAPH_X_START, GRAPH_Y_MAX-3, GRAPH_X_MAX, GRAPH_Y_ZERO+3, WHITE);

}


void display_logo(void)
{
	/* White background, logo with color key (BACK_COLOR3/WHITE -> transparent) */
	TFT_Fill(0, 0, 480, 272, WHITE);

	NAND_ReadData2(	imageBuffer, 41, 0, 24);
	TFT_DrawImage( 0, 0, 480, 50,   (uint16_t *)imageBuffer, DRAW_WHITE_BG);

	NAND_ReadData2(	imageBuffer, 41, 24, 24);
	TFT_DrawImage( 0, 50, 480, 100,  (uint16_t *)imageBuffer, DRAW_WHITE_BG);

	NAND_ReadData2(	imageBuffer, 42, 0, 24);
	TFT_DrawImage( 0, 100, 480, 150, (uint16_t *)imageBuffer, DRAW_WHITE_BG);

	NAND_ReadData2(	imageBuffer, 42, 24, 24);
	TFT_DrawImage( 0, 150, 480, 200, (uint16_t *)imageBuffer, DRAW_WHITE_BG);

	NAND_ReadData2(	imageBuffer, 43, 0, 24);
	TFT_DrawImage( 0, 200, 480, 250, (uint16_t *)imageBuffer, DRAW_WHITE_BG);

	NAND_ReadData2(	imageBuffer, 43, 24, 24);
	TFT_DrawImage( 0, 250, 480, 272, (uint16_t *)imageBuffer, DRAW_WHITE_BG);
}




