#include <stdio.h>
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
