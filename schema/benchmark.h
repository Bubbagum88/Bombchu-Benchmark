// src/benchmark.h
#ifndef BENCHMARK_H
#define BENCHMARK_H

typedef struct BenchmarkResult {
    const char* name;
    float score;
    float duration_ms;
    float throughput_mb_s;
    int success;
} BenchmarkResult;

typedef struct Benchmark {
    const char* name;

    void (*setup)(void);
    void (*run)(BenchmarkResult* result, void (*progress_cb)(float));
    void (*teardown)(void);
} Benchmark;

#endif
