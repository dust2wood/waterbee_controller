/**
 * ReDisplay_ch4.c - 4-channel UI (2x2 grid for 3+ sensors)
 * Each channel checks sensor exists before rendering; unused channels show "--".
 * Uses Draw_4CH_HOME_Number (consistent with existing display helpers).
 */
#include "Main.h"
#include "NANDDisplay.h"
#include "sensor_autodetect.h"
#include "Sensor_Manager.h"
#include <stdio.h>

/* 2x2 grid positions: slot 0=pH, 1=EC, 2=NTU, 3=Temp */
#define CH4_X0  30
#define CH4_X1  170
#define CH4_Y0  55
#define CH4_Y1  130

#define CH4_PLACEHOLDER "--"

static void draw_cell(uint16_t x, uint16_t y, const char *str)
{
    Draw_4CH_HOME_Number(x, y, (char *)str, BROWN);
}

static int is_temp_valid(int16_t t)
{
    return (t < 9999 && t > -9999);
}

static int is_ntu_valid(float ntu)
{
    return (ntu != SENSOR_INVALID_FLOAT && ntu >= 0.0f && ntu < 99.999f);
}

void ReDisplay_ch4(void)
{
    /* Slot 0: pH - only if sensor registered */
    if (sensor_autodetect_ph) {
        if (currentData.S1PPM >= 1400)
            sprintf(strBuffer, "14.00");
        else
            sprintf(strBuffer, "%2d.%02d", currentData.S1PPM / 100, currentData.S1PPM % 100);
        draw_cell(CH4_X0, CH4_Y0, strBuffer);
    } else {
        draw_cell(CH4_X0, CH4_Y0, CH4_PLACEHOLDER);
    }

    /* Slot 1: EC - only if sensor registered */
    if (sensor_autodetect_ec) {
        if (currentData.S2PPM >= 20000)
            sprintf(strBuffer, "2000.0");
        else
            sprintf(strBuffer, "%4d.%01d", currentData.S2PPM / 10, currentData.S2PPM % 10);
        draw_cell(CH4_X1, CH4_Y0, strBuffer);
    } else {
        draw_cell(CH4_X1, CH4_Y0, CH4_PLACEHOLDER);
    }

    /* Slot 2: NTU - only if sensor registered and value valid */
    if (sensor_autodetect_adc && is_ntu_valid(water_data.ntu)) {
        float ntu = water_data.ntu;
        sprintf(strBuffer, "%2d.%03d", (int)ntu, (int)(ntu * 1000.0f) % 1000);
        draw_cell(CH4_X0, CH4_Y1, strBuffer);
    } else {
        draw_cell(CH4_X0, CH4_Y1, CH4_PLACEHOLDER);
    }

    /* Slot 3: Temperature - show value if valid, else "--" */
    if (is_temp_valid(currentData.temperature)) {
        int16_t t = currentData.temperature;
        if (t >= 0)
            sprintf(strBuffer, " %02d.%01dC ", t / 10, t % 10);
        else
            sprintf(strBuffer, "-%02d.%01dC ", (-t) / 10, (-t) % 10);
        draw_cell(CH4_X1, CH4_Y1, strBuffer);
    } else {
        draw_cell(CH4_X1, CH4_Y1, CH4_PLACEHOLDER);
    }
}
