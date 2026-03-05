#include "Button.h"

uint32_t prevButton = 0;
uint32_t ButtonTimer = 0;

uint32_t CheckButton(void)
{
uint32_t button = 0, retVal = 0;

#ifndef NEW_BOARD	
	button = (~(GPIOF->IDR))&0xFF;
#else
	button = (~(GPIOE->IDR))&0xFF;
	button = (button<<1);
	if (((GPIOB->IDR) & 0x200) ==0) button|=1;
#endif

	if (ButtonTimer==0 && button!=0) {
		retVal = prevButton;
		ButtonTimer=15;	// 15* 10msec=150msec
		buz_time=20;	// 20*10msec=200msec
	}

	if(prevButton == 0 && button != 0) {
		retVal = button;
		ButtonTimer=30;	// 70* 10msec=700msec
		buz_time=20;	// 20*10msec=200msec
	}

//	if(prevButton != 0 && button == 0) {
//		retVal = prevButton;
//		ButtonTimer=50;	// 50* 10msec=500msec
//	}

	prevButton =  button;
	return retVal;
}

