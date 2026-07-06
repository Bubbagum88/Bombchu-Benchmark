static void drawBackground_default(void) {
    // Solid color background
    GX_SetCopyClear((GXColor){0,0,0,255}, 0x00ffffff);
}

static void drawHighlight_default(int x, int y, int w, int h) {
    draw_rect(x, y, w, h, (GXColor){50,50,200,180});
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
