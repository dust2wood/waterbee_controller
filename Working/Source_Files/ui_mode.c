/**
 * ui_mode.c - UI mode from sensor count
 */
#include "ui_mode.h"
#include "Sensor_Manager.h"

ui_mode_t GetUiMode(void)
{
    int n = sensor_manager_count();
    if (n <= 1) return UI_MODE_1CH;
    if (n == 2) return UI_MODE_2CH;
    return UI_MODE_4CH;
}
