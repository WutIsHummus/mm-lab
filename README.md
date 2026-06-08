# Memory Allocator (CS 429 MM Lab)

A custom dynamic memory allocator in C, implementing `umalloc` and `ufree` as drop-in replacements for `malloc` and `free`.

## Project Overview

**Spring 2026**

- Built a custom dynamic memory allocator in C (`umalloc`/`ufree`) with 16-byte alignment, metadata-managed blocks, and heap extension logic.
- Implemented segregated free lists, block splitting, and adjacent-block coalescing to improve allocation efficiency and fragmentation behavior.
- Validated allocator correctness/performance using provided trace runners and benchmarking harnesses.

## Design & Implementation

### Block Management
- **Block header** encodes size in bits [63:4] and allocation status in bit 0, with 16-byte payload alignment throughout.
- **Segregated free lists**: 6 size-class bins routing blocks based on configurable size limits.
- **Block splitting**: Splits oversized free blocks to reduce internal fragmentation and wastage.
- **Coalescing**: Merges physically adjacent free blocks on-demand to combat external fragmentation.
- **Heap extension**: Calls `csbrk` to grow heap in page-aligned increments when needed.

## Building & Testing

```bash
make              # Standard build with coalesce instrumentation
make debug        # Debug build (-O0, no instrumentation)
make deploy       # Optimized build (-O2)
```

### Test Harnesses

- `runner` – Runs allocation traces to validate correctness
- `heap_runner` – Extended heap consistency checking
- `performance <trace>` – Benchmarks allocator against allocation traces
- `bump_test` – Baseline bump allocator comparison

## Project Structure

| File | Purpose |
|------|---------|
| `umalloc.c/h` | Core allocator: `umalloc`, `ufree`, `uinit`, `coalesce`, `split`, `extend`, `find` |
| `check_heap.c/h` | Heap validity checker |
| `runner.c` | Trace-driven test harness |
| `heap_runner.c` | Extended heap validation |
| `performance.c` | Timing benchmark suite |
| `support.c/h` | Trace file I/O utilities |
| `csbrk.h` | Custom `sbrk` wrapper |
| `traces/` | Allocation trace test files |

## Key Takeaways

- Hands-on memory management implementation covering allocation, deallocation, coalescing, and fragmentation strategies.
- Applied data structures (segregated free lists) to optimize allocator performance.
- Validated design against comprehensive test harnesses and benchmarks.
