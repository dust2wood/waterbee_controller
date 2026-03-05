#include "DAC7512Interface.h"


void ControlDAC(uint32_t outDAC)
{
	uint32_t i = 0, andpercent = 0;

	//Cycle1 SYNC LOW
	SYNC1_LOW;
	DelaySPI(3);

	DIN1_LOW;
	/////////////////////////////PD
	for(i = 0 ; i < 4 ; ++i)
	{
		SCLK1_LOW;//Falling

		SCLK1_HIGH;//Rising
	}
	andpercent = 0x1000;
	for(i = 0 ; i < 12 ; ++i)
	{
		andpercent = andpercent >> 1;
		if(outDAC&andpercent)//currentR
			DIN1_HIGH;
		else
			DIN1_LOW;

		SCLK1_LOW;//Falling

		SCLK1_HIGH;//Rising
	}
	//Cycle1 SYNC HIGH
	SYNC1_HIGH;
	DelaySPI(3);
}

void InitDAC(void)//Set Value 2000
{
	SYNC1_LOW;//Sync4
	DelaySPI(3);

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising
	///////////////////////////////////////////////PD
	DIN1_LOW;

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising
	///////////////////////////////////////////////PD END
	DIN1_LOW;//data

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SCLK1_LOW;//Falling	
	SCLK1_HIGH;//Rising

	SYNC1_HIGH;//Sync4
	DelaySPI(3);
}

void DelaySPI(volatile uint32_t t)
{
	uint32_t i;
	for(i = 0 ; i < t ; ++i);
}


