#include <string.h>
#include "benchmark.h"

static BenchmarkResult global_result;

void run_benchmark(const Benchmark* bench, void (*progress_cb)(float)) {
    memset(&global_result, 0, sizeof(global_result));
    global_result.name = bench->name;

    bench->setup();
    bench->run(&global_result, progress_cb);
    bench->teardown();
}

BenchmarkResult* get_last_result(void) {
    return &global_result;
}
