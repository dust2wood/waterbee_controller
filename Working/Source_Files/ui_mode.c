/**
 * ui_mode.c - UI mode from live sensor heartbeat timers
 */
#include "ui_mode.h"
#include "Main.h"

ui_mode_t g_ui_mode = UI_MODE_1CH;

ui_mode_t GetUiMode(void)
{
    int n = (Sensor1_OK_TIME > 0 ? 1 : 0)
          + (Sensor2_OK_TIME > 0 ? 1 : 0)
          + (Sensor3_OK_TIME > 0 ? 1 : 0)
          + (Sensor4_OK_TIME > 0 ? 1 : 0);

    if (n <= 1) g_ui_mode = UI_MODE_1CH;
    else if (n == 2) g_ui_mode = UI_MODE_2CH;
    else g_ui_mode = UI_MODE_4CH;
    return g_ui_mode;
}
