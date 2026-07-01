# Architecture Notes

Use this area for design decisions and diagrams.

Files to add:

- `gemm-architecture.md`: platform decision (Kria KV260 vs Arty A7 + RISC-V), systolic array sizing, block diagram
- `gemm-register-map.md`: eight AXI-Lite CSRs (CTRL, STATUS, DIM_M, DIM_K, DIM_N, A_ADDR, B_ADDR, C_ADDR), offsets, bit semantics, alignment constraints

Existing reference:

- `arrow-summary.md`: notes from the Arrow architecture paper (background research)
