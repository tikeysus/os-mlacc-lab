# Runtime and Driver

Software interface for invoking the GEMM accelerator.

Files to add:

- `gemm_driver.h`: four-function API — `gemm_accel_init`, `gemm_accel_run`, `gemm_accel_wait`, `gemm_accel_status`
- `gemm_driver_sim.c`: simulation stub; implements the API by calling `gemm_f32` instead of MMIO — testable on any laptop
- `gemm_driver.c`: MMIO implementation (`/dev/mem` mmap on Linux, or bare-metal volatile writes); written in Phase 3 once the board is chosen
