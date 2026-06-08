# Memory Allocator (MM Lab)

A custom dynamic memory allocator implemented in C as part of CS 429 (Computer Organization and Architecture) at UT Austin.

## Overview

Implements `umalloc` and `ufree` — drop-in replacements for `malloc` and `free` — using an explicit free list with segregated bins, block splitting, and coalescing.

## Design

- **Block structure**: Each block carries a header (`mem_block_header_t`) encoding size in bits [63:4] and allocation status in bit 0, with 16-byte payload alignment enforced throughout.
- **Segregated free lists**: 6 size-class bins (`BIN_COUNT = 6`) with configurable limits (64, 128, 256, 512, 1024, ∞ bytes). `select_bin` routes blocks to the correct bin; `find` searches from the best-fit bin upward.
- **Splitting**: When a free block is larger than needed, `split` carves off the remainder and inserts it into the appropriate bin.
- **Coalescing**: `coalesce` collects all free blocks across all bins, sorts them by address with `qsort`, merges physically adjacent blocks, and redistributes into bins. Called lazily when `find` fails before falling back to `extend`.
- **Heap extension**: `extend` calls `csbrk` to grow the heap in page-aligned increments, inserting the new block into the free list.

## Building

```bash
make          # standard build
make debug    # debug build (-O0, no coalesce instrumentation)
make deploy   # optimized build (-O2)
```

## Running

```bash
./runner       # run allocation trace tests
./heap_runner  # heap correctness checker
./performance <tracefile>  # benchmark against a trace
./bump_test    # bump allocator baseline test
```

## Files

| File | Description |
|------|-------------|
| `umalloc.c` | Allocator implementation (`umalloc`, `ufree`, `uinit`, `coalesce`, `split`, `extend`, `find`) |
| `umalloc.h` | Block struct definition and function declarations |
| `check_heap.c/h` | Heap consistency checker |
| `runner.c` | Trace-driven correctness runner |
| `heap_runner.c` | Extended heap validation harness |
| `performance.c` | Timing benchmark harness |
| `support.c/h` | Trace file I/O utilities |
| `csbrk.h` | Custom `sbrk` wrapper for heap growth |
| `traces/` | Allocation trace files for testing |
