#include <stdio.h>
#include "theme.h"

static void drawBackground_retro(void) {
    printf("\x1b[40m\x1b[32m");   // black background, green text
}

static void drawHighlight_retro(int x, int y, int w, int h) {
    (void)w; (void)h;
    printf("\x1b[%d;%dH\x1b[33m>\x1b[32m", y, x);
}

UITheme theme_retro = {
    .bgColor = {20,20,20,255},
    .textColor = {0,255,0,255},
    .highlightColor = {255,255,0,200},
    .fontSize = 14,
    .lineSpacing = 20,
    .drawBackground = drawBackground_retro,
    .drawHighlight = drawHighlight_retro
};
