# GEMM Accelerator Architecture (Phase 0)

## Overview

This document defines the architecture for the v1 GEMM (C = A × B) accelerator in a **platform-agnostic** manner. All design decisions through Phase 2 are independent of the final FPGA platform (Kria KV260 vs Arty A7 + RISC-V soft-core); the platform itself will be selected in Phase 3 (see issue #3) after RTL simulation is complete.

The accelerator is implemented as an **AXI-Lite attached systolic array**, paired with a host processor via an AXI-Lite CSR interface. The design prioritizes correctness and simplicity over maximum throughput in v1.

## Systolic Array Sizing

### Methodology

The core compute engine is an **N×N grid of processing elements (PEs)**, where N is parameterized and chosen per platform constraints discovered in Phase 3.

**Phase 0 assumption: N = 8** (8×8 PE array). This is a reasonable starting point for mid-range FPGAs and a conservative target for resource-constrained platforms.

#### Per-PE Cost Estimate

Each PE implements a fused multiply-accumulate (MAC) cell:
- **32-bit multiplier**: ~160–220 LUTs (platform-dependent)
- **Registered A/B pass-through**: 64 bits registered = ~64 FFs
- **32-bit accumulator**: ~32 FFs
- Local control (mux, clear): ~20 LUTs

**Per-PE footprint: ~280–340 LUTs + ~96 FFs** (order of magnitude).

#### Array-Level Cost

For an 8×8 array:
- PE grid: 64 PEs × 300 LUTs ≈ **19.2K LUTs** (rough estimate)
- Local scratch BRAM for A/B input buffering and C accumulation: **~500 KB BRAM** (all on-chip)

Scaling: cost grows O(N²). For N=4 (resource-constrained platforms), divide by 4. For N=16 (resource-rich platforms), multiply by 4.

### Tile and Input Blocksize

The systolic array processes one tile per compute phase:
- **Tile dimensions**: M×K × K×N (where M, K, N are matrix dimensions)
- For Phase 2 RTL simulation, assume **4×4 tile** (a 4×4 GEMM per iteration)
- Phase 4 benchmarks will sweep tiles up to **64×64** on real hardware

The choice of tile is orthogonal to PE array size. A smaller tile can be processed by a larger array through time-multiplexed summation.

## Hardware-Software Interface

### Block Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    Host Processing System (CPU)              │
└────────────────────────┬────────────────────────────────────┘
                         │ AXI-Lite
                         ▼
┌─────────────────────────────────────────────────────────────┐
│              AXI-Lite CSR Slave (Registers)                  │
│  [CTRL] [STATUS] [DIM_M] [DIM_K] [DIM_N]                    │
│  [A_ADDR] [B_ADDR] [C_ADDR]  (offsets 0x00–0x1C)           │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                  State Machine (IDLE→LOAD→COMPUTE→DONE)      │
└────────────────────────┬────────────────────────────────────┘
                         │
        ┌────────────────┼────────────────┐
        ▼                ▼                ▼
   ┌─────────┐     ┌──────────┐     ┌───────────┐
   │Systolic │     │BRAM      │     │Bus Bridge │
   │Array    │────▶│(A,B,C)   │     │(to DDR/   │
   │N×N PEs  │     │Buffers   │◀────│ External) │
   └─────────┘     └──────────┘     └───────────┘
```

**Data Flow:**
1. CPU writes matrix dimensions (DIM_M, DIM_K, DIM_N) and base addresses (A_ADDR, B_ADDR, C_ADDR) to CSRs
2. CPU sets CTRL.START bit
3. State machine reads A and B from memory into on-chip BRAM
4. Systolic array computes C = A × B over several cycles
5. State machine writes C back to memory
6. State machine sets STATUS.DONE
7. CPU polls STATUS.DONE or awaits interrupt (v1: polling only)

### Clocking and Synchronization

**Clock assumptions:**
- **Single clock domain** (no CDC): both CPU and accelerator run on the same clock
- **Nominal frequency: 100 MHz** (subject to timing closure in Phase 3)
- **Pipeline depth**: ~3–5 cycles per PE (registered A/B pass-through and accumulation)

Once a platform is selected, timing analysis will validate this assumption. Resource-constrained platforms typically achieve 80–100 MHz; higher-end platforms can exceed 200 MHz with conservative synthesis.

### Memory Hierarchy

**On-chip (BRAM):**
- A tile buffer: M×K FP32 = M×K×4 bytes
- B tile buffer: K×N FP32 = K×N×4 bytes
- C accumulator: M×N FP32 = M×N×4 bytes
- Phase 2 example (4×4 tile): 4×4×4 + 4×4×4 + 4×4×4 = **192 bytes** (negligible)

**Off-chip (DDR or system memory):**
- Full A (M×K), B (K×N), C (M×N) matrices
- Phase 3 will set up the bus bridge for external memory access

For Phase 2 simulation, memory is modeled as an idealized read/write port with 1–2 cycle latency.

## Register Map

Eight AXI-Lite CSRs (32-bit each), fully documented in `gemm-register-map.md`:

| Offset | Name | Purpose |
|--------|------|---------|
| 0x00 | CTRL | START bit, enable, error clear |
| 0x04 | STATUS | DONE bit, error flags, busy |
| 0x08 | DIM_M | Matrix height (M) |
| 0x0C | DIM_K | Reduction dimension (K) |
| 0x10 | DIM_N | Matrix width (N) |
| 0x14 | A_ADDR | Base address of input matrix A |
| 0x18 | B_ADDR | Base address of input matrix B |
| 0x1C | C_ADDR | Base address of output matrix C |

CSRs are frozen per issue #10 before Phase 2 RTL begins.

## Design Assumptions and Constraints

**v1 Scope:**
- Single job in flight (no multi-queue, no context switching)
- 32-bit floating point (f32) arithmetic
- Row-major matrix layout in memory
- Synchronous design, single clock domain
- One clock per PE per cycle (no pipelining optimization within the array)

**Out of Scope (Phase 5+ or defer):**
- Dynamic arbitrary-size convolution kernels
- Floating-point exceptions (denormals, infinities)
- Cache coherency
- Interrupt-driven completion (polling only in v1)
- Multi-PE pipelining or systolic optimization beyond the basic dataflow

## Validation Plan

**Phase 2:** Icarus Verilog simulation of `gemm_top.v` with a 4×4 case (both array and tile can be smaller for sim speed).

**Phase 3:** After platform selection, re-validate timing assumptions and BRAM budget against the real chip.

**Phase 4:** Benchmark on both implementations (if both are built) to measure any clock-rate or resource-utilization differences.

---

**See also:**
- `docs/roadmap.md` — full 10-week project timeline
- `docs/architecture/gemm-register-map.md` — CSR details (issue #10)
- `hardware/rtl/` — Verilog implementation (issue #11, Phase 2)
- Issue #3 — platform selection and bring-up (Phase 3)
