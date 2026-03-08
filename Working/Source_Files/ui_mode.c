/**
 * ui_mode.c - UI mode from sensor count
 */
#include "ui_mode.h"
#include "Sensor_Manager.h"

ui_mode_t g_ui_mode = UI_MODE_1CH;

ui_mode_t GetUiMode(void)
{
    int n = sensor_manager_count();
    if (n <= 1) g_ui_mode = UI_MODE_1CH;
    else if (n == 2) g_ui_mode = UI_MODE_2CH;
    else g_ui_mode = UI_MODE_4CH;
    return g_ui_mode;
}
