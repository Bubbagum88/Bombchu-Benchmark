#include <gccore.h>
#include <stdio.h>
#include "ui_engine.h"
#include "theme.h"

// Active theme pointer (set by theme selector)
UITheme *currentTheme = NULL;

// Internal GX frame begin/end helpers
static void ui_begin_frame(void) {
    GX_SetCopyClear(currentTheme->bgColor, 0x00ffffff);
    GX_InvalidateTexAll();
    GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GX_SetColorUpdate(GX_TRUE);
    GX_SetAlphaUpdate(GX_TRUE);
}

static void ui_end_frame(void) {
    GX_DrawDone();
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_Flush();
    VIDEO_WaitVSync();
}

// Draw text using the theme's font settings
void ui_draw_text(int x, int y, const char *msg) {
    // For now, use console text (simple)
    // Later: replace with GX font renderer
    printf("\x1b[%d;%dH%s", y, x, msg);
}

// Draw a menu item with highlight if selected
void ui_draw_menu_item(const char *label, int x, int y, int selected) {
    if (selected) {
        currentTheme->drawHighlight(x - 2, y - 1, 300, currentTheme->lineSpacing);
    }
    ui_draw_text(x, y, label);
}

// Draw a full menu screen
void ui_draw_menu(const char **items, int count, int selected) {
    printf("\x1b[2J");      // Clear screen
    printf("\x1b[2;0H");    // Reset cursor

    currentTheme->drawBackground();

    for (int i = 0; i < count; i++) {
        int y = 4 + i * (currentTheme->lineSpacing / 2);
        ui_draw_menu_item(items[i], 4, y, (i == selected));
    }
}

// Draw a simple centered title
void ui_draw_title(const char *title) {
    printf("\x1b[2;0H");
    ui_draw_text(4, 2, title);
}
