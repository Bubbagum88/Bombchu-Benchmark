#include "ui_theme_selector.h"
#include "ui_engine.h"
#include "themes/theme_default.h"
#include "themes/theme_dark.h"
#include "themes/theme_retro.h"
#include "themes/theme_minimal.h"

void ui_theme_selector(void) {

    const char* names[] = {
        "Default",
        "Dark",
        "Retro",
        "Minimal"
    };

    Theme* themes[] = {
        &theme_default,
        &theme_dark,
        &theme_retro,
        &theme_minimal
    };

    const int theme_count = sizeof(themes) / sizeof(themes[0]);
    int selected = 0;

    while (1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        if (down & PAD_BUTTON_DOWN && selected < theme_count - 1)
            selected++;

        if (down & PAD_BUTTON_UP && selected > 0)
            selected--;

        if (down & PAD_BUTTON_A) {
            currentTheme = themes[selected];
            return;
        }

        ui_draw_theme_menu(names, selected);
    }
}
