#include "benchmark.h"
#include <ogc/lwp_watchdog.h>
#include <stdio.h>
#include <stdlib.h>

static void storage_setup(void) {
    // TODO: setup code
}

static void storage_run(BenchmarkResult* result, void (*progress)(float)) {
    const int steps = 100;

    u64 start = gettime();

    for (int i = 0; i < steps; i++) {
        // TODO: benchmark logic
        progress((float)i / steps);
    }

    u64 end = gettime();
    float ms = ticks_to_millisecs(end - start);

    result->name = "Storage Benchmark";
    result->duration_ms = ms;
    result->throughput_mb_s = 0.0f; // TODO: replace
    result->success = 1;
}

static void storage_teardown(void) {
    // TODO: teardown code
}

Benchmark _BENCHMARK = {
    .name = "Storage Benchmark",
    .setup = storage_setup,
    .run = storage_run,
    .teardown = storage_teardown
};
