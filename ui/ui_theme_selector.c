#include <gccore.h>
#include <stdio.h>
#include "ui_engine.h"
#include "theme.h"
#include "ui_theme_selector.h"

// List of theme pointers
static UITheme *themes[] = {
    &theme_default,
    &theme_dark,
    &theme_retro,
    &theme_minimal
};

static const char *themeNames[] = {
    "Default",
    "Dark",
    "Retro",
    "Minimal"
};

#define THEME_COUNT (sizeof(themes) / sizeof(themes[0]))

void ui_theme_selector(void) {
    int selected = 0;

    while (1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        // Navigation
        if (down & PAD_BUTTON_DOWN) selected++;
        if (down & PAD_BUTTON_UP)   selected--;

        if (selected < 0) selected = 0;
        if (selected > THEME_COUNT - 1) selected = THEME_COUNT - 1;

        // Apply theme
        if (down & PAD_BUTTON_A) {
            currentTheme = themes[selected];
            return; // Return to caller (main menu)
        }

        // Draw screen
        printf("\x1b[2J");
        ui_draw_title("Select Theme");

        ui_draw_menu(themeNames, THEME_COUNT, selected);

        printf("\n\nPress A to apply theme");
        printf("\nPress B to cancel");
        
        if (down & PAD_BUTTON_B) {
            return; // Cancel
        }
    }
}
