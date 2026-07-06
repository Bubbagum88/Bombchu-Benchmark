#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <ogcsys.h>
#include <unistd.h>     // for usleep/sleep

#include "ui_engine.h"        // Your drawing primitives + theme support
#include "ui_theme_selector.h"
#include "theme.h"            // For currentTheme and theme definitions

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// State machine
typedef enum {
    STATE_MENU,
    STATE_RUN_PLACEHOLDER,
    STATE_THEME_SELECTOR,
    STATE_EXIT
} AppState;

static AppState currentState = STATE_MENU;
static int menuSelection = 0;   // For arrow navigation in the future

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

    // Initialize console (fallback/printf support)
    CON_Init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    // Initialize input
    PAD_Init();

    // === UI / Theme initialization ===
    currentTheme = &theme_default;   // Critical: set before any drawing

    printf("\x1b[2;0H");  // Initial positioning
    printf("Bombchu Benchmark v0.0.1 (Early Development)\n");
    printf("======================================\n\n");
    printf("Systems initialized.\n\n");

    while (1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        // Basic global exit (works from anywhere)
        if (down & PAD_BUTTON_START) {
            currentState = STATE_EXIT;
        }

        switch (currentState) {
            case STATE_MENU:
                // Clear / redraw frame
                // (In a full UI engine you'd have a proper clear + draw call)
                ui_draw_title("Bombchu Benchmark");

                const char *menuItems[] = {
                    "Run Placeholder Benchmark",
                    "Open Theme Selector",
                    "Exit"
                };

                ui_draw_menu(menuItems, 3, menuSelection);

                // Navigation (expandable)
                if (down & PAD_BUTTON_UP) {
                    if (menuSelection > 0) menuSelection--;
                }
                if (down & PAD_BUTTON_DOWN) {
                    if (menuSelection < 2) menuSelection++;
                }

                if (down & PAD_BUTTON_A) {
                    if (menuSelection == 0) {
                        currentState = STATE_RUN_PLACEHOLDER;
                    } else if (menuSelection == 1) {
                        currentState = STATE_THEME_SELECTOR;
                    } else if (menuSelection == 2) {
                        currentState = STATE_EXIT;
                    }
                }
                break;

            case STATE_RUN_PLACEHOLDER:
                // TODO: Replace with real benchmark call later
                ui_draw_title("Running Benchmark...");
                printf("Running placeholder benchmark...\n");
                usleep(500000);  // 0.5s visual feedback
                printf("Test complete (stub).\n\n");
                currentState = STATE_MENU;
                menuSelection = 0;  // Reset selection
                break;

            case STATE_THEME_SELECTOR:
                ui_theme_selector();   // This should block until user exits the selector
                currentState = STATE_MENU;
                menuSelection = 1;     // Return to the theme option
                break;

            case STATE_EXIT:
                // Optional cleanup
                VIDEO_SetBlack(TRUE);
                VIDEO_Flush();
                exit(0);
                break;
        }
    }

    return 0;
}#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <ogcsys.h>
#include <unistd.h>   // for sleep/usleep if needed

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
    VIDEO_Init();

    // Get the current video mode
    rmode = VIDEO_GetPreferredMode(NULL);

    // Allocate framebuffer
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    // Configure video
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();

    // Initialize console on framebuffer
    CON_Init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    printf("\x1b[2;0H");  // Position cursor
    printf("Bombchu Benchmark v0.0.1 (Early Development)\n");
    printf("======================================\n\n");

    printf("Initializing systems...\n");

    // Basic PAD support (GameCube controller)
    PAD_Init();

    // TODO: fatInitDefault() for storage tests later

    printf("Ready.\n\n");
    printf("Press A to run basic test (placeholder)\n");
    printf("Press START to exit\n\n");

    while(1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();

        int buttonsDown = PAD_ButtonsDown(0);

        if (buttonsDown & PAD_BUTTON_A) {
            printf("Running placeholder benchmark...\n");
            // TODO: Call your first storage test here
            sleep(1);  // Visual pause
            printf("Test complete (stub).\n\n");
        }

        if (buttonsDown & PAD_BUTTON_START) {
            exit(0);
        }
    }

    return 0;
}
