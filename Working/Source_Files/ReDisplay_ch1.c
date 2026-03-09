/**
 * ReDisplay_ch1.c - 1-channel UI (single sensor fullscreen)
 * Uses sensor_manager_get_display_field(0) for display mapping.
 */
#include "Main.h"
#include "NANDDisplay.h"
#include "Sensor_Manager.h"
#include <stdio.h>

#define CH1_X  80
#define CH1_Y  90

void ReDisplay_ch1(void)
{
    water_field_t f;
    if (sensor_manager_count() < 1) return;

    f = sensor_manager_get_display_field(0);

    switch (f) {
    case WATER_FIELD_PH: {
        uint32_t v = (uint32_t)(water_data.ph * 100.0f);
        if (v >= 1400) sprintf(strBuffer, "14.00");
        else sprintf(strBuffer, "%2d.%02d", v / 100, v % 100);
        display_2ch_ph();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_ph();
        break;
    }
    case WATER_FIELD_EC: {
        uint32_t v = (uint32_t)(water_data.ec * 10.0f);
        if (v >= 20000) sprintf(strBuffer, "2000.0");
        else sprintf(strBuffer, "%4d.%01d", v / 10, v % 10);
        display_2ch_ec();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_ec();
        break;
    }
    case WATER_FIELD_NTU: {
        uint32_t v = (uint32_t)(water_data.ntu * 1000.0f);
        if (v >= 99999) sprintf(strBuffer, "99.999 ");
        else sprintf(strBuffer, "%2d.%03d ", v / 1000, v % 1000);
        display_2ch_tu();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_tu();
        break;
    }
    case WATER_FIELD_CL: {
        uint32_t v = (uint32_t)(water_data.cl * 100.0f);
        if (v >= 9999) sprintf(strBuffer, "99.99");
        else sprintf(strBuffer, "%2d.%02d", v / 100, v % 100);
        display_2ch_cl();
        Draw_2CH_HOME_Number(CH1_X, CH1_Y + 20, strBuffer, BROWN);
        display_2ch_unit_cl();
        break;
    }
    default:
        break;
    }
}
