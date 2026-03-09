#ifndef TFT_H
#define TFT_H

#include "Main.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"


extern uint16_t  POINT_COLOR;//Ä¬ÈÏºìÉ«    
extern uint16_t  BACK_COLOR;//±³¾°ÑÕÉ«.Ä¬ÈÏÎª°×É«

#define DRAW_NORMAL						0
#define DRAW_REVERSE					1
#define DRAW_ARROW_CROSS				2
#define DRAW_RIGHT_TOP					2
#define DRAW_LEFT_BOTTOM				3
#define DRAW_RIGHT_BOTTOM				4
#define DRAW_WHITE_BG					10	/* Color key: BACK_COLOR3/WHITE -> WHITE for logo on white */

#define NOP				0x0000
#define SOFT_RESET		0x0001

#define GET_POWER_MODE	0x000A
#define GET_ADDR_MODE	0x000B
#define GET_PIXEL_MODE	0x000C
#define GET_DISP_MODE	0x000D
#define GET_SIGNAL_MODE	0x000E


#define SET_CMD_ADDR	0x002A
#define SET_PAGE_ADDR	0x002B
#define WRITE_MEM_START	0x002C
#define READ_MEM_START	0x002E

/* Color Define 565 */
#define Transparent	0x0821			//	1,    	1,    	1
#define RED	  				0XF800          // 	255,	0,    	0
#define GREEN 			0X07E0          //	0,		255,	0
#define BLUE  				0X001F          //	0,    	0,		255 
#define CYAN 			0X9EDD          //	0x99,	0xD9,	0xEA
#define BRED  				0XF81F          // 	255,    0,		255
#define GRED  			0XFFE0          // 	255,	255,    0
#define GBLUE 			0X07FF          // 	255,	255,    0É«
#define WHITE 			0XFFFF
#define YELLOW  		0XFFE0	
#define BLACK 			0X0000 
#define BROWN 			0XBC40
#define BRRED 			0XFC07
#define GRAY  				0X8430
#define LGRAY 			0XC618
#define DGRAY			0x7BEF
#define SKYBLUE			0x7C38
#define DBLUE				0X0008

#define MAGENTA		0xF81F



//#define BACK_COLOR		0x967f	// 0x99CCFF
//#define BACK_COLOR		CYAN
//#define BACK_COLOR		0xef9f
//#define BACK_COLOR		((0xef>>2)<<11)|((0xf1>>1)<<5)|(0xfc>>2)
//#define BACK_COLOR		((0xcc>>3)<<11)|((0x99>>2)<<5)|(0xff>>3)
//#define BACK_COLOR		((0xcc>>3)<<11)|((0x99>>2)<<5)|(0xff>>3)

//#define BACK_COLOR		0x9cd6  //99 99 AA
//#define BACK_COLOR		((0x99>>3)<<11)|((0x99>>2)<<5)|(0xaa>>3) //9999aa
//#define BACK_COLOR		((0xb9>>3)<<11)|((0xb9>>2)<<5)|(0xca>>3) //bc bc bc
//#define BACK_COLOR		((0xb0>>3)<<11)|((0xb9>>2)<<5)|(0xd0>>3) //bc bc bc  good
#define BACK_COLOR		((0xa9>>3)<<11)|((0xb3>>2)<<5)|(0xc9>>3) //a9 b3 c9	, (169. 179, 201) 

// À§ÂÊ »ç°¢Çü ¹è°æ»ö±ò
#define BACK_COLOR2		((0xbc>>3)<<11)|((0xbc>>2)<<5)|(0xbc>>3) //bc bc bc	   (188)

// °¡¿îµ¥ Å¸¿ø ¹è°æ»ö±ò
//#define BACK_COLOR3		((0xb5>>3)<<11)|((0xb5>>2)<<5)|(0xb5>>3) // b5 b5 b5 (181)
#define BACK_COLOR3		0xad99 //0XB5B6 ..1ch



#define BLUE2BLACK		0xF987





#define FSIZE 		16


void TFT_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_WR_REG(uint16_t index);
void TFT_WR_CMD(uint16_t index,uint16_t val);
void TFT_WR_Data(uint16_t val);
void TFT_WR_Data_8(uint16_t val);
uint16_t TFT_RD_data(void);
void TFT_SET_Addr(uint16_t x, uint16_t y);
void TFT_DrawImage(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t* Image, uint16_t Flag);
void TFT_Init(void);void TFT_clear(uint16_t color);
void TFT_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void TFT_DelayTime(uint16_t i);
void TFT_Point(uint16_t x,uint16_t y,uint16_t fontcolor);
void Delay(__IO uint32_t nCount);
void lcd_rst(void);
#endif




