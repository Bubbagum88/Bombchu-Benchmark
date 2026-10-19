#include <gccore.h>
#include <stdio.h>
#include "ui_engine.h"
#include "theme.h"

// Active theme pointer (set by theme selector)
UITheme *currentTheme = &theme_default;

// Clear the console using the active theme's colours
void ui_clear_screen(void) {
    currentTheme->drawBackground();
    printf("\x1b[2J");
}

// Draw text at a console row/column
void ui_draw_text(int x, int y, const char *msg) {
    // For now, use console text (simple)
    // Later: replace with GX font renderer
    printf("\x1b[%d;%dH%s", y, x, msg);
}

// Draw a menu item with highlight if selected
void ui_draw_menu_item(const char *label, int x, int y, int selected) {
    if (selected) {
        currentTheme->drawHighlight(x - 2, y, 300, currentTheme->lineSpacing);
    }
    ui_draw_text(x, y, label);
}

// Draw a menu, one item per console row starting at row 4
void ui_draw_menu(const char **items, int count, int selected) {
    for (int i = 0; i < count; i++) {
        ui_draw_menu_item(items[i], 4, 4 + i, (i == selected));
    }
}

// Draw a simple title on row 2
void ui_draw_title(const char *title) {
    ui_draw_text(4, 2, title);
}

// Show a benchmark result and wait for A or B
void ui_show_results_screen(const BenchmarkResult *result) {
    char line[64];

    ui_clear_screen();
    ui_draw_title(result->name ? result->name : "Benchmark");

    if (result->success) {
        snprintf(line, sizeof(line), "Duration:   %.0f ms", result->duration_ms);
        ui_draw_text(4, 4, line);
        snprintf(line, sizeof(line), "Throughput: %.2f MB/s", result->throughput_mb_s);
        ui_draw_text(4, 5, line);
    } else {
        ui_draw_text(4, 4, "Benchmark failed.");
    }

    ui_draw_text(4, 8, "Press A to return");

    while (1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();
        if (PAD_ButtonsDown(0) & (PAD_BUTTON_A | PAD_BUTTON_B))
            return;
    }
}
