/**
 * display_update.c - Dispatches to channel-specific display based on GetUiMode().
 * On layout change, clears screen so the new layout draws cleanly.
 */
#include "display_update.h"
#include "ui_mode.h"
#include "Main.h"

static ui_mode_t s_prev_mode = (ui_mode_t)0xFF;  /* invalid sentinel */

void Display_Update(void)
{
	IWDG_ReloadCounter();
    ui_mode_t mode = GetUiMode();

    if (mode != s_prev_mode) {
        /* Layout changed: full screen clear before redraw */
        TFT_clear(WHITE);
        s_prev_mode = mode;
    }

    switch (mode) {
        case UI_MODE_1CH:  ReDisplay_ch1(); break;
        case UI_MODE_2CH:  ReDisplay_ch2(); break;
        case UI_MODE_4CH:  ReDisplay_ch4(); break;
    }
}
