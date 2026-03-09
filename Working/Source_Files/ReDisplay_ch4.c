/**
 * ReDisplay_ch4.c - 4-channel UI (2x2 grid for 3+ active sensors)
 * Uses Sensor*_OK_TIME > 0 for runtime presence detection.
 * Disconnected slots show "--".
 */
#include "Main.h"
#include "NANDDisplay.h"
#include <stdio.h>

/* 2x2 grid positions */
#define CH4_X0  30
#define CH4_X1  170
#define CH4_Y0  55
#define CH4_Y1  130

#define CH4_PLACEHOLDER "--"

static void draw_cell(uint16_t x, uint16_t y, const char *str)
{
    Draw_4CH_HOME_Number(x, y, (char *)str, BROWN);
}

void ReDisplay_ch4(void)
{
    /* Slot 0: Sensor 1 (pH) */
    if (Sensor1_OK_TIME > 0) {
        if (currentData.S1PPM >= 1400)
            sprintf(strBuffer, "14.00");
        else
            sprintf(strBuffer, "%2d.%02d", currentData.S1PPM / 100, (int)abs(currentData.S1PPM % 100));
        draw_cell(CH4_X0, CH4_Y0, strBuffer);
    } else {
        draw_cell(CH4_X0, CH4_Y0, CH4_PLACEHOLDER);
    }

    /* Slot 1: Sensor 2 (EC) */
    if (Sensor2_OK_TIME > 0) {
        if (currentData.S2PPM >= 20000)
            sprintf(strBuffer, "2000.0");
        else
            sprintf(strBuffer, "%4d.%01d", currentData.S2PPM / 10, (int)abs(currentData.S2PPM % 10));
        draw_cell(CH4_X1, CH4_Y0, strBuffer);
    } else {
        draw_cell(CH4_X1, CH4_Y0, CH4_PLACEHOLDER);
    }

    /* Slot 2: Sensor 3 (raw mV) */
    if (Sensor3_OK_TIME > 0) {
        sprintf(strBuffer, "%5lu", (unsigned long)currentData.S3mV);
        draw_cell(CH4_X0, CH4_Y1, strBuffer);
    } else {
        draw_cell(CH4_X0, CH4_Y1, CH4_PLACEHOLDER);
    }

    /* Slot 3: Sensor 4 (raw mV) */
    if (Sensor4_OK_TIME > 0) {
        sprintf(strBuffer, "%5lu", (unsigned long)currentData.S4mV);
        draw_cell(CH4_X1, CH4_Y1, strBuffer);
    } else {
        draw_cell(CH4_X1, CH4_Y1, CH4_PLACEHOLDER);
    }
}
