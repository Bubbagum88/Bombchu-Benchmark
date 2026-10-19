#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <ogcsys.h>

#include "ui_engine.h"        // Console drawing primitives + theme support
#include "ui_theme_selector.h"
#include "ui_benchmark_selector.h"
#include "theme.h"            // For currentTheme and theme definitions

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// State machine
typedef enum {
    STATE_MENU,
    STATE_BENCHMARK_SELECTOR,
    STATE_THEME_SELECTOR,
    STATE_EXIT
} AppState;

static AppState currentState = STATE_MENU;
static int menuSelection = 0;

static const char *menuItems[] = {
    "Run Benchmark",
    "Open Theme Selector",
    "Exit"
};

#define MENU_COUNT ((int)(sizeof(menuItems) / sizeof(menuItems[0])))

static void draw_main_menu(void) {
    ui_clear_screen();
    ui_draw_title("Bombchu Benchmark v0.0.1 (Early Development)");
    ui_draw_menu(menuItems, MENU_COUNT, menuSelection);
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);

    // Allocate framebuffer
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    // Initialize console (all UI is console text for now)
    CON_Init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    // Initialize input
    PAD_Init();

    int redraw = 1;

    while (1) {
        if (currentState == STATE_MENU && redraw) {
            draw_main_menu();
            redraw = 0;
        }

        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        // Basic global exit
        if (down & PAD_BUTTON_START) {
            currentState = STATE_EXIT;
        }

        switch (currentState) {
            case STATE_MENU:
                if (down & PAD_BUTTON_UP && menuSelection > 0) {
                    menuSelection--;
                    redraw = 1;
                }
                if (down & PAD_BUTTON_DOWN && menuSelection < MENU_COUNT - 1) {
                    menuSelection++;
                    redraw = 1;
                }

                if (down & PAD_BUTTON_A) {
                    if (menuSelection == 0) {
                        currentState = STATE_BENCHMARK_SELECTOR;
                    } else if (menuSelection == 1) {
                        currentState = STATE_THEME_SELECTOR;
                    } else if (menuSelection == 2) {
                        currentState = STATE_EXIT;
                    }
                }
                break;

            case STATE_BENCHMARK_SELECTOR:
                ui_benchmark_selector();   // Blocks until the user backs out
                currentState = STATE_MENU;
                redraw = 1;
                break;

            case STATE_THEME_SELECTOR:
                ui_theme_selector();   // Blocks until the user exits the selector
                currentState = STATE_MENU;
                menuSelection = 1;     // Return to the theme option
                redraw = 1;
                break;

            case STATE_EXIT:
                VIDEO_SetBlack(TRUE);
                VIDEO_Flush();
                exit(0);
                break;
        }
    }

    return 0;
}
