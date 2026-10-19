#ifndef BENCHMARK_H
#define BENCHMARK_H

typedef struct {
    const char* name;
    float duration_ms;
    float throughput_mb_s;
    int success;
} BenchmarkResult;

typedef struct {
    const char* name;
    void (*setup)(void);
    void (*run)(BenchmarkResult* result, void (*progress)(float));
    void (*teardown)(void);
} Benchmark;

void run_benchmark(const Benchmark* bench, void (*progress_cb)(float));
BenchmarkResult* get_last_result(void);

#endif
