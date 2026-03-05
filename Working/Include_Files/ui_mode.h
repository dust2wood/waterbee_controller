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

/**
 * 1 sensor -> UI_MODE_1CH
 * 2 sensors -> UI_MODE_2CH
 * 3 or more -> UI_MODE_4CH
 */
ui_mode_t GetUiMode(void);

#endif /* UI_MODE_H */
