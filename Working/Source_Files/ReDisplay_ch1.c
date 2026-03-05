/**
 * ReDisplay_ch1.c - 1-channel UI (single sensor centered)
 * Reuses 2CH layout helpers; draws at left (CH1_X) for single sensor.
 */
#include "Main.h"
#include "NANDDisplay.h"
#include "sensor_autodetect.h"
#include <stdio.h>

#define CH1_X  80
#define CH1_Y  90

void ReDisplay_ch1(void)
{
    if (sensor_autodetect_ph) {
        if (currentData.S1PPM >= 1400)
            sprintf(strBuffer, "14.00");
        else
            sprintf(strBuffer, "%2d.%02d", currentData.S1PPM / 100, currentData.S1PPM % 100);
        display_2ch_ph();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_ph();
    } else if (sensor_autodetect_ec) {
        if (currentData.S2PPM >= 20000)
            sprintf(strBuffer, "2000.0");
        else
            sprintf(strBuffer, "%4d.%01d", currentData.S2PPM / 10, currentData.S2PPM % 10);
        display_2ch_ec();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_ec();
    } else if (sensor_autodetect_adc) {
        if (currentData.S2PPM >= 99999)
            sprintf(strBuffer, "99.999 ");
        else
            sprintf(strBuffer, "%2d.%03d ", currentData.S2PPM / 1000, currentData.S2PPM % 1000);
        display_2ch_tu();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_tu();
    }
}
