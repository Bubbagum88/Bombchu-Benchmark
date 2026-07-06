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
