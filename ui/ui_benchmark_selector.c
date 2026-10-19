#include <gccore.h>
#include <stdio.h>
#include "ui_engine.h"
#include "ui_benchmark_selector.h"
#include "ui_progress_callback.h"
#include "storage_benchmark.h"

void ui_benchmark_selector(void) {

    // CPU, network and filesystem benchmarks go here once they exist
    Benchmark* benches[] = {
        &STORAGE_BENCHMARK
    };

    const int count = sizeof(benches) / sizeof(benches[0]);
    const char* names[sizeof(benches) / sizeof(benches[0])];
    for (int i = 0; i < count; i++)
        names[i] = benches[i]->name;

    int selected = 0;
    int redraw = 1;

    while (1) {
        if (redraw) {
            ui_clear_screen();
            ui_draw_title("Select Benchmark");
            ui_draw_menu(names, count, selected);
            ui_draw_text(4, 6 + count, "A: run   B: back");
            redraw = 0;
        }

        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        if (down & PAD_BUTTON_DOWN && selected < count - 1) {
            selected++;
            redraw = 1;
        }

        if (down & PAD_BUTTON_UP && selected > 0) {
            selected--;
            redraw = 1;
        }

        if (down & PAD_BUTTON_B)
            return;

        if (down & PAD_BUTTON_A) {
            ui_clear_screen();
            ui_draw_title("Running benchmark...");
            run_benchmark(benches[selected], ui_progress_callback);
            ui_show_results_screen(get_last_result());
            redraw = 1;
        }
    }
}
