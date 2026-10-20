# The tools under Bombchu Benchmark

A beginner's map of everything your code sits on: the compiler, the libraries,
the headers you `#include`, the Makefile, the CI build, and how the repo's own
files fit together. Every example uses calls that exist in the current
libogc2 / libdvm headers (checked against their source on 2026-10-20).

Contents:

1. [The big picture](#1-the-big-picture)
2. [devkitPro and devkitPPC: the compiler](#2-devkitpro-and-devkitppc-the-compiler)
3. [libogc2: the console's "standard library"](#3-libogc2-the-consoles-standard-library)
4. [The Makefile, line by line](#4-the-makefile-line-by-line)
5. [The GitHub Actions build](#5-the-github-actions-build)
6. [Every header the repo includes, and what it gives you](#6-every-header-the-repo-includes-and-what-it-gives-you)
7. [How the repo's own code fits together](#7-how-the-repos-own-code-fits-together)
8. [Adding a new benchmark, step by step](#8-adding-a-new-benchmark-step-by-step)
9. [Toolkit for each planned benchmark](#9-toolkit-for-each-planned-benchmark)
10. [Gotchas worth knowing](#10-gotchas-worth-knowing)
11. [Where to read more](#11-where-to-read-more)

---

## 1. The big picture

```
 Your code (src/, benchmarks/, ui/)
        │  #include <gccore.h>, <fat.h>, <stdio.h>, <network.h> ...
        ▼
 ┌──────────────────────────────────────────────────────────────┐
 │ libogc2 (-logc)      video, console, controllers, timers,    │
 │                      threads, cache, Wii IOS, networking     │
 │ libdvm  (-lfat)      FAT/exFAT on SD/USB → makes fopen work  │
 │ newlib  (libc, -lm)  stdio.h, stdlib.h, string.h, math.h     │
 └──────────────────────────────────────────────────────────────┘
        │  compiled by
        ▼
 devkitPPC (powerpc-eabi-gcc)  →  .elf  →  elf2dol  →  bombchu-wii.dol / bombchu-gc.dol
        │  run by
        ▼
 GitHub Actions (devkitpro/devkitppc Docker image), one job per platform
```

There is no operating system underneath you. On a PC, `fopen` asks Windows or
Linux to open a file. On a GameCube or Wii, your `.dol` *is* the whole program
that runs: libogc2 is the code that talks to the hardware, and newlib (the C
library) calls into libogc2 and libdvm when you use `printf` or `fopen`. That
is why a benchmark here really measures the hardware plus these libraries, and
nothing else.

## 2. devkitPro and devkitPPC: the compiler

**devkitPro** is the organisation and package system (`dkp-pacman`) that ships
homebrew toolchains. **devkitPPC** is their toolchain for PowerPC, the CPU
family both consoles use (the GameCube's "Gekko" and Wii's "Broadway" are
PowerPC 750 derivatives). It contains:

| Tool | What it does |
|---|---|
| `powerpc-eabi-gcc` | The C compiler. The Makefile calls it as `$(CC)`. |
| `elf2dol` | Converts the linked `.elf` into a `.dol`, the executable format the consoles and the Homebrew Channel load. |
| `base_tools` | A small Makefile fragment that sets `CC`, `AS`, `AR` etc. to the PowerPC versions. Our Makefile pulls it in with `include $(DEVKITPPC)/base_tools`. |
| newlib | The C standard library (`stdio.h`, `stdlib.h`, `string.h`, `math.h`). |

Two environment variables tell the Makefile where everything is installed:
`DEVKITPRO` (usually `/opt/devkitpro`) and `DEVKITPPC` (`/opt/devkitpro/devkitPPC`).
The CI container sets both, which is why the Makefile stops with an error if
either is missing.

**The machine flags.** The Makefile picks one of these per platform:

```make
# Wii
MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
# GameCube
MACHDEP := -DGEKKO -mogc -mcpu=750 -meabi -mhard-float
```

- `-mrvl` / `-mogc` say "this is a Wii (Revolution) / GameCube build". This
  defines `HW_RVL` or `HW_DOL` in your code, which libogc2 uses to switch on
  platform-only features. You can use them too:
  ```c
  #ifdef HW_RVL
      // Wii-only code (USB storage, Wi-Fi, IOS calls)
  #else
      // GameCube-only code (Broadband Adapter, SD2SP2)
  #endif
  ```
- `-mcpu=750` tunes code for the PowerPC 750.
- `-mhard-float` uses the CPU's real floating-point unit (important for a CPU
  benchmark: float math is done in hardware, not emulated).
- `-DGEKKO` defines `GEKKO`, a flag some libraries check for.

## 3. libogc2: the console's "standard library"

**libogc** is the original devkitPro library for GameCube/Wii hardware.
**libogc2** is a fork maintained by Extrems (extremscorner.org) with more
hardware support (SD2SP2, GC Loader, newer SD adapters) and fixes. You chose
libogc2 in PR #2. The API is the same family of functions (`VIDEO_*`, `PAD_*`,
`CON_*`, `LWP_*`, `net_*`), so most libogc tutorials still apply.

libogc2 installs per platform:

```
$DEVKITPRO/libogc2/wii/include        $DEVKITPRO/libogc2/wii/lib
$DEVKITPRO/libogc2/gamecube/include   $DEVKITPRO/libogc2/gamecube/lib
```

The libraries inside `lib/` that matter to us:

| Library | Link flag | Platform | Gives you |
|---|---|---|---|
| libogc | `-logc` | both | video, console, GC controllers, timers, threads, cache, system, Wii networking |
| libbba | `-lbba` | GameCube | networking through the Broadband Adapter (lwIP stack) |
| libwiiuse + libbte | `-lwiiuse -lbte` | Wii | Wii Remotes (`WPAD_*`) over Bluetooth |
| libfat (from libdvm) | `-lfat` | both | mounting SD/USB so `fopen("sd:/...")` works |
| libdi | `-ldi` | Wii | reading the DVD drive |

**libdvm** (the `libogc2-libdvm` package CI installs) is the disk and volume
manager. It provides `fat.h` and a `libfat.a` that is compatible with the old
libfat API, so `fatInitDefault()` still works, but underneath it supports
FAT12/16/32, exFAT and ext2/3/4.

## 4. The Makefile, line by line

```make
include $(DEVKITPPC)/base_tools        # sets CC=powerpc-eabi-gcc etc.

PLATFORM ?= wii                        # `make` = Wii, `make PLATFORM=gc` = GameCube
OGC ?= $(DEVKITPRO)/libogc2            # which libogc to build against
```

`?=` means "use this unless the person running make set it". That's how CI
runs `make PLATFORM=gc` with the same Makefile.

```make
ifneq ($(wildcard $(OGC)/$(OGC_PLATFORM)/lib),)
OGC_INC := $(OGC)/$(OGC_PLATFORM)/include   # libogc2 layout
OGC_LIB := $(OGC)/$(OGC_PLATFORM)/lib
else
OGC_INC := $(OGC)/include                   # stock libogc layout
OGC_LIB := $(OGC)/lib/$(OGC_LIBDIR)
endif
```

This detects whether `OGC` points at libogc2 or the original libogc, so
`make OGC=$(DEVKITPRO)/libogc` still works as a fallback.

```make
SOURCES  := src benchmarks ui ui/themes     # every .c in these folders is compiled
INCLUDES := include ui ui/themes            # these folders are searched for "quoted.h" headers
```

**This is the part you'll touch most.** Any new `.c` file you drop into one of
the `SOURCES` folders is picked up automatically. If you make a new folder
(say `benchmarks/net/`), add it to `SOURCES`, and add a header folder to
`INCLUDES` if it has one.

```make
CFLAGS  := -Wall -O2 -g $(MACHDEP)
CFLAGS  += $(foreach dir,$(INCLUDES),-I$(dir)) -I$(OGC_INC)
LIBS    := -L$(OGC_LIB) -lfat -logc -lm
```

- `-Wall` turns on warnings, `-O2` optimises, `-g` keeps debug symbols in the
  `.elf` (the `.dol` doesn't carry them).
- `-O2` matters for benchmarks: the compiler will delete work whose result is
  never used. See [Gotchas](#10-gotchas-worth-knowing).
- `LIBS` order matters to the linker: libraries that *use* others come first.
  `-lfat` uses libogc, so it comes before `-logc`. When you add networking on
  GameCube, `-lbba` goes before `-logc` too.

```make
$(TARGET).dol: $(BUILD)/$(TARGET).elf
	elf2dol $< $@                     # .elf → .dol

$(BUILD)/$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBS) -o $@   # link all .o files with the libraries

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@       # compile one .c into one .o
```

Build pipeline: each `.c` → `build/<platform>/…/*.o` → linked into
`build/<platform>/bombchu-<platform>.elf` → `bombchu-<platform>.dol`. The
`-MMD -MP` flags write `.d` files so editing a header rebuilds the files that
include it. `-Wl,-Map,…map` writes a map file listing every function's address
and size, handy if you ever want to know what's eating space.

## 5. The GitHub Actions build

`.github/workflows/build.yml` runs on every push to `main`, every PR into
`main`, and on demand ("Run workflow" button):

1. **Container** `devkitpro/devkitppc:latest`: a Linux image with devkitPPC
   and stock libogc already installed, and `DEVKITPRO`/`DEVKITPPC` set.
2. **Matrix** `platform: [wii, gc]`: the job runs twice in parallel, once per
   console. `fail-fast: false` means one failing doesn't cancel the other.
3. **Install libogc2**: trusts Extrems' package-signing key, adds the libogc2
   package repo to pacman's config *above* devkitPro's own (so libogc2 wins),
   then installs `libogc2` and `libogc2-libdvm`. These are rolling packages, so
   each run gets the latest version. (PR #4, if merged, trims this step so
   only the libogc2 repo is synced.)
4. **Build**: `make PLATFORM=wii` or `make PLATFORM=gc`.
5. **Upload**: the `.dol` becomes a downloadable artifact named
   `bombchu-wii` / `bombchu-gc` on the run's summary page.

Your loop without local tools: edit → push to a branch / open a PR → wait for
the green check → download the artifact → copy the `.dol` to your SD card as
`apps/bombchu/boot.dol` (or load it in Dolphin). Compiler errors show up in the
**Build project** step's log, with file and line numbers like a local build.

## 6. Every header the repo includes, and what it gives you

### `<gccore.h>` (libogc2) — the main one

An umbrella header: including it pulls in `ogc/video.h`, `ogc/pad.h`,
`ogc/consol.h`, `ogc/system.h`, `ogc/cache.h`, `ogc/lwp.h`, `ogc/gx.h` and more
(plus Wii-only headers like `ogc/ios.h` and `ogc/usbstorage.h` on Wii builds).
Used in `src/main.c`, all of `ui/`, and `ui/themes/theme.h`.

**Video** (`ogc/video.h`, `ogc/system.h`). `src/main.c:44-56` is the standard
startup recipe:

```c
VIDEO_Init();                                    // power up the video interface
rmode = VIDEO_GetPreferredMode(NULL);            // pick NTSC/PAL/progressive from console settings
xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode)); // allocate the screen memory, uncached address
VIDEO_Configure(rmode);
VIDEO_SetNextFramebuffer(xfb);                   // show this buffer
VIDEO_SetBlack(FALSE);                           // un-blank the screen
VIDEO_Flush();                                   // apply the settings
VIDEO_WaitVSync();                               // wait one frame (1/60 or 1/50 s)
```

The framebuffer (`xfb`) is just a block of RAM the video hardware reads 60
times a second. `MEM_K0_TO_K1` converts a "cached" address to an "uncached"
one so writes go straight to RAM where the video chip can see them.

`VIDEO_WaitVSync()` sleeps until the next screen refresh. Every menu loop in
the repo calls it once per iteration, so menus run at 60 loops per second
instead of spinning the CPU flat out.

**Console** (`ogc/consol.h`). `CON_Init(xfb, 20, 20, width, height, stride)`
turns the framebuffer into a text terminal, so `printf` draws text on screen.
It understands ANSI escape codes, which is how `ui/ui_engine.c` positions and
colours text:

| Escape | Meaning | Used in |
|---|---|---|
| `\x1b[2J` | clear screen | `ui_clear_screen` |
| `\x1b[row;colH` | move cursor | `ui_draw_text` |
| `\x1b[40m` / `\x1b[37m` | black background / white text | `theme_default.c` |
| `\x1b[32m` / `\x1b[33m` / `\x1b[36m` | green / yellow / cyan text | themes |

`CON_GetMetrics(&cols, &rows)` tells you how many text columns and rows fit,
useful when results tables get bigger.

**Controllers** (`ogc/pad.h`). GameCube controllers, which also work on a Wii
through its GC ports:

```c
PAD_Init();                    // once at startup
// every frame:
PAD_ScanPads();                // read all four ports
u32 down = PAD_ButtonsDown(0); // buttons pressed *this frame* on port 1 (0 = port 1)
u32 held = PAD_ButtonsHeld(0); // buttons currently held down
if (down & PAD_BUTTON_A) { ... }
```

`ButtonsDown` fires once per press (good for menus); `ButtonsHeld` stays set
while held (good for "hold B to cancel a long run"). Constants:
`PAD_BUTTON_A/B/X/Y/START/UP/DOWN/LEFT/RIGHT/L/R/Z`. Analog sticks:
`PAD_StickX(0)`, `PAD_StickY(0)`.

Wii Remotes are a different library (`wiiuse/wpad.h`, `WPAD_Init()`,
`WPAD_ScanPads()`, `WPAD_ButtonsDown(0)`, linked with `-lwiiuse -lbte`). The
app doesn't use them yet, so on a Wii you currently need a GameCube controller.

**Colours** (`ogc/gx_struct.h`). `GXColor` (`{r, g, b, a}`) in
`ui/themes/theme.h` comes from here. It's the colour type for GX, the 3D
graphics chip, which the UI doesn't initialise yet (the themes use console
colours instead).

**Cache** (`ogc/cache.h`). `DCFlushRange(ptr, len)` writes cached data out to
RAM; `DCInvalidateRange(ptr, len)` throws cached copies away so the next read
comes from RAM. Not used yet, but useful for honest memory benchmarks (§9).

### `<ogcsys.h>` (libogc2)

Older umbrella header for system-level bits. `src/main.c` includes it; with
`<gccore.h>` already there it adds little, and is harmless.

### `<ogc/lwp_watchdog.h>` (libogc2) — the timer

Used by `benchmarks/storage_benchmark.c`. It includes `ogc/timesupp.h`, which
is where the timing tools live:

| Call | What it does |
|---|---|
| `u64 gettime(void)` | Read the CPU's time base: a counter that ticks at a fixed rate (bus clock ÷ 4: 60.75 MHz on Wii, 40.5 MHz on GameCube). |
| `diff_ticks(t0, t1)` | Ticks between two readings (handles wraparound). |
| `ticks_to_millisecs(t)` | Ticks → whole milliseconds (**integer**, rounds down). |
| `ticks_to_microsecs(t)` | Ticks → whole microseconds. |
| `ticks_to_nanosecs(t)` | Ticks → nanoseconds. |
| `diff_usec(t0, t1)` | Microseconds between two readings (`u32`). |
| `ticks_to_cycles(t)` | Ticks → CPU clock cycles. |

This is the right clock for benchmarking: very fine-grained (about 16 ns per
tick on Wii) and not affected by anything else. Pattern:

```c
u64 start = gettime();
do_work();
u64 end = gettime();
float ms = ticks_to_microsecs(diff_ticks(start, end)) / 1000.0f; // keeps fractions of a ms
```

### `<fat.h>` (libdvm) — mounting storage

Used by `benchmarks/storage_benchmark.c`. `fatInitDefault()` probes for
storage and mounts whatever it finds as named drives. After that, normal C
`fopen`/`fread`/`fwrite`/`remove` work with paths like `"sd:/file.bin"`.

What gets mounted (from libdvm's `dvm_libogc2.c`):

| Name | Wii | GameCube |
|---|---|---|
| `sd:` | front SD slot | SD2SP2 in Serial Port 2 |
| `usb:` | first USB drive | — |
| `carda:` / `cardb:` | SD Gecko-style adapter in memory card slot A / B | same |
| `dvd:` | — | GC Loader |
| `ram:` | — | Memory Expansion Pak |

`fatInitDefault()` returns `true` if anything mounted. Other useful calls:
`fatUnmount("sd")`, `fatGetVolumeLabel("sd", buf)`. For a "which drive?"
picker, `fatInitDefault()` followed by trying `opendir("usb:/")`,
`opendir("carda:/")` etc. tells you which exist.

### `<stdio.h>`, `<stdlib.h>`, `<string.h>` (newlib)

The normal C library. `printf`/`snprintf` print to the console;
`fopen`/`fread`/`fwrite`/`fclose`/`remove` go through libdvm to the card;
`malloc`/`free` allocate from the console's RAM (24 MB "MEM1" on GameCube,
plus 64 MB "MEM2" on Wii); `memset`/`memcpy` are here too. `exit(0)` in
`main.c` returns to the loader (Homebrew Channel or Swiss). You can also use
`<dirent.h>` (`opendir`, `readdir`), `<sys/stat.h>` (`stat`, `mkdir`) and
`<unistd.h>` (`unlink`, `usleep`) for filesystem work.

### Headers you'll add later

| Header | Library | For |
|---|---|---|
| `<network.h>` | libogc2 (+ `-lbba` on GC) | sockets: `net_init`, `if_config`, `net_socket`, `net_connect`, `net_send`, `net_recv` |
| `<dirent.h>`, `<sys/stat.h>`, `<unistd.h>` | newlib + libdvm | filesystem benchmark |
| `<wiiuse/wpad.h>` | libwiiuse (`-lwiiuse -lbte`) | Wii Remote input |
| `<math.h>` | libm (`-lm`, already linked) | `sqrt`, `sin` etc. for CPU float tests |

## 7. How the repo's own code fits together

```
src/main.c                 sets up video/console/pad, runs the main menu loop
  └─ ui/ui_benchmark_selector.c   lists benchmarks, A runs the chosen one
        ├─ src/benchmark_dispatcher.c   run_benchmark(): setup → run → teardown
        │     └─ benchmarks/storage_benchmark.c   the actual test
        ├─ ui/ui_progress_callback.c    called during a run (empty on purpose)
        └─ ui/ui_engine.c               draw text/menus, ui_show_results_screen()
  └─ ui/ui_theme_selector.c       picks theme_default or theme_retro (ui/themes/)
```

**The benchmark contract** lives in `include/benchmark.h`:

```c
typedef struct {
    const char* name;
    float duration_ms;
    float throughput_mb_s;
    int success;
} BenchmarkResult;               // what a run produces

typedef struct {
    const char* name;
    void (*setup)(void);         // prepare (mount, allocate, connect)
    void (*run)(BenchmarkResult* result, void (*progress)(float)); // timed work
    void (*teardown)(void);      // clean up (delete files, close sockets)
} Benchmark;                     // what a benchmark is
```

`Benchmark` is a struct of **function pointers**: each benchmark fills in
its own three functions, and the dispatcher calls them without knowing what
they do. `benchmarks/storage_benchmark.c` ends with exactly that:

```c
Benchmark STORAGE_BENCHMARK = {
    .name = "Storage Benchmark",
    .setup = storage_setup,
    .run = storage_run,
    .teardown = storage_teardown
};
```

and `include/storage_benchmark.h` makes it visible to other files with
`extern Benchmark STORAGE_BENCHMARK;`.

`run_benchmark()` (`src/benchmark_dispatcher.c`) zeroes a shared result,
calls setup/run/teardown, and `get_last_result()` hands that result to
`ui_show_results_screen()`.

**The menus** are all the same shape: a `while (1)` loop that redraws only
when something changed, waits a frame, reads the pad, and moves a selection
index. `main.c` adds a small state machine (`STATE_MENU`,
`STATE_BENCHMARK_SELECTOR`, …) on top.

**The progress callback** is passed into `run()` so a benchmark can report
0.0 → 1.0. `ui_progress_callback` is left empty on purpose: it's called inside
the timed loop, so drawing there would add screen-drawing time to the result.

## 8. Adding a new benchmark, step by step

Example: a CPU benchmark.

**1. Header** `include/cpu_benchmark.h`:

```c
#ifndef CPU_BENCHMARK_H
#define CPU_BENCHMARK_H

#include "benchmark.h"

extern Benchmark CPU_BENCHMARK;

#endif
```

**2. Source** `benchmarks/cpu_benchmark.c` (already in `SOURCES`, so the
Makefile picks it up):

```c
#include "cpu_benchmark.h"
#include <gccore.h>
#include <ogc/lwp_watchdog.h>

#define ITERATIONS 10000000

static void cpu_setup(void) {}
static void cpu_teardown(void) {}

static void cpu_run(BenchmarkResult* result, void (*progress)(float)) {
    volatile u32 acc = 0;        // volatile: stops -O2 deleting the loop

    u64 start = gettime();
    for (u32 i = 0; i < ITERATIONS; i++)
        acc += i * 3 + (acc >> 1);
    u64 end = gettime();

    progress(1.0f);

    result->duration_ms = ticks_to_microsecs(diff_ticks(start, end)) / 1000.0f;
    result->throughput_mb_s = 0;   // not meaningful here; see note below
    result->success = 1;
}

Benchmark CPU_BENCHMARK = {
    .name = "CPU Integer",
    .setup = cpu_setup,
    .run = cpu_run,
    .teardown = cpu_teardown
};
```

**3. Register it** in `ui/ui_benchmark_selector.c`:

```c
#include "cpu_benchmark.h"
...
Benchmark* benches[] = {
    &STORAGE_BENCHMARK,
    &CPU_BENCHMARK
};
```

**4. Push** and let CI build it.

Note: `BenchmarkResult` only has "duration" and "MB/s". CPU and network
tests will want other numbers (operations per second, latency, separate
read/write speeds), so expect to extend that struct and
`ui_show_results_screen()` when you add them.

## 9. Toolkit for each planned benchmark

### CPU

Tools: `gettime()`, `ticks_to_*`, `volatile`, `<math.h>`, and optionally
`ticks_to_cycles()` to report "cycles per operation".

```c
// Floating point (uses the hardware FPU thanks to -mhard-float)
volatile float f = 1.0001f;
u64 t0 = gettime();
for (int i = 0; i < 5000000; i++) f = f * 1.0000001f + 0.5f;
u64 t1 = gettime();

// Memory bandwidth: copy a big buffer
u8 *src = memalign(32, 4 << 20), *dst = memalign(32, 4 << 20); // <malloc.h>, 32-byte aligned
t0 = gettime();
memcpy(dst, src, 4 << 20);
t1 = gettime();
float mb_s = 4.0f / (ticks_to_microsecs(diff_ticks(t0, t1)) / 1e6f);
```

Ideas: integer ops, float ops, `memcpy` bandwidth, MEM1 vs MEM2 speed on Wii,
and cached vs uncached reads (call `DCFlushRange` / `DCInvalidateRange` on
the buffer first to start from a cold cache).

### Storage (exists today)

Tools: `fatInitDefault()`, `fopen`/`fwrite`/`fread`/`fclose`, `gettime()`.
`benchmarks/storage_benchmark.c` writes 128 × 64 KB blocks (8 MB) to
`sd:/bombchu_storage_test.bin`, reads them back, and times each half.

Improvements to make it a real benchmark:

- Report write and read speed separately (each moved 8 MB in its own time).
  Right now `total_mb / (write_ms + read_ms)` counts the 8 MB once but the
  time twice, so the number shown is half the average speed.
- Check the return value of `fatInitDefault()` and of each `fwrite`/`fread`
  (`fwrite` returning less than `BLOCK_SIZE` means the card is full or
  failed).
- Let the user pick the drive (`sd:`, `usb:`, `carda:` …) and block size
  (4 KB vs 64 KB vs 1 MB shows very different numbers).
- Use `ticks_to_microsecs(...) / 1000.0f` for fractional milliseconds.

### Filesystem

Tests how fast the filesystem handles *many small operations* rather than
raw bytes. Tools from newlib + libdvm:

```c
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

mkdir("sd:/bombchu_fs", 0777);

u64 t0 = gettime();
for (int i = 0; i < 200; i++) {
    char path[64];
    snprintf(path, sizeof(path), "sd:/bombchu_fs/f%03d.txt", i);
    FILE *f = fopen(path, "wb");
    fputs("x", f);
    fclose(f);
}
u64 t1 = gettime();   // → "files created per second"

DIR *d = opendir("sd:/bombchu_fs");
struct dirent *e;
int n = 0;
while ((e = readdir(d)) != NULL) n++;   // → directory listing speed
closedir(d);

struct stat st;
stat("sd:/bombchu_fs/f000.txt", &st);  // → metadata lookup speed

// teardown: unlink() each file, then rmdir("sd:/bombchu_fs")
```

Report operations per second for create, list, stat and delete.

### Network

Tools: `<network.h>` from libogc2. On Wii it uses the built-in Wi-Fi/LAN
adapter through IOS (already in `-logc`). On GameCube it needs the Broadband
Adapter and **`-lbba` added to `LIBS` before `-logc`** for the GC build:

```make
ifeq ($(PLATFORM),gc)
LIBS := -L$(OGC_LIB) -lfat -lbba -logc -lm
endif
```

Bringing the network up and measuring TCP upload speed to a PC on your LAN:

```c
#include <network.h>
#include <errno.h>
#include <string.h>

char ip[16] = {0}, mask[16] = {0}, gw[16] = {0};
if (if_config(ip, mask, gw, true) < 0) {   // true = use DHCP; brings up the stack
    result->success = 0;
    return;
}
printf("Console IP: %s\n", ip);

s32 sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
struct sockaddr_in server;
memset(&server, 0, sizeof(server));
server.sin_family = AF_INET;
server.sin_port = htons(5000);
inet_aton("192.168.1.50", &server.sin_addr);   // your PC's IP
if (net_connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) { /* fail */ }

static u8 buf[32 * 1024];
u64 t0 = gettime();
for (int i = 0; i < 256; i++)                  // 8 MB
    net_send(sock, buf, sizeof(buf), 0);
u64 t1 = gettime();
net_close(sock);
```

On the PC, any TCP listener that discards data works as the other end (for
example `nc -l 5000 > /dev/null`, or a small Python script). Other useful
calls: `net_recv` (download speed), `net_gethostip()`, `net_gethostbyname()`
(DNS timing), and round-trip latency by sending a tiny message and timing the
echo. libogc2 functions return negative error codes (like `-EAGAIN`) instead
of setting `errno`.

## 10. Gotchas worth knowing

- **The optimiser deletes unused work.** With `-O2`, a loop whose result is
  never used can be removed entirely, and your benchmark reports 0 ms. Store
  results in a `volatile` variable or print them.
- **Don't draw inside the timed section.** `printf` to the console is slow
  (it renders pixels in software). Draw before `gettime()` and after.
- **`ticks_to_millisecs` is whole numbers.** Short tests round to 0; use
  `ticks_to_microsecs` and divide by 1000.0f.
- **Caches skew results.** The CPU has caches, and libdvm keeps a small sector
  cache. Reading a file you just wrote, or a buffer you just filled, can come
  from cache rather than the card or RAM. Make test sizes much larger than the
  cache, or flush/invalidate first.
- **Run several times.** Report average and best of 3–5 runs; the first run
  is often slower (card wake-up, cold caches).
- **Check every return value.** No OS will tell you something failed; a
  `NULL` `FILE*` or a short `fwrite` is your only signal.
- **Wii vs GameCube differ.** Different clock speeds (Wii's CPU is 729 MHz,
  GC's 486 MHz), RAM, storage and network hardware. Wrap platform-only code
  in `#ifdef HW_RVL` and keep results labelled by platform.
- **libogc2 is unpinned in CI.** Each run installs the newest package. If a
  build suddenly breaks with no code change, a libogc2 update is the first
  suspect.
- **Real hardware vs Dolphin.** Dolphin is fine for testing menus, but its
  timings don't reflect real hardware. Benchmark numbers only mean something
  on a console.

## 11. Where to read more

- libogc2 headers (the real documentation; most functions have comments):
  https://github.com/extremscorner/libogc2/tree/master/include — start with
  `ogc/video.h`, `ogc/pad.h`, `ogc/timesupp.h`, `ogc/cache.h`, `network.h`.
- libdvm (`fat.h`, `dvm.h`, and which drives get mounted in
  `source/dvm_libogc2.c`): https://github.com/extremscorner/libdvm
- devkitPro's GameCube/Wii examples (written for libogc, nearly all apply):
  https://github.com/devkitPro/gamecube-examples and
  https://github.com/devkitPro/wii-examples
- WiiBrew (hardware details, homebrew format, Homebrew Channel `meta.xml`):
  https://wiibrew.org
