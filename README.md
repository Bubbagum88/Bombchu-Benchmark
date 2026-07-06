# Bombchu-Benchmark

**Bombchu Benchmark** is a system-level performance profiling tool for the Nintendo GameCube and Wii homebrew ecosystem.

It helps developers, homebrew authors, and hardware enthusiasts measure and compare real-world performance across storage devices, network stacks, CPU workloads, and filesystems under libogc2/devkitPro.

![Bombchu Logo (placeholder)](https://via.placeholder.com/800x200?text=Bombchu+Benchmark) <!-- Replace with actual badge/image later -->

---

## Why Bombchu-Benchmark?

GameCube and Wii homebrew often run on highly variable hardware:
- **Storage**: SD cards (various speeds/classes), USB 2.0 drives, IDE/SATA adapters, DVD, network shares.
- **CPU**: 486 MHz Gekko with varying cache behavior, overclocks (on some mods), and compiler differences.
- **Network**: Limited but relevant for multiplayer or streaming homebrew.
- **Filesystem**: FAT, ext2/3, custom loaders.

Bombchu provides standardized, reproducible benchmarks so you can answer questions like:
- "Does this faster SD card actually improve load times in my app?"
- "How does my new compression routine perform vs. stock?"
- "What's the real throughput of my USB Gecko or BBA setup?"

---

## Features

- **Storage Benchmarking**: Sequential & random read/write tests for SD, USB, etc.
- **Network Benchmarking**: Throughput, latency, and packet tests (where applicable).
- **CPU Benchmarking**: Integer, floating-point, cache, and synthetic workloads tailored to PPC.
- **Filesystem Analysis**: Metadata ops, directory traversal, fragmentation insights.
- **Extensible**: Easy to add custom test suites or export results.
- **Output Formats**: Human-readable tables, CSV/JSON for scripting or comparison.

---

## Benchmark Types

### Storage
Measures raw throughput and I/O patterns relevant to homebrew (e.g., loading assets, saving states).

**Tests include**:
- Sequential Read/Write (large blocks, game-like data).
- Random Read/Write (small files, save data).
- Multi-threaded or interleaved I/O where supported.

**Example results**:
| Device       | Sequential Read | Sequential Write | Random 4K Read | Notes                  |
|--------------|-----------------|------------------|----------------|------------------------|
| SD Card (Class 10) | 18.5 MB/s     | 12.3 MB/s       | 4.2 MB/s      | SanDisk Ultra         |
| USB 2.0 HDD  | 25.1 MB/s      | 18.7 MB/s       | 6.8 MB/s      | With Swiss loader     |

### Network
Tests TCP/UDP performance, useful for netplay, remote loading, or WiiConnect24-style features.

**Tests include**:
- Bandwidth (upload/download).
- Latency and jitter.
- Small-packet performance.

### CPU
Focuses on GameCube/Wii realities: branch prediction, cache effects, FPU, AltiVec/VMX if applicable.

**Tests include**:
- Dhrystone / Whetstone variants.
- Custom loops (e.g., texture decompression, audio mixing).
- Memory bandwidth (cache-to-RAM).

### Filesystem Analysis
- Directory listing speed.
- File creation/deletion throughput.
- Fragmentation impact.

---

## Example Output

```text
Bombchu Benchmark v0.1.0
Running on GameCube (PAL) / Wii

=== Storage Benchmarks ===
SD Read (Sequential)     : 18.4 MB/s
SD Write (Sequential)    : 11.2 MB/s
...

=== CPU Benchmarks ===
Integer Score            : 245.3
Float Score              : 187.6
...
---

## Build Instructions
make

---

## Requirements
- devkitPro
- libogc2

---

## License
MIT
