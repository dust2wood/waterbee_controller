#include "Main.h"


//====================================================
// NEMERIC_2CH_HOME (24x30) ��� DrawMediumNumber(18x23) ���
// NEMERIC_2CH_TEMP (12x16) ��� DrawSmallNumber(12x16) ���
// NAND Block 80 ��Ʈ�� ���� ����, display_icon ���� ��Ʈ�� �ؽ�Ʈ ���
extern void DrawMediumNumber(uint32_t x, uint32_t y, char* str, uint16_t color);
extern void DrawSmallNumber(uint32_t x, uint32_t y, char* str, uint16_t color);

void Draw_2CH_HOME_Number(uint32_t x, uint32_t y, char* str, uint16_t color) {
    /* 24x30 -> 18x23 ��ȯ: ��ǥ ���� (y + 4�� ���� �߾� ����, 30px ���� �� 23px ��Ʈ) */
    DrawMediumNumber(x, y + 4, str, color);
}

void Draw_2CH_TEMP_Number(uint32_t x, uint32_t y, char* str, uint16_t color) {
    /* 12x16 ���� ũ��, DrawSmallNumber �״�� ��� */
    DrawSmallNumber(x, y, str, color);
}


//====================================================
//====================================================
//====================================================

void NEMERIC_4CH_HOME(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	NAND_ReadData2(	imageBuffer, 82, 6, 9);
	TFT_DrawNumeric(x, y,  x+21, y+27,  (uint16_t *)(&imageBuffer[data*21*27*2]), color);
}


void Draw_4CH_HOME_Number(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
//    NAND_ReadSNumber();
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
			NEMERIC_4CH_HOME(x,y,str[i]-0x30, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 15 * 17 * 2]), color);
        else if (str[i] == ' ')
			NEMERIC_4CH_HOME(x,y,13, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[13 * 15 * 17 * 2]), color);
        else if (str[i] == '-')
            NEMERIC_4CH_HOME(x,y,10, color);
			//TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[10 * 15 * 17 * 2]), color);
        else if (str[i] == ':')
            NEMERIC_4CH_HOME(x,y,11,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[11 * 15 * 17 * 2]), color);
        else if (str[i] == '.')
            NEMERIC_4CH_HOME(x,y,12,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[12 * 15 * 17 * 2]), color);
        else if (str[i] == 'C')
            NEMERIC_4CH_HOME(x,y,14,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[14 * 15 * 17 * 2]), color); // �µ� ǥ�� �߰�����.
        else
            return;
        x += 20;
        ++i;
    }
}


//====================================================
void NEMERIC_4CH_TEMP(unsigned int x, unsigned int y, unsigned char data, uint16_t color)
{
	NAND_ReadData2(	imageBuffer, 82, 19, 2);
	TFT_DrawNumeric(x, y,  x+10, y+13,  (uint16_t *)(&imageBuffer[data*10*13*2]), color);
}


void Draw_4CH_TEMP_Number(uint32_t x, uint32_t y, char* str, uint16_t color) {
    uint32_t i = 0;
//    NAND_ReadSNumber();
    while (str[i] != 0) {
        if (str[i] > 0x2F && str[i] < 0x3A)
			NEMERIC_4CH_TEMP(x,y,str[i]-0x30, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[(str[i] - 0x30) * 15 * 17 * 2]), color);
        else if (str[i] == '-')
            NEMERIC_4CH_TEMP(x,y,10, color);
			//TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[10 * 15 * 17 * 2]), color);
        else if (str[i] == ':')
            NEMERIC_4CH_TEMP(x,y,11,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[11 * 15 * 17 * 2]), color);
        else if (str[i] == '.')
            NEMERIC_4CH_TEMP(x,y,12,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[12 * 15 * 17 * 2]), color);
        else if (str[i] == ' ')
			NEMERIC_4CH_TEMP(x,y,13, color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[13 * 15 * 17 * 2]), color);
        else if (str[i] == 'C')
            NEMERIC_4CH_TEMP(x,y,14,color);
            //TFT_DrawImageSmall(x, y, x + 15, y + 17, (uint16_t*)&(imageBuffer[14 * 15 * 17 * 2]), color); // �µ� ǥ�� �߰�����.
        else
            return;
        x += 9;
        ++i;
    }
}


//***************************
// 2ch icon
//***************************

// 2ch cl
#define ch2_cl_x	50
#define ch2_cl_y	70
void display_2ch_cl(void)
{
	NAND_ReadData2(	imageBuffer, 80, 0, 2);
	TFT_DrawImage( ch2_cl_x, ch2_cl_y, ch2_cl_x+76, ch2_cl_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 2ch tu
#define ch2_tu_x	290
#define ch2_tu_y	70
void display_2ch_tu(void)
{
	NAND_ReadData2(	imageBuffer, 80, 25, 1);
	TFT_DrawImage( ch2_tu_x, ch2_tu_y, ch2_tu_x+38, ch2_tu_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch ph
#define ch2_ph_x	50
#define ch2_ph_y	70
void display_2ch_ph(void)
{
	NAND_ReadData2(	imageBuffer, 80, 18, 2);
	TFT_DrawImage( ch2_ph_x, ch2_ph_y, ch2_ph_x+76, ch2_ph_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 2ch ec
#define ch2_ec_x	280
#define ch2_ec_y	70
void display_2ch_ec(void)
{
	NAND_ReadData2(	imageBuffer, 80, 3, 2);
	TFT_DrawImage( ch2_ec_x, ch2_ec_y, ch2_ec_x+97, ch2_ec_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 2ch small_unit_ec
#define ch2_small_unit_ec_x	190
#define ch2_small_unit_ec_y	162
void display_2ch_small_unit_ec(void)
{
	NAND_ReadData2(	imageBuffer, 80, 27, 1);
	TFT_DrawImage( ch2_small_unit_ec_x, ch2_small_unit_ec_y, ch2_small_unit_ec_x+40, ch2_small_unit_ec_y+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// ec_trand
void display_2ch_small_ec_xy(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 80, 27, 1);
	TFT_DrawImage( x, y, x+40, y+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch small_unit_ph
#define ch2_small_unit_ph_x	40
#define ch2_small_unit_ph_y	162
void display_2ch_small_unit_ph(void)
{
	NAND_ReadData2(	imageBuffer, 80, 29, 1);
	TFT_DrawImage( ch2_small_unit_ph_x, ch2_small_unit_ph_y, ch2_small_unit_ph_x+19, ch2_small_unit_ph_y+14,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// ph_trand
void display_2ch_small_ph_xy(unsigned int x, unsigned int y)
{
	NAND_ReadData2(	imageBuffer, 80, 29, 1);
	TFT_DrawImage( x, y, x+19, y+14,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch unit_cl
#define ch2_unit_cl_x	168
#define ch2_unit_cl_y	120
void display_2ch_unit_cl(void)
{
	NAND_ReadData2(	imageBuffer, 80, 31, 1);
	TFT_DrawImage( ch2_unit_cl_x, ch2_unit_cl_y, ch2_unit_cl_x+48, ch2_unit_cl_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 2ch unit_ec
#define ch2_unit_ec_x	400
#define ch2_unit_ec_y	120
void display_2ch_unit_ec(void)
{
	NAND_ReadData2(	imageBuffer, 80, 33, 2);
	TFT_DrawImage( ch2_unit_ec_x, ch2_unit_ec_y, ch2_unit_ec_x+52, ch2_unit_ec_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 2ch unit_ph
#define ch2_unit_ph_x	50
#define ch2_unit_ph_y	120
void display_2ch_unit_ph(void)
{
	NAND_ReadData2(	imageBuffer, 80, 36, 1);
	TFT_DrawImage( ch2_unit_ph_x, ch2_unit_ph_y, ch2_unit_ph_x+26, ch2_unit_ph_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 2ch unit_tu
#define ch2_unit_tu_x	404
#define ch2_unit_tu_y	120
void display_2ch_unit_tu(void)
{
	NAND_ReadData2(	imageBuffer, 80, 38, 1);
	TFT_DrawImage( ch2_unit_tu_x, ch2_unit_tu_y, ch2_unit_tu_x+48, ch2_unit_tu_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//=========================================

// 2ch set_cl1
#define ch2_set_cl1_x	20
#define ch2_set_cl1_y	225
void display_2ch_set_cl1(void)
{
	NAND_ReadData2(	imageBuffer, 81, 0, 7);
	TFT_DrawImage( ch2_set_cl1_x, ch2_set_cl1_y, ch2_set_cl1_x+212, ch2_set_cl1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_cl2
#define ch2_set_cl2_x	20
#define ch2_set_cl2_y	225
void display_2ch_set_cl2(void)
{
	NAND_ReadData2(	imageBuffer, 81, 8, 7);
	TFT_DrawImage( ch2_set_cl2_x, ch2_set_cl2_y, ch2_set_cl2_x+212, ch2_set_cl2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_ph1
#define ch2_set_ph1_x	20
#define ch2_set_ph1_y	225
void display_2ch_set_ph1(void)
{
	NAND_ReadData2(	imageBuffer, 81, 32, 7);
	TFT_DrawImage( ch2_set_ph1_x, ch2_set_ph1_y, ch2_set_ph1_x+212, ch2_set_ph1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_ph2
#define ch2_set_ph2_x	20
#define ch2_set_ph2_y	225
void display_2ch_set_ph2(void)
{
	NAND_ReadData2(	imageBuffer, 81, 40, 7);
	TFT_DrawImage( ch2_set_ph2_x, ch2_set_ph2_y, ch2_set_ph2_x+212, ch2_set_ph2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_ec1
#define ch2_set_ec1_x	255
#define ch2_set_ec1_y	225
void display_2ch_set_ec1(void)
{
	NAND_ReadData2(	imageBuffer, 81, 16, 7);
	TFT_DrawImage( ch2_set_ec1_x, ch2_set_ec1_y, ch2_set_ec1_x+212, ch2_set_ec1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_ec2
#define ch2_set_ec2_x	255
#define ch2_set_ec2_y	225
void display_2ch_set_ec2(void)
{
	NAND_ReadData2(	imageBuffer, 81, 24, 7);
	TFT_DrawImage( ch2_set_ec2_x, ch2_set_ec2_y, ch2_set_ec2_x+212, ch2_set_ec2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_tu1
#define ch2_set_tu1_x	255
#define ch2_set_tu1_y	225
void display_2ch_set_tu1(void)
{
	NAND_ReadData2(	imageBuffer, 81, 48, 7);
	TFT_DrawImage( ch2_set_tu1_x, ch2_set_tu1_y, ch2_set_tu1_x+212, ch2_set_tu1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 2ch set_tu2
#define ch2_set_tu2_x	255
#define ch2_set_tu2_y	225
void display_2ch_set_tu2(void)
{
	NAND_ReadData2(	imageBuffer, 81, 56, 7);
	TFT_DrawImage( ch2_set_tu2_x, ch2_set_tu2_y, ch2_set_tu2_x+212, ch2_set_tu2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//***************************
// 4ch icon
//***************************

// 4ch cl
#define ch4_cl_x	350
#define ch4_cl_y	105
void display_4ch_cl(void)
{
	NAND_ReadData2(	imageBuffer, 82, 0, 2);
	TFT_DrawImage( ch4_cl_x, ch4_cl_y, ch4_cl_x+76, ch4_cl_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch ec
#define ch4_ec_x	350
#define ch4_ec_y	105
void display_4ch_ec(void)
{
	NAND_ReadData2(	imageBuffer, 82, 3, 2);
	TFT_DrawImage( ch4_ec_x, ch4_ec_y, ch4_ec_x+97, ch4_ec_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch ph
#define ch4_ph_x	350
#define ch4_ph_y	105
void display_4ch_ph(void)
{
	NAND_ReadData2(	imageBuffer, 82, 16, 2);
	TFT_DrawImage( ch4_ph_x, ch4_ph_y, ch4_ph_x+76, ch4_ph_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch tu
#define ch4_tu_x	350
#define ch4_tu_y	105
void display_4ch_tu(void)
{
	NAND_ReadData2(	imageBuffer, 82, 22, 1);
	TFT_DrawImage( ch4_tu_x, ch4_tu_y, ch4_tu_x+38, ch4_tu_y+20,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch small_unit_ec
#define ch4_small_unit_ec_x	350
#define ch4_small_unit_ec_y	105
void display_4ch_small_unit_ec(void)
{
	NAND_ReadData2(	imageBuffer, 82, 24, 1);
	TFT_DrawImage( ch4_small_unit_ec_x, ch4_small_unit_ec_y, ch4_small_unit_ec_x+40, ch4_small_unit_ec_y+16,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch small_unit_ph
#define ch4_small_unit_ph_x	350
#define ch4_small_unit_ph_y	105
void display_4ch_small_unit_ph(void)
{
	NAND_ReadData2(	imageBuffer, 82, 26, 1);
	TFT_DrawImage( ch4_small_unit_ph_x, ch4_small_unit_ph_y, ch4_small_unit_ph_x+19, ch4_small_unit_ph_y+14,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch unit_cl
#define ch4_unit_cl_x	350
#define ch4_unit_cl_y	105
void display_4ch_unit_cl(void)
{
	NAND_ReadData2(	imageBuffer, 82, 28, 1);
	TFT_DrawImage( ch4_unit_cl_x, ch4_unit_cl_y, ch4_unit_cl_x+48, ch4_unit_cl_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch unit_ec
#define ch4_unit_ec_x	350
#define ch4_unit_ec_y	105
void display_4ch_unit_ec(void)
{
	NAND_ReadData2(	imageBuffer, 82, 30, 2);
	TFT_DrawImage( ch4_unit_ec_x, ch4_unit_ec_y, ch4_unit_ec_x+52, ch4_unit_ec_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch unit_ph
#define ch4_unit_ph_x	350
#define ch4_unit_ph_y	105
void display_4ch_unit_ph(void)
{
	NAND_ReadData2(	imageBuffer, 82, 33, 1);
	TFT_DrawImage( ch4_unit_ph_x, ch4_unit_ph_y, ch4_unit_ph_x+26, ch4_unit_ph_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}

// 4ch unit_tu
#define ch4_unit_tu_x	350
#define ch4_unit_tu_y	105
void display_4ch_unit_tu(void)
{
	NAND_ReadData2(	imageBuffer, 82, 35, 1);
	TFT_DrawImage( ch4_unit_tu_x, ch4_unit_tu_y, ch4_unit_tu_x+48, ch4_unit_tu_y+21,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


//==================================


// 4CH set_cl1
#define ch4_set_cl1_x	350
#define ch4_set_cl1_y	105
void display_4CH_set_cl1(void)
{
	NAND_ReadData2(	imageBuffer, 83, 0, 3);
	TFT_DrawImage( ch4_set_cl1_x, ch4_set_cl1_y, ch4_set_cl1_x+100, ch4_set_cl1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_cl2
#define ch4_set_cl2_x	350
#define ch4_set_cl2_y	105
void display_4CH_set_cl2(void)
{
	NAND_ReadData2(	imageBuffer, 83, 4, 3);
	TFT_DrawImage( ch4_set_cl2_x, ch4_set_cl2_y, ch4_set_cl2_x+100, ch4_set_cl2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_ec1
#define ch4_set_ec1_x	350
#define ch4_set_ec1_y	105
void display_4CH_set_ec1(void)
{
	NAND_ReadData2(	imageBuffer, 83, 8, 3);
	TFT_DrawImage( ch4_set_ec1_x, ch4_set_ec1_y, ch4_set_ec1_x+100, ch4_set_ec1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_cl2
#define ch4_set_ec2_x	350
#define ch4_set_ec2_y	105
void display_4CH_set_ec2(void)
{
	NAND_ReadData2(	imageBuffer, 83, 12, 3);
	TFT_DrawImage( ch4_set_ec2_x, ch4_set_ec2_y, ch4_set_ec2_x+100, ch4_set_ec2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_ph1
#define ch4_set_ph1_x	350
#define ch4_set_ph1_y	105
void display_4CH_set_ph1(void)
{
	NAND_ReadData2(	imageBuffer, 83, 16, 3);
	TFT_DrawImage( ch4_set_ph1_x, ch4_set_ph1_y, ch4_set_ph1_x+100, ch4_set_ph1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_ph2
#define ch4_set_ph2_x	350
#define ch4_set_ph2_y	105
void display_4CH_set_ph2(void)
{
	NAND_ReadData2(	imageBuffer, 83, 20, 3);
	TFT_DrawImage( ch4_set_ph2_x, ch4_set_ph2_y, ch4_set_ph2_x+100, ch4_set_ph2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_tu1
#define ch4_set_tu1_x	350
#define ch4_set_tu1_y	105
void display_4CH_set_tu1(void)
{
	NAND_ReadData2(	imageBuffer, 83, 24, 3);
	TFT_DrawImage( ch4_set_tu1_x, ch4_set_tu1_y, ch4_set_tu1_x+100, ch4_set_tu1_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


// 4CH set_tu2
#define ch4_set_tu2_x	350
#define ch4_set_tu2_y	105
void display_4CH_set_tu2(void)
{
	NAND_ReadData2(	imageBuffer, 83, 28, 3);
	TFT_DrawImage( ch4_set_tu2_x, ch4_set_tu2_y, ch4_set_tu2_x+100, ch4_set_tu2_y+30,   (uint16_t *)imageBuffer, DRAW_NORMAL);
}


