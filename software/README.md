# Software Workspace

This directory contains host-side baselines, future RISC-V benchmarks, runtime/API code, and tests.

- `benchmarks/scalar/`: CPU-only reference kernels
- `include/`: shared software headers
- `runtime/`: future accelerator API, driver-facing helpers, and state-management code
- `tests/`: correctness and regression tests

The edge-detection scalar reference should start in `benchmarks/scalar/image_kernels.c`,
with public declarations in `include/image_kernels.h` and correctness coverage in
`tests/test_image_kernels.c`.
