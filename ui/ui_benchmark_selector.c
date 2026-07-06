#include <gccore.h>
#include <ogc/pad.h>
#include "ui_engine.h"
#include "../benchmarks/storage_benchmark.h"
#include "../benchmarks/cpu_benchmark.h"
#include "../benchmarks/network_benchmark.h"
#include "../benchmarks/filesystem_benchmark.h"

extern Benchmark STORAGE_BENCHMARK;
extern Benchmark CPU_BENCHMARK;
extern Benchmark NETWORK_BENCHMARK;
extern Benchmark FILESYSTEM_BENCHMARK;

void ui_benchmark_selector(void) {

    const char* names[] = {
        "Storage Benchmark",
        "CPU Benchmark",
        "Network Benchmark",
        "Filesystem Benchmark"
    };

    Benchmark* benches[] = {
        &STORAGE_BENCHMARK,
        &CPU_BENCHMARK,
        &NETWORK_BENCHMARK,
        &FILESYSTEM_BENCHMARK
    };

    const int count = sizeof(benches) / sizeof(benches[0]);
    int selected = 0;

    while (1) {
        VIDEO_WaitVSync();
        PAD_ScanPads();
        u32 down = PAD_ButtonsDown(0);

        if (down & PAD_BUTTON_DOWN && selected < count - 1)
            selected++;

        if (down & PAD_BUTTON_UP && selected > 0)
            selected--;

        if (down & PAD_BUTTON_A) {
            run_benchmark(benches[selected], ui_progress_callback);
            ui_show_results_screen(get_last_result());
            return;
        }

        ui_draw_benchmark_menu(names, selected);
    }
}
