#include "benchmark.h"
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <ogc/lwp_watchdog.h>

#define TEST_FILE   "sd:/bombchu_storage_test.bin"
#define BLOCK_SIZE  (64 * 1024)     // 64KB
#define BLOCK_COUNT 128             // 8MB total

static void storage_setup(void) {
    fatInitDefault();
}

static void storage_run(BenchmarkResult* result, void (*progress)(float)) {

    // Allocate buffer
    char* buffer = malloc(BLOCK_SIZE);
    if (!buffer) {
        result->success = 0;
        return;
    }

    // Fill buffer with deterministic data
    for (int i = 0; i < BLOCK_SIZE; i++)
        buffer[i] = (char)(i & 0xFF);

    // -----------------------------
    // WRITE TEST
    // -----------------------------
    FILE* f = fopen(TEST_FILE, "wb");
    if (!f) {
        free(buffer);
        result->success = 0;
        return;
    }

    u64 write_start = gettime();

    for (int i = 0; i < BLOCK_COUNT; i++) {
        fwrite(buffer, 1, BLOCK_SIZE, f);
        progress((float)i / (BLOCK_COUNT * 2));   // 0.0 → 0.5
    }

    fclose(f);

    u64 write_end = gettime();
    float write_ms = ticks_to_millisecs(write_end - write_start);

    // -----------------------------
    // READ TEST
    // -----------------------------
    f = fopen(TEST_FILE, "rb");
    if (!f) {
        free(buffer);
        result->success = 0;
        return;
    }

    u64 read_start = gettime();

    for (int i = 0; i < BLOCK_COUNT; i++) {
        fread(buffer, 1, BLOCK_SIZE, f);
        progress(0.5f + (float)i / (BLOCK_COUNT * 2)); // 0.5 → 1.0
    }

    fclose(f);

    u64 read_end = gettime();
    float read_ms = ticks_to_millisecs(read_end - read_start);

    // -----------------------------
    // RESULTS
    // -----------------------------
    float total_mb = (BLOCK_SIZE * BLOCK_COUNT) / (1024.0f * 1024.0f);

    result->name = "Storage Benchmark";
    result->duration_ms = write_ms + read_ms;
    result->throughput_mb_s = total_mb / (result->duration_ms / 1000.0f);
    result->success = 1;

    free(buffer);
}

static void storage_teardown(void) {
    remove(TEST_FILE);
}

Benchmark STORAGE_BENCHMARK = {
    .name = "Storage Benchmark",
    .setup = storage_setup,
    .run = storage_run,
    .teardown = storage_teardown
};
