static void drawBackground_retro(void) {
    draw_checkerboard();
}

static void drawHighlight_retro(int x, int y, int w, int h) {
    draw_rect(x, y, w, h, (GXColor){255,255,0,200});
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
