# Bombchu-Benchmark

**Bombchu Benchmark** is an **early-stage** system-level performance profiling tool for the Nintendo GameCube and Wii homebrew ecosystem, built with devkitPro and libogc2.

The goal is to provide standardized benchmarks for storage, network, CPU, and filesystem performance to help homebrew developers optimize their projects.

---

## Current Status

This project is in its very early stages. Only the basic repository structure exists.

**Planned Features** (subject to change):
- Storage benchmarking (SD, USB, etc.)
- Network benchmarking
- CPU benchmarking (PowerPC-focused)
- Filesystem analysis

Expect frequent changes. Feedback and contributions are welcome!

---

## Build Instructions

### Requirements
- [devkitPro](https://devkitpro.org/) (devkitPPC)
- libogc2

### Compile
```bash
make
