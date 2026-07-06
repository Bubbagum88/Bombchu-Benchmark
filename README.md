> LICENSE:
>     This software is licensed under the GNU General Public License v2
>       which is available at: http://www.gnu.org/licenses/gpl-2.0.txt
>     This requires any released modifications to be licensed similarly,
>       and to have the source available.

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
