# Benchmark Notes

Use this area for benchmark definitions and results.

Files to add:

- `scalar-baseline.md`: gemm_f32 and gemm_i8 timing across (M,K,N) sweep on the development CPU
- `accelerator-results.md`: speedup vs scalar, compute-only vs end-to-end wall time, FPGA resource utilization (LUTs, DSPs, BRAMs)
- `overhead-breakdown.md`: DMA transfer time, driver call overhead, data movement vs compute ratio
