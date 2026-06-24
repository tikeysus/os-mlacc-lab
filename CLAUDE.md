# CLAUDE.md

## Project Overview

This is a hardware/software co-design lab: build a working, benchmarked ML accelerator from scratch, modeled after the Arrow architecture, on an FPGA-hosted RISC-V SoC. The goal is to close the full loop from RTL micro-architecture through OS/runtime integration to measured speedup numbers on real hardware.

The project is organized around a 10–12 week roadmap (`docs/roadmap.md`). As of June 2026, Phase 0 (research) is partially done and Phase 1 (scalar software baseline) is ~50% complete. Hardware bring-up and everything from Phase 2 onward has not started.

## Repository Layout

```
hardware/
  fpga/     FPGA board project, constraints, bring-up notes
  rtl/      Accelerator RTL modules
  sim/      RTL testbenches and simulator scripts
software/
  benchmarks/scalar/   Scalar C kernel implementations (correctness + perf baseline)
  include/             Public headers: ml_kernels.h, image_kernels.h
  runtime/             Future: C API, MMIO helpers, scheduling policy
  tests/               Unity-based unit tests
docs/                  Roadmap, architecture notes, issue planning
scripts/               Repo hygiene and test runner helpers
tools/                 Setup and reproducibility helpers
build/                 Generated binaries (not committed)
third_party/unity/     Unity test framework
```

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

**Image kernels** (`software/include/image_kernels.h`):
- `threshold_u8` — element-wise threshold
- `convolve_u8` — general 2D convolution
- `convolve3x3_u8` — fixed 3×3 convolution
- `sobel3x3_u8` — Sobel edge detection with threshold

These are the correctness reference and the scalar performance baseline. When the accelerator is built, they get ported to the accelerator API and compared cycle-for-cycle.

## Code Conventions

- C only in `software/` and `hardware/`; `snake_case` for all functions and variables
- No function over 80 lines, no nesting deeper than 4 levels, no source file over 500 lines
- Spaces for indentation everywhere except Makefile recipes (tabs)
- Unity test naming: `test_<function>_<condition>` — match existing test files exactly
- No comments unless the WHY is non-obvious

## Quality Rules

Enforced by `scripts/check_repo` and `make verify`:
- No trailing whitespace, no CRLF, no merge conflict markers, no files over 1 MiB
- No committed build output under `build/`
- C compiled with `-Wall -Wextra -Werror`
- Shell scripts: POSIX `sh`, `snake_case` names, executable bit set

## What Comes Next (Hardware Path)

Phase 2 starts with `hardware/rtl/`: vector register file and SIMD ALU, then either an AXI co-processor or tightly coupled pipeline. Phase 3 fills `software/runtime/`: MMIO access, interrupt/completion handling, context save/restore, one-at-a-time scheduling. Phase 4 ports the scalar kernels to the accelerator API and measures speedup.

## Key Reference Documents

- `docs/roadmap.md` — full phase-by-phase plan
- `docs/quality.md` — quality check details
- Arrow paper notes are in `docs/` (summarized from the Arrow architecture paper)
