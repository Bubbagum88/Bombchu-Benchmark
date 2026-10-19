#include <gccore.h>
#include <stdio.h>
#include "ui_engine.h"
#include "theme.h"
#include "ui_theme_selector.h"

// List of theme pointers
static UITheme *themes[] = {
    &theme_default,
    &theme_retro
};

static const char *themeNames[] = {
    "Default",
    "Retro"
};

#define THEME_COUNT ((int)(sizeof(themes) / sizeof(themes[0])))

void ui_theme_selector(void) {
    int selected = 0;
    int redraw = 1;

    while (1) {
        if (redraw) {
            ui_clear_screen();
            ui_draw_title("Select Theme");
            ui_draw_menu(themeNames, THEME_COUNT, selected);
            ui_draw_text(4, 6 + THEME_COUNT, "A: apply   B: cancel");
            redraw = 0;
        }

        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        // Navigation
        if (down & PAD_BUTTON_DOWN && selected < THEME_COUNT - 1) {
            selected++;
            redraw = 1;
        }
        if (down & PAD_BUTTON_UP && selected > 0) {
            selected--;
            redraw = 1;
        }

        // Apply theme
        if (down & PAD_BUTTON_A) {
            currentTheme = themes[selected];
            return; // Return to caller (main menu)
        }

        if (down & PAD_BUTTON_B) {
            return; // Cancel
        }
    }
}
