# Project Roadmap

This roadmap tracks the 10–12 week path for the GEMM accelerator project.
The hardware platform (Kria KV260 vs Arty A7 + RISC-V) is deferred until Phase 3.
Phases 0–2 are entirely platform-agnostic.

## Phase 0: GEMM Foundation, Week 1

Goal: freeze the hardware-software interface contract before any RTL or driver code is written.

- Write `docs/architecture/gemm-architecture.md`: platform options, systolic array sizing, block diagram.
- Write `docs/architecture/gemm-register-map.md`: eight CSRs (CTRL, STATUS, DIM_M, DIM_K, DIM_N, A_ADDR, B_ADDR, C_ADDR), offsets, bit semantics, alignment constraints.
- Define `software/runtime/gemm_driver.h`: four-function API (`gemm_accel_init`, `gemm_accel_run`, `gemm_accel_wait`, `gemm_accel_status`).
- Gate: register map and driver header are frozen as documents before Phase 2 RTL starts.

## Phase 1: Scalar GEMM Baseline, Weeks 2–3

Goal: working scalar reference and benchmark that `make verify` accepts.

- Add `gemm_f32` and `gemm_i8` to `software/include/ml_kernels.h`.
- Implement `software/benchmarks/scalar/gemm.c` (naive triple-loop; `gemm_f32` calls `dot_product_f32`).
- Add `software/tests/test_gemm.c` with at least 8 Unity tests covering identity, rectangular, K=0, and M=1 vs gemv cross-check.
- Add `software/benchmarks/scalar/gemm_benchmark.c`: times (M,K,N) sweep from (8,8,8) to (128,128,128), prints results.
- Record CPU scalar timings in `docs/benchmarks/scalar-baseline.md`.
- `make verify` passes with zero failures.

## Phase 2: RTL — Systolic Array and AXI-Lite CSRs, Weeks 4–7

Goal: simulate a correct 4×4 × 4×4 GEMM in software before touching hardware.

- `hardware/rtl/pe.v`: single fused-MAC cell with registered a/b pass-through and local accumulator.
- `hardware/rtl/systolic_array.v`: parameterized N×N grid of PEs; A fed row-wise, B column-wise.
- `hardware/rtl/axi_lite_csr.v`: implements the eight registers from the register map.
- `hardware/rtl/gemm_top.v`: integrates all three with an IDLE→LOAD→COMPUTE→DONE state machine.
- `hardware/sim/tb_gemm_top.v` + `hardware/sim/run_sim`: Icarus Verilog simulation passes 4×4 case.
- `software/runtime/gemm_driver_sim.c`: stub that calls `gemm_f32` instead of MMIO, for API testing without hardware.

## Phase 3: Platform Bring-Up and Hardware Driver, Weeks 7–9

Goal: driver talks to real hardware and produces correct output.

- Choose platform (Kria KV260 or Arty A7 + RISC-V soft-core).
- Vivado block design (Kria) or LiteX SoC config (Arty): synthesize and flash `gemm_top`.
- Implement `software/runtime/gemm_driver.c` using MMIO (`/dev/mem` on Linux or bare-metal volatile writes).
- End-to-end test: driver → hardware → C output matches `gemm_f32` scalar reference.

## Phase 4: Benchmarking and Analysis, Weeks 9–10

Goal: measure and document speedup numbers.

- Sweep (M,K,N) = (8,8,8) through (64,64,64) on hardware vs scalar baseline.
- Measure: compute-only time, DMA/copy time, total wall time.
- Report: speedup table, memory bottleneck analysis, FPGA resource utilization (LUTs, DSPs, BRAMs).
- Write `docs/benchmarks/accelerator-results.md`.

## Phase 5: Robustness, Documentation, and Stretch Goals, Weeks 11–12

Goal: portfolio-quality artifact.

- Boundary tests: M=0, K=0, misaligned addresses, dimensions exceeding BRAM capacity.
- RTL assertions in `gemm_top.v` for invalid input conditions.
- Final technical report: architecture decisions, register map, driver API, benchmark results, timing charts.
- Stretch: i8 systolic array (int8 × int8 → int32 PEs), or double-buffered tile pipeline for overlap.
