/**
 * display_update.c - Dispatches to channel-specific display based on GetUiMode()
 * 전략 C: 값 변경 시에만 갱신 (CPU/NAND 부하 감소)
 */
#include "display_update.h"
#include "ui_mode.h"
#include "Main.h"
#include "Sensor_Manager.h"

void Display_Update(void)
{
    if (GetUiMode() == UI_MODE_1CH)
        ReDisplay_ch1();
    else
        ReDisplay_ch2();  /* 2+ sensors -> 2CH; 4CH not used in this version */
}

/* 전략 C: 이전 값 저장, 변경 시에만 갱신 */
static uint32_t s_prev_S1PPM, s_prev_S2PPM;
static int16_t s_prev_temp, s_prev_temp1;
static uint32_t s_prev_ntu;  /* 4CH NTU (water_data.ntu * 1000) */
static uint8_t s_force_next;

void Display_NotifyScreenCleared(void)
{
    s_force_next = 1;
}

static int display_needs_update(void)
{
    if (s_force_next) return 1;
    if (currentData.S1PPM != s_prev_S1PPM) return 1;
    if (currentData.S2PPM != s_prev_S2PPM) return 1;
    if (currentData.temperature != s_prev_temp) return 1;
    if (currentData.temperature1 != s_prev_temp1) return 1;
    /* 4CH NTU */
    uint32_t ntu_int = (uint32_t)(water_data.ntu * 1000.0f);
    if (ntu_int != s_prev_ntu) return 1;
    return 0;
}

static void display_save_values(void)
{
    s_prev_S1PPM = currentData.S1PPM;
    s_prev_S2PPM = currentData.S2PPM;
    s_prev_temp = currentData.temperature;
    s_prev_temp1 = currentData.temperature1;
    s_prev_ntu = (uint32_t)(water_data.ntu * 1000.0f);
    s_force_next = 0;
}

void Display_UpdateIfChanged(void)
{
    if (!display_needs_update()) return;
    display_save_values();
    Display_Update();
}
