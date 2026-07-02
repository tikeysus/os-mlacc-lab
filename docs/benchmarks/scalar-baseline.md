# Scalar GEMM Baseline Timings

Scalar (no-SIMD, no-accelerator) C reference implementation measured on the
development CPU. These numbers are the comparison baseline for the Phase 4
accelerator speedup analysis (issue #13).

## Hardware & Software

| Field | Value |
|-------|-------|
| CPU | Apple M3 |
| ISA | arm64 |
| Cores | 8 |
| OS | macOS 15.2 (Sequoia) |
| Compiler | Apple clang 15.0.0 |
| CFLAGS | `-std=c11 -Wall -Wextra -O2` |
| Timing method | `clock()` / `CLOCKS_PER_SEC` (CPU time, 1 µs resolution) |

## gemm_f32 — float32, C = A × B (row-major)

Square (N×N×N) sweep; 2N³ FLOPs counted.

| N | elapsed (µs) | GFLOP/s |
|---|-------------|---------|
| 8 | < 1 tick | n/a |
| 16 | 2 | 4.10 |
| 32 | 10 | 6.55 |
| 64 | 107 | 4.90 |
| 128 | 1143 | 3.67 |

## gemm_i8 — int8 inputs → int32 accumulator (row-major)

Same sweep; same FLOP count formula.

| N | elapsed (µs) | GFLOP/s |
|---|-------------|---------|
| 8 | < 1 tick | n/a |
| 16 | 2 | 4.10 |
| 32 | 9 | 7.28 |
| 64 | 70 | 7.49 |
| 128 | 617 | 6.80 |

## Notes

- Results at N=8 complete in under one `clock()` tick (< 1 µs) and cannot
  be timed with this method; a higher-resolution timer is needed for small
  matrices.
- Timings reflect a single run; variance at small N is ±1 tick (±1 µs).
- These numbers will be the denominator in the Phase 4 speedup ratio:
  `speedup = scalar_elapsed / accelerator_elapsed`.
