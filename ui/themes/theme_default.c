#include <stdio.h>
#include "theme.h"

// Console-only for now: GX is not initialised yet, so themes use
// ANSI console colours instead of GX drawing.

static void drawBackground_default(void) {
    printf("\x1b[40m\x1b[37m");   // black background, white text
}

static void drawHighlight_default(int x, int y, int w, int h) {
    (void)w; (void)h;
    printf("\x1b[%d;%dH\x1b[36m>\x1b[37m", y, x);
}

UITheme theme_default = {
    .bgColor = {0,0,0,255},
    .textColor = {255,255,255,255},
    .highlightColor = {50,50,200,180},
    .fontSize = 16,
    .lineSpacing = 24,
    .drawBackground = drawBackground_default,
    .drawHighlight = drawHighlight_default
};
