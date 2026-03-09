#include "Adc_Read.h"
#include "stm32f10x.h"

uint16_t Read_Channel_ADC1(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}
