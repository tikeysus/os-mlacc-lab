# CLAUDE.md

## Project Overview

This is a hardware/software co-design lab: build a working, benchmarked GEMM (General
Matrix-Matrix Multiply) accelerator from scratch on a SoC FPGA. The goal is to close the
full loop from RTL micro-architecture through software driver to measured speedup numbers
on real hardware.

The hardware platform is deferred until Phase 3. Everything through Phase 2 is platform-agnostic C and Verilog.

The project follows a 10–12 week roadmap (`docs/roadmap.md`). As of June 2026, the
hardware-software interface contract is being defined (Phase 0) and the scalar software
baseline is the active work area (Phase 1).

## Build and Test

```sh
make          # build scalar_dot_product benchmark binary
make run      # run it
make test     # build and run all Unity tests
make verify   # clean build with -Werror + tests + repo hygiene checks
```

`make verify` is the gate before any commit. It runs clang-format and clang-tidy when available.

## Current Scalar Kernel Baseline

**ML kernels** (`software/include/ml_kernels.h`):
- `dot_product_f32` — f32 dot product
- `dot_product_i8` — int8 quantized dot product, accumulates into int32
- `gemv_f32` — general matrix-vector multiply (row-major)
- `gemm_f32` — general matrix-matrix multiply C = A×B (row-major, f32) [Phase 1]
- `gemm_i8` — quantized matrix-matrix multiply, int8 inputs → int32 output [Phase 1]

`dot_product_f32` and `gemv_f32` are building blocks of `gemm_f32` and cross-check targets.
These are the correctness reference and the scalar performance baseline for the accelerator
comparison in Phase 4.

## Code Conventions

- Unity test naming: `test_<function>_<condition>` — match existing test files exactly

## Issue Priority Ordering

See GitHub issues ordered by dependency and phase: #1 (scope) → #10 (register map) → #11 (RTL) → #3 (board) → #12 (driver) → #18 (HW test) → #13 (benchmark) → #17 (MMIO) → #21 (stretch) → #19 (robustness) → #20 (report).

## What Comes Next (Hardware Path)

Phase 2 starts with `hardware/rtl/`: a single processing element (`pe.v`) implementing a
fused multiply-accumulate cell, then a systolic array (`systolic_array.v`), AXI-Lite CSR
block (`axi_lite_csr.v`), and top-level wrapper (`gemm_top.v`). Simulation uses Icarus
Verilog (no Vivado license required). Phase 3 selects the physical board and flashes the
design. Phase 4 ports the scalar GEMM benchmark to the driver API and measures speedup.
