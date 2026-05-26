# Next Week Issues

These are the next high-value project planning and bring-up tasks mirrored as GitHub issues.

The active label scheme for these and future issues is documented in `docs/issue-labels.md`.

## Define project scope and architecture for the RISC-V vector accelerator

Read and summarize the Arrow reference design and select the implementation approach for this project. Decide the FPGA board, base scalar core, and integration style: either an AXI-attached accelerator or a tightly coupled RISC-V vector unit. Produce a one-page architecture note with a block diagram, a minimal v1 feature set, and the exact ML kernels to target first, for example vector add, dot product, and a small matrix multiply. Include the planned accelerator interface, signal responsibilities, and the assumptions about clock, memory, and peripheral support.

## Set up FPGA development environment and baseline SoC bring-up

Create the FPGA project targeting the chosen board and confirm that the basic SoC can synthesize, implement, and boot successfully. The baseline design must include the scalar CPU core, on-chip memory, and UART output, and it must run a "hello world" test on hardware. If applicable, also verify timer/interrupt support and document any board-specific constraints, pin assignments, or resource limits that affect the accelerator integration.

## Establish RISC-V toolchain, simulator, and benchmark baseline

Install and validate the RISC-V compiler/toolchain with vector support, plus a simulator for functional testing. Build and run at least one small vector test in simulation and create a scalar baseline benchmark program that performs one target ML-style kernel on the CPU only. The benchmark should compile cleanly, run in simulation, and print correct output over UART or the simulator console. Record the exact compiler flags, simulator settings, and expected output so the baseline can be reproduced later.

## Define software API and accelerator state-management requirements

Specify the initial software interface for invoking the accelerator from C or a lightweight runtime layer. Document the function names, argument formats, data layout expectations, and how completion is detected. In the same issue, define which accelerator state must be saved and restored during context switches, including any custom registers, control/status fields, or vector state. Include a short note on how jobs will be serialized or queued when more than one process wants to use the accelerator.
