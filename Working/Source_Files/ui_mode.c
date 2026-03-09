/**
 * ui_mode.c - UI mode from sensor count
 */
#include "ui_mode.h"
#include "Sensor_Manager.h"

ui_mode_t GetUiMode(void)
{
    int n = sensor_manager_count();
    if (n <= 1) return UI_MODE_1CH;
    /* Max 2 channels in this version; 3+ sensors -> cap at 2CH, no 4CH UI */
    return UI_MODE_2CH;
}
