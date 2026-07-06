#include "benchmark.h"

static BenchmarkResult global_result;

void run_benchmark(const Benchmark* bench, void (*progress_cb)(float)) {
    bench->setup();
    bench->run(&global_result, progress_cb);
    bench->teardown();
}

BenchmarkResult* get_last_result(void) {
    return &global_result;
}
