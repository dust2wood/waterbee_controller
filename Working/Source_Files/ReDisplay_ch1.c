/**
 * ReDisplay_ch1.c - 1-channel UI (single active sensor, full screen)
 * Picks the first sensor whose OK_TIME > 0 in priority order (1,2,3,4).
 */
#include "Main.h"
#include "NANDDisplay.h"
#include <stdio.h>

#define CH1_X  80
#define CH1_Y  90

void ReDisplay_ch1(void)
{
    if (Sensor1_OK_TIME > 0) {
        /* Sensor 1: pH */
        if (currentData.S1PPM >= 1400)
            sprintf(strBuffer, "14.00");
        else
            sprintf(strBuffer, "%2d.%02d", currentData.S1PPM / 100, currentData.S1PPM % 100);
        display_2ch_ph();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_ph();
    } else if (Sensor2_OK_TIME > 0) {
        /* Sensor 2: EC */
        if (currentData.S2PPM >= 20000)
            sprintf(strBuffer, "2000.0");
        else
            sprintf(strBuffer, "%4d.%01d", currentData.S2PPM / 10, currentData.S2PPM % 10);
        display_2ch_ec();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_ec();
    } else if (Sensor3_OK_TIME > 0) {
        /* Sensor 3: raw mV */
        sprintf(strBuffer, "%5lu", (unsigned long)currentData.S3mV);
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
    } else if (Sensor4_OK_TIME > 0) {
        /* Sensor 4: raw mV */
        sprintf(strBuffer, "%5lu", (unsigned long)currentData.S4mV);
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
    }
}
