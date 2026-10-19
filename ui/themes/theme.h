#ifndef THEME_H
#define THEME_H

#include <gccore.h>

typedef struct {
    GXColor bgColor;
    GXColor textColor;
    GXColor highlightColor;

    int fontSize;
    int lineSpacing;

    void (*drawBackground)(void);
    void (*drawHighlight)(int x, int y, int w, int h);
} UITheme;

extern UITheme *currentTheme;

extern UITheme theme_default;
extern UITheme theme_retro;

#endif
