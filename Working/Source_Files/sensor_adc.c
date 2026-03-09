/**
 * sensor_adc.c - ADC sensor driver (4-20mA input)
 *
 * Reads ADC Channel 10, converts to NTU (or generic 0-20 scale).
 * 12-bit ADC: 0-4095. Typical 4mA~0.1V, 20mA~0.5V -> ~400-2000 raw.
 */
#include "sensor_manager.h"
#include "Main.h"
#include "Adc_Read.h"

#define ADC_MAX      4095
#define ADC_4MA      400
#define ADC_20MA     2000

static void sensor_adc_init(void)
{
    /* ADC configured in User_ADC_Configuration */
}

static float sensor_adc_read(void)
{
    uint16_t raw = Read_Channel_ADC1();
    if (raw < ADC_4MA || raw > ADC_20MA) return SENSOR_INVALID_FLOAT;
    /* Linear 4-20mA -> 0-20.0 NTU (or arbitrary scale) */
    return (float)(raw - ADC_4MA) * 20.0f / (float)(ADC_20MA - ADC_4MA);
}

static uint8_t sensor_adc_status(void)
{
    uint16_t raw = Read_Channel_ADC1();
    return (raw >= ADC_4MA && raw <= ADC_20MA) ? 0 : 1;
}

const sensor_driver_t sensor_adc_driver = {
    .init   = sensor_adc_init,
    .read   = sensor_adc_read,
    .status = sensor_adc_status,
};
