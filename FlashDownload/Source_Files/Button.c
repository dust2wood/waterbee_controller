#include "Button.h"

uint32_t prevButton = 0;

uint32_t CheckButton(void)
{
	uint32_t button = 0, retVal = 0;
	
	button = (~(GPIOF->IDR))&0xFF;
	
	if(prevButton != 0 && button == 0)
		retVal = prevButton;
	
	prevButton =  button;
	return retVal;
}

