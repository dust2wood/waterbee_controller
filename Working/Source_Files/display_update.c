/**
 * display_update.c - Dispatches to channel-specific display based on GetUiMode()
 */
#include "display_update.h"
#include "ui_mode.h"

void Display_Update(void)
{
    switch (GetUiMode()) {
        case UI_MODE_1CH:  ReDisplay_ch1(); break;
        case UI_MODE_2CH:  ReDisplay_ch2(); break;
        case UI_MODE_4CH:  ReDisplay_ch4(); break;
    }
}
