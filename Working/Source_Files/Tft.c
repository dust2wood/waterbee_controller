#include "tft.h"
#include <stdlib.h>

////////////////////////////////////////////////////
//#define SIZE7_0INCH
#define SIZE4_3INCH
//#define SIZE3_2INCH
///////////////////////////////////////////////////

/* Keil: #pragma O0  disables optimization for this file.
 * GCC: equivalent is __attribute__((optimize("O0"))) per-function.
 * Applied below to timing-critical functions: TFT_DelayTime, lcd_rst, TFT_Init. */
#if defined(__CC_ARM)
#pragma  O0
#endif

#ifndef NEW_BOARD
	#define Bank1_LCD_D    ((uint32_t)0x68040000)    //disp Data ADDR
	#define Bank1_LCD_C    ((uint32_t)0x68000000)	 //disp Reg ADDR
#else
	#define Bank1_LCD_D    ((uint32_t)0x60040000)    //disp Data ADDR
	#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR
#endif

#define MAX_CHAR_POSX 472
#define MAX_CHAR_POSY 264 
//#define MAX_CHAR_POSX 792
//#define MAX_CHAR_POSY 472 

uint16_t  HDP=479;   
uint16_t  HT=531;
uint16_t  HPS=43;
uint16_t  LPS=8;
unsigned char HPW=10;
uint16_t  VDP=271;  
uint16_t  VT=288;
uint16_t  VPS=12;
uint16_t  FPS=4;
unsigned char   VPW=10;

void lcd_rst(void);
uint16_t TFT_RD_data(void);
extern void Delay(__IO uint32_t nCount);

void TFT_Line(uint16_t x1, uint16_t  y1, uint16_t  x2, uint16_t  y2, uint16_t color)
{
   uint16_t  deltax, deltay, x,y, z;
   uint16_t   steep;
   int32_t lerror, ystep;

    steep = ((y1>y2)?(y1-y2):(y2-y1)) > ((x1>x2)?(x1-x2):(x2-x1));   //check slope

     if (steep)
     {
		 z = x1;		 x1 = y1;		 y1 = z;
		 z = x2;		 x2 = y2;		 y2 = z;
     }

     if (x1 > x2)
     {
		 z = x1;		 x1 = x2;		 x2 = z;
		 z = y1;		 y1 = y2;		 y2 = z;
     }

     deltax = x2 - x1;
     deltay = ((y2>y1)?(y2-y1):(y1-y2));  
     lerror = deltax / 2;
     y = y1;
     if(y1 < y2) ystep = 1;  else ystep = -1;

     for(x = x1; x <= x2; x++)
     {
         if (steep) TFT_Point(y,x, color); else TFT_Point(x,y, color);
            lerror -= deltay;
         if (lerror < 0){
             y = y + ystep;
             lerror += deltax;
         }
     }
 }

/* ===================================================== */
/* 1. TFT Command Function								 */
/* ===================================================== */
/* 1-0. TFT_WR_REG		                                 */
/* ===================================================== */
void TFT_WR_REG(uint16_t index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}

/* ===================================================== */
/* 1-1. TFT_WR_CMD		                                 */
/* ===================================================== */
void TFT_WR_CMD(uint16_t index,uint16_t val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/* ===================================================== */
/* 1-2. TFT_WR_Data		                                 */
/* ===================================================== */
void    TFT_WR_Data(uint16_t val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

/* ===================================================== */
/* 1-3. TFT_WR_Data_8	                                 */
/* ===================================================== */
void TFT_WR_Data_8(uint16_t val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

/* ===================================================== */
/* 1-4. TFT_RD_data		                                 */
/* ===================================================== */
uint16_t TFT_RD_data(void){
	uint16_t a=0;
	//a=(*(__IO uint16_t *) (Bank1_LCD_D)); 	//Dummy
	//a= *(__IO uint16_t *) (Bank1_LCD_D);  	//H
	//a=a<<8;
	a=*(__IO uint16_t *) (Bank1_LCD_D); //L

	return(a);	
}


/* ===================================================== */
/* 1-4. TFT_SET_Addr		                                 */
/* ===================================================== */
void TFT_SET_Addr(uint16_t x, uint16_t y){

	TFT_WR_REG(SET_CMD_ADDR);	 
	TFT_WR_Data(x>>8);		 
	TFT_WR_Data(x&0x00ff);
	TFT_WR_Data(HDP>>8);	 
	TFT_WR_Data(HDP&0x00ff);

	TFT_WR_REG(SET_PAGE_ADDR);	 
	TFT_WR_Data(y>>8);		 
	TFT_WR_Data(y&0x00ff);
	TFT_WR_Data(VDP>>8);	 
	TFT_WR_Data(VDP&0x00ff);

	TFT_WR_REG(WRITE_MEM_START); 

}


// by JU
void TFT_DrawNumeric(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *Image, uint16_t Flag)
{
    uint32_t i, j, n = (xEnd - xStart) * (yEnd - yStart);
	uint32_t x =  xEnd - xStart , y = yEnd - yStart;
	uint16_t data;

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


	if(Flag == BACK_COLOR3)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = Image[i];
			if (data==BACK_COLOR3)	TFT_WR_Data(BACK_COLOR3);
			else if (data==WHITE)	TFT_WR_Data(BACK_COLOR3);
			else 					TFT_WR_Data(BLUE);
		}
	}
	else if(Flag == RED)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = Image[i];
			if (data==BACK_COLOR3)	TFT_WR_Data(data);
			else if (data==WHITE)	TFT_WR_Data(data);
			else 					TFT_WR_Data(RED);
		}
	}
	else if(Flag == GREEN)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = Image[i];
			if (data==BACK_COLOR3)	TFT_WR_Data(data);
			else if (data==WHITE)	TFT_WR_Data(data);
			else 					TFT_WR_Data(GREEN);
		}
	}
	else if(Flag == BLUE)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = Image[i];
			if (data==BACK_COLOR3)	TFT_WR_Data(data);
			else if (data==WHITE)	TFT_WR_Data(data);
			else 					TFT_WR_Data(BLUE);
		}
	}

	else if(Flag == BROWN)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = Image[i];
			if (data==BACK_COLOR3)	TFT_WR_Data(data);
			else if (data==WHITE)	TFT_WR_Data(data);
			else 					TFT_WR_Data(BLACK);
		}
	}

	else if(Flag == YELLOW)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = Image[i];
			if (data==BACK_COLOR3)	TFT_WR_Data(data);
			else if (data==WHITE)	TFT_WR_Data(data);
			else 					TFT_WR_Data(GREEN);
		}
	}


	//=================================
	else if(Flag == DRAW_REVERSE)
	{
		for(i = 0 ; i < n ; ++i)
			TFT_WR_Data(~Image[i]);
	}
	else if(Flag == DRAW_RIGHT_TOP)
	{
		for(i = 0 ; i < y ; i++)
		{
			for(j = x ; j > 0 ; j--)
			{
				TFT_WR_Data(Image[(i*x)+(j-1)]);
			}
		}
	}
	else if(Flag == DRAW_LEFT_BOTTOM)
	{
		for(i = y ; i > 0 ; i--)
		{
			for(j = 0 ; j < x ; j++)
			{
				TFT_WR_Data(Image[(j*x)+(i-1)]);
			}
		}
	}
	else if(Flag == DRAW_RIGHT_BOTTOM)
	{
		for(i = y ; i > 0 ; i--)
		{
			for(j = x ; j > 0 ; j--)
			{
				TFT_WR_Data(Image[((j-1)*x)+(i-1)]);
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


void TFT_DrawImageSmall(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *Image, uint16_t Flag)
{
    uint32_t i, j, n = (xEnd - xStart) * (yEnd - yStart);
	uint32_t x =  xEnd - xStart , y = yEnd - yStart;
	uint16_t data;

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


	if(Flag == BACK_COLOR3)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = ~Image[i];
			if (data==WHITE)	TFT_WR_Data(BACK_COLOR3);
			else 				TFT_WR_Data(BLUE);
		}
	}
	else if(Flag == RED)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = ~Image[i];
			if (data==WHITE)	TFT_WR_Data(data);
			else 				TFT_WR_Data(RED);
		}
	}
	else if(Flag == GREEN)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = ~Image[i];
			if (data==WHITE)	TFT_WR_Data(data);
			else 				TFT_WR_Data(GREEN);
		}
	}
	else if(Flag == BLUE)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = ~Image[i];
			if (data==WHITE)	TFT_WR_Data(data);
			else 				TFT_WR_Data(GREEN);
		}
	}

	else if(Flag == BROWN)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = ~Image[i];
			if (data==WHITE)	TFT_WR_Data(data);
			else 				TFT_WR_Data(BLACK);
		}
	}

	else if(Flag == YELLOW)
	{
		for(i = 0 ; i < n ; ++i)
		{
			data = ~Image[i];
			if (data==WHITE)	TFT_WR_Data(data);
			else 				TFT_WR_Data(GREEN);
		}
	}


	//=================================
	else if(Flag == DRAW_REVERSE)
	{
		for(i = 0 ; i < n ; ++i)
			TFT_WR_Data(~Image[i]);
	}
	else if(Flag == DRAW_RIGHT_TOP)
	{
		for(i = 0 ; i < y ; i++)
		{
			for(j = x ; j > 0 ; j--)
			{
				TFT_WR_Data(Image[(i*x)+(j-1)]);
			}
		}
	}
	else if(Flag == DRAW_LEFT_BOTTOM)
	{
		for(i = y ; i > 0 ; i--)
		{
			for(j = 0 ; j < x ; j++)
			{
				TFT_WR_Data(Image[(j*x)+(i-1)]);
			}
		}
	}
	else if(Flag == DRAW_RIGHT_BOTTOM)
	{
		for(i = y ; i > 0 ; i--)
		{
			for(j = x ; j > 0 ; j--)
			{
				TFT_WR_Data(Image[((j-1)*x)+(i-1)]);
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
			TFT_WR_Data(~Image[i]);
	}
}


void TFT_DrawImage(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *Image, uint16_t Flag)
{
    uint32_t i, j, n = (xEnd - xStart) * (yEnd - yStart);
	uint32_t x =  xEnd - xStart , y = yEnd - yStart;
	uint16_t data;

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
	else if(Flag == DRAW_RIGHT_TOP)
	{
		for(i = 0 ; i < y ; i++)
		{
			for(j = x ; j > 0 ; j--)
			{
				TFT_WR_Data(Image[(i*x)+(j-1)]);
			}
		}
	}
	else if(Flag == DRAW_LEFT_BOTTOM)
	{
		for(i = y ; i > 0 ; i--)
		{
			for(j = 0 ; j < x ; j++)
			{
				TFT_WR_Data(Image[(j*x)+(i-1)]);
			}
		}
	}
	else if(Flag == DRAW_RIGHT_BOTTOM)
	{
		for(i = y ; i > 0 ; i--)
		{
			for(j = x ; j > 0 ; j--)
			{
				TFT_WR_Data(Image[((j-1)*x)+(i-1)]);
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

/* TFT_Init: SSD1963 PLL + timing init.
 * GCC noinline + O0 ensures register-write sequence and delays are exact. */
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((noinline, optimize("O0")))
#endif
void TFT_Init(void)
{
    lcd_rst();

#ifndef 		SIZE3_2INCH
	TFT_WR_REG(0x00E2);	//PLL multiplier, set PLL clock to 120M
	TFT_WR_Data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	TFT_WR_Data(0x0002);
	TFT_WR_Data(0x0004);

	TFT_WR_REG(0x00E0);  // PLL enable
	TFT_WR_Data(0x0001);
	TFT_DelayTime(1);
	TFT_WR_REG(0x00E0);
	TFT_WR_Data(0x0003);
	TFT_DelayTime(5);
	TFT_WR_REG(0x0001);  // software reset
	TFT_DelayTime(5);
	TFT_WR_REG(0x00E6);	//PLL setting for PCLK, depends on resolution
	/////////////////////4.3Inch
	#ifdef SIZE4_3INCH
		TFT_WR_Data(0x0001);
		TFT_WR_Data(0x0033);
		TFT_WR_Data(0x0032);
	#endif
	////////////////////////7Inch
	#ifdef SIZE7_0INCH
		TFT_WR_Data(0x0004);
		TFT_WR_Data(0x0093);
		TFT_WR_Data(0x00e0);
	#endif
	///////////////////////////////
	TFT_WR_REG(0x00B0);	//LCD SPECIFICATION
	TFT_WR_Data(0x0023);
	TFT_WR_Data(0x0000);
	TFT_WR_Data((HDP>>8)&0X00FF);  //Set HDP
	TFT_WR_Data(HDP&0X00FF);
    TFT_WR_Data((VDP>>8)&0X00FF);  //Set VDP
	TFT_WR_Data(VDP&0X00FF);
    TFT_WR_Data(0x0000);

	TFT_WR_REG(0x00B4);	//HSYNC
	TFT_WR_Data((HT>>8)&0X00FF);  //Set HT
	TFT_WR_Data(HT&0X00FF);
	TFT_WR_Data((HPS>>8)&0X00FF);  //Set HPS
	TFT_WR_Data(HPS&0X00FF);
	TFT_WR_Data(HPW);			   //Set HPW
	TFT_WR_Data((LPS>>8)&0X00FF);  //Set HPS
	TFT_WR_Data(LPS&0X00FF);
	TFT_WR_Data(0x0000);

	TFT_WR_REG(0x00B6);	//VSYNC
	TFT_WR_Data((VT>>8)&0X00FF);   //Set VT
	TFT_WR_Data(VT&0X00FF);
	TFT_WR_Data((VPS>>8)&0X00FF);  //Set VPS
	TFT_WR_Data(VPS&0X00FF);
	TFT_WR_Data(VPW);			   //Set VPW
	TFT_WR_Data((FPS>>8)&0X00FF);  //Set FPS
	TFT_WR_Data(FPS&0X00FF);


	TFT_WR_REG(0x00B8);
	TFT_WR_Data(0x0007);    //GPIO3=input, GPIO[2:0]=output
	TFT_WR_Data(0x0001);    //GPIO0 normal

	TFT_WR_REG(0x00BA);
	TFT_WR_Data(0x000F);    //GPIO[3:0] out 1

	TFT_WR_REG(0x0036); //rotation
	TFT_WR_Data(0x0000);	

	TFT_WR_REG(0x00F0); //pixel data interface
	TFT_WR_Data(0x0003);

	TFT_WR_REG(0x0021);	//����ͼ����?��?תģʽ

    TFT_WR_REG(0x00BC);//��Ҫ
	TFT_WR_Data(0x0080);//�Աȶ�
    TFT_WR_Data(0x0080);//����
    TFT_WR_Data(0x0080);//���Ͷ�ֵ  //
    TFT_WR_Data(0x0001);//����������
	TFT_DelayTime(5);
	TFT_WR_REG(0x0029); //display on
#endif
#ifdef SIZE3_2INCH
	TFT_WR_REG(0x0000);			TFT_WR_Data(0x0001);
	TFT_WR_REG(0x0003);			TFT_WR_Data(0xA8A4);
	TFT_WR_REG(0x000C);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x000D);		TFT_WR_Data(0x080C);
	TFT_WR_REG(0x000E);		TFT_WR_Data(0x2B00);
	TFT_WR_REG(0x001E);		TFT_WR_Data(0x00B0);

	TFT_WR_REG(0x0001);		TFT_WR_Data(0x2B3F);
	TFT_WR_REG(0x0002);		TFT_WR_Data(0x0600);
	TFT_WR_REG(0x0010);		TFT_WR_Data(0x0000);

	TFT_WR_REG(0x0011);		TFT_WR_Data(0x6070);

	TFT_WR_REG(0x0005);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0006);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0016);		TFT_WR_Data(0xEF1C);

	TFT_WR_REG(0x0017);		TFT_WR_Data(0x0003);
	TFT_WR_REG(0x0007);		TFT_WR_Data(0x0233);

	TFT_WR_REG(0x000B);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x000F);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0041);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0042);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0048);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0049);		TFT_WR_Data(0x013F);
	TFT_WR_REG(0x004A);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x004B);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0044);		TFT_WR_Data(0xEF00);
	TFT_WR_REG(0x0045);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0046);		TFT_WR_Data(0x013F);
	TFT_WR_REG(0x0030);		TFT_WR_Data(0x0707);
	TFT_WR_REG(0x0031);		TFT_WR_Data(0x0204);
	TFT_WR_REG(0x0032);		TFT_WR_Data(0x0204);
	TFT_WR_REG(0x0033);		TFT_WR_Data(0x0502);
	TFT_WR_REG(0x0034);		TFT_WR_Data(0x0507);
	TFT_WR_REG(0x0035);		TFT_WR_Data(0x0204);
	TFT_WR_REG(0x0036);		TFT_WR_Data(0x0204);
	TFT_WR_REG(0x0037);		TFT_WR_Data(0x0502);
	TFT_WR_REG(0x003A);		TFT_WR_Data(0x0302);
	TFT_WR_REG(0x003B);		TFT_WR_Data(0x0302);
	TFT_WR_REG(0x0023);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0024);		TFT_WR_Data(0x0000);
	TFT_WR_REG(0x0025);		TFT_WR_Data(0x8000);

	TFT_WR_REG(0x004F);	ZA	TFT_WR_Data(0x0000);
	TFT_WR_REG(0x004E);		TFT_WR_Data(0x0000);

	TFT_WR_REG(0x0022);
#endif
}


/* ===================================================== */
/* 2-2. TFT_clear	                     	         	 */
/* ===================================================== */
void TFT_clear(uint16_t color)
{                    
    uint16_t i,l,w;

	l=HDP+1;
	w=VDP+1;


  	TFT_WR_REG(SET_CMD_ADDR);	
    TFT_WR_Data(0);	    
	TFT_WR_Data(0);
	TFT_WR_Data(HDP>>8);	    
	TFT_WR_Data(HDP&0x00ff);

    TFT_WR_REG(SET_PAGE_ADDR);	
	TFT_WR_Data(0);	    
	TFT_WR_Data(0);
	TFT_WR_Data(VDP>>8);	    
	TFT_WR_Data(VDP&0x00ff);


	TFT_WR_REG(WRITE_MEM_START);	
    while(l--) {
	      for(i=0;i<w;i++) TFT_WR_Data(color);
    } 

}


/* ===================================================== */
/* 2-3. TFT_Fill	                     	         	 */
/* ===================================================== */
//  (xend-xsta)*(yend-ysta)
void TFT_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{                    
    unsigned long n;

	n=(unsigned long)(yend-ysta)*(xend-xsta);

//	TFT_WR_CMD(0x0050, xsta); // Horizontal GRAM Start Address
//	TFT_WR_CMD(0x0051, xend); // Horizontal GRAM End Address
//	TFT_WR_CMD(0x0052, ysta); // Vertical GRAM Start Address
//	TFT_WR_CMD(0x0053, yend); // Vertical GRAM Start Address	 
//	TFT_WR_CMD(32, xsta);
//   	TFT_WR_CMD(33, ysta);


	TFT_WR_REG(SET_CMD_ADDR);	
	TFT_WR_Data(xsta>>8);	    
	TFT_WR_Data(xsta&0x00ff);
	TFT_WR_Data((xend-1)>>8);	    
	TFT_WR_Data((xend-1)&0x00ff);

    TFT_WR_REG(SET_PAGE_ADDR);	
	TFT_WR_Data(ysta>>8);	    
	TFT_WR_Data(ysta&0x00ff);
	TFT_WR_Data((yend-1)>>8);	    
	TFT_WR_Data((yend-1)&0x00ff);

	TFT_WR_REG(WRITE_MEM_START);

	while(n--)TFT_WR_Data(color);//��ʾ��?? 

} 


/* ===================================================== */
/* 2-4. TFT_DelayTime                                    */
/* ===================================================== */
/* GCC -O1 optimizes plain loop variables away.
 * Fix: volatile counter + __attribute__((optimize("O0"))) prevent removal.
 * Scale: 72MHz / ~3 cycles per iteration = 24M iter/sec.
 * 1 unit => cnt=72000 => ~3ms.  Keil was ~1ms per unit at 0-opt. */
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((noinline, optimize("O0")))
#endif
void TFT_DelayTime(uint16_t i)
{
    volatile uint32_t cnt;
    for (cnt = (uint32_t)i * 72000UL; cnt != 0U; cnt--)
        ;
}

void TFT_Point(uint16_t x,uint16_t y,uint16_t fontcolor)
{
	   TFT_WR_REG(SET_CMD_ADDR);	
	   TFT_WR_Data(x>>8);	    
	   TFT_WR_Data(x&0x00ff);
	   TFT_WR_Data(HDP>>8);	    
	   TFT_WR_Data(HDP&0x00ff);

       TFT_WR_REG(SET_PAGE_ADDR);	
	   TFT_WR_Data(y>>8);	    
	   TFT_WR_Data(y&0x00ff);
	   TFT_WR_Data(VDP>>8);	    
	   TFT_WR_Data(VDP&0x00ff);
	   TFT_WR_REG(WRITE_MEM_START);	

	   TFT_WR_Data(fontcolor); 

}  

/* Delay loop: __IO (volatile) prevents GCC from removing iterations.
 * 72MHz, ~3 cycles/iter -> 1,000,000 counts ~= 42ms.               */
void Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--)
        ;
}

/* lcd_rst: SSD1963 datasheet requires
 *   RST LOW  >= 10us  (we use ~14ms: Delay(1000000))
 *   RST HIGH >= 120ms before first command (we use ~140ms: Delay(10000000))
 * GCC __attribute__((optimize("O0"))) prevents loop unrolling/removal.   */
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((noinline, optimize("O0")))
#endif
void lcd_rst(void)
{
#ifndef NEW_BOARD
    GPIO_ResetBits(GPIOG, GPIO_Pin_8);
    Delay(1000000UL);                   /* ~14ms  RST LOW  */
    GPIO_SetBits(GPIOG, GPIO_Pin_8);
    Delay(10000000UL);                  /* ~140ms RST stabilize */
#else
    /* SUB B/D Ver1.161224: LCD RST = PD3 */
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    Delay(1000000UL);                   /* ~14ms  RST LOW  */
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
    Delay(10000000UL);                  /* ~140ms RST stabilize */
#endif
}

