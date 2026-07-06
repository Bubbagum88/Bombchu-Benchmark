void ui_theme_selector(void) {
    const char *themes[] = {
        "Default",
        "Dark",
        "Retro",
        "Minimal"
    };

    int selected = 0;

    while (1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        if (down & PAD_BUTTON_DOWN) selected++;
        if (down & PAD_BUTTON_UP) selected--;

        if (selected < 0) selected = 0;
        if (selected > 3) selected = 3;

        if (down & PAD_BUTTON_A) {
            switch (selected) {
                case 0: currentTheme = &theme_default; break;
                case 1: currentTheme = &theme_dark; break;
                case 2: currentTheme = &theme_retro; break;
                case 3: currentTheme = &theme_minimal; break;
            }
            return;
        }

        ui_draw_theme_menu(themes, selected);
    }
}
