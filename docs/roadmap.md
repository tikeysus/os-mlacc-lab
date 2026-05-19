# Project Roadmap

This roadmap tracks the rough 10-12 week path for the RISC-V vector accelerator project.

## Phase 0: Research and Setup, Week 1

Goal: understand Arrow's design and get the tools and board workflow ready.

- Read the Arrow paper and slides, then summarize architecture and benchmarks.
- Pick the FPGA board and scalar core.
- Set up FPGA tools, a RISC-V toolchain with RVV support, and Spike or another simulator.
- Run "hello world" on the scalar core in simulation and on FPGA.

## Phase 1: Baseline RISC-V SoC Bring-Up, Weeks 2-3

Goal: create a stable scalar SoC on FPGA as the reference platform.

- Build an SoC with a scalar RISC-V core, on-chip memory, UART, and timer.
- Add basic interrupt handling for timer and UART.
- Implement scalar ML-style kernels: dot product, GEMV, and small convolution.
- Measure cycle counts and verify behavior in both simulator and FPGA runs.

## Phase 2: Vector or Custom Extension RTL, Weeks 4-6

Goal: design and implement a minimal Arrow-like vector unit.

- Choose the first kernels and vector ISA subset.
- Design vector register file and SIMD ALU micro-architecture.
- Implement either an AXI-attached co-processor or a tightly coupled vector pipeline.
- Develop RTL testbenches for vector instructions and kernel-level behavior.

## Phase 3: OS and Runtime Integration, Weeks 7-8

Goal: make the accelerator a usable system resource with a clean API.

- Define kernel or driver interface, including registers, interrupts, and error handling.
- Define user-level C APIs or intrinsics.
- Save and restore vector or accelerator state during task switches.
- Add basic scheduling policy for one user at a time or queued jobs.
- Add timeout and error handling for stalled operations.

## Phase 4: Benchmarking, Evaluation, and Tuning, Weeks 9-10

Goal: show clear speedup and overhead numbers.

- Port scalar kernels to the accelerator API or RVV intrinsics.
- Measure speedup versus scalar in cycles and wall time.
- Estimate power or energy when possible.
- Analyze context switch, driver call, and data movement overheads.
- Tune vector length, tiling, and scheduling policy.

## Phase 5: Robustness, Documentation, and Stretch Goals, Weeks 11-12

Goal: make the project credible as a portfolio-quality artifact.

- Add boundary tests for short vectors, misaligned data, and multiple tasks.
- Add debug/status registers and RTL assertions.
- Write the final technical report with ISA subset, micro-architecture, OS integration, benchmarks, diagrams, and timing charts.
- Explore one extra ML-oriented instruction such as ReLU or quantization support.
