#ifndef TOUCH_H
#define TOUCH_H

typedef struct TouchPoint{
	uint16_t X;
	uint16_t Y;
}TouchPoint;





#define TDIN  									(1<<15)  // PB15
#define TCLK  									(1<<13)  // PB13
#define TCS   									(1<<12)  // PB12   			    
#define TDIN_SET(x) 						GPIOB->ODR=(GPIOB->ODR&~TDIN)|(x ? TDIN:0)
#define TCLK_SET(x) 						GPIOB->ODR=(GPIOB->ODR&~TCLK)|(x ? TCLK:0)													    
#define TCS_SET(x)  						GPIOB->ODR=(GPIOB->ODR&~TCS)|(x ? TCS:0)  
#define CMD_RDX 							0xD0
#define CMD_RDY							0x90


void CheckTouchInput(void);
uint16_t CheckTouchArea(TouchPoint touchVal);

void TouchDebuggingFunction(void);
uint32_t ReadTouchScreen(TouchPoint* touchVal);
void TouchCalibration(void);
uint16_t SPI_Flash_SendByte(uint8_t byte);
void start_spi(void);
uint16_t SPI_Flash_ReadByte(uint8_t a);
void delay_us(uint32_t nCount);
TouchPoint read_once(void);



#endif











