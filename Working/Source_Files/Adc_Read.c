/**
 * Adc_Read.c - ADC channel read (4-20mA input, Channel 10)
 */
#include "Adc_Read.h"
#include "stm32f10x_adc.h"

uint16_t Read_Channel_ADC1(void)
{
    return ADC_GetConversionValue(ADC1);
}
