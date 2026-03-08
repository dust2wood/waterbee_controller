/**
 * ui_mode.h - UI layout mode based on detected sensor count
 */
#ifndef UI_MODE_H
#define UI_MODE_H

typedef enum {
    UI_MODE_1CH,
    UI_MODE_2CH,
    UI_MODE_4CH
} ui_mode_t;

extern ui_mode_t g_ui_mode;  /* current UI mode set by GetUiMode() */

/**
 * Detects sensor count via sensor_manager_count(), sets g_ui_mode, and returns it.
 * Call once after sensor_manager_init().
 */
ui_mode_t GetUiMode(void);

#endif /* UI_MODE_H */
