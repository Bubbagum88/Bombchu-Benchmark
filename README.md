# Bombchu Benchmark

**Bombchu Benchmark** is an early-stage system-level performance profiling tool for the Nintendo GameCube and Wii homebrew ecosystem, built with devkitPro and libogc2.

The goal is to provide standardized benchmarks for storage, network, CPU, and filesystem performance to help homebrew developers optimize their projects.

---

## Current Status

Early development. What works today:

- Builds for both Wii and GameCube, against libogc2
- A text-console main menu with a benchmark selector, results screen and theme selector
- A storage benchmark (SD card write and read throughput)

## Planned Features (subject to change)

- Storage benchmarking for USB, and more accurate SD numbers
- Network benchmarking
- CPU benchmarking (PowerPC-focused)
- Filesystem benchmarking
- GX-based UI engine and themes
- On-device visualization of benchmark results

---

## Repository Structure

```
benchmarks/          Benchmark modules (storage_benchmark.c)
include/             Shared headers (benchmark.h, storage_benchmark.h)
src/                 Entry point and benchmark dispatcher
ui/                  Console UI: menus, selectors, results screen
ui/themes/           Theme definitions
images/bombchu/      Bombchu ASCII art frames
images/title/        Title screen ASCII art frames
images/previews/     Rendered previews of the ASCII art
images/source/       Source artwork the ASCII art was made from
docs/                Developer guides
.github/workflows/   GitHub Actions build
Makefile             Build script (devkitPPC + libogc2)
```

---

## Building

### Requirements
- [devkitPro](https://devkitpro.org/) with devkitPPC
- [libogc2](https://github.com/extremscorner/libogc2) (see its README to add the package repository)

### Compile
```bash
make                 # Wii build      -> bombchu-wii.dol
make PLATFORM=gc     # GameCube build -> bombchu-gc.dol
make OGC=$DEVKITPRO/libogc   # build against stock libogc instead of libogc2
```

Every push and pull request is also built by GitHub Actions for both platforms. The `.dol` files are attached to each run as artifacts, and the workflow can be started by hand from the Actions tab.

New to devkitPPC or libogc2? [docs/toolchain-guide.md](docs/toolchain-guide.md) explains the toolchain, the libraries and headers the code uses, and how to add a new benchmark.

---

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on submitting patches, reporting issues, and participating in development.

## License

This software is licensed under the GNU General Public License v3.0 or later. See [LICENSE](LICENSE).
