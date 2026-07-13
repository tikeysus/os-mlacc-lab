# Architecture Notes

This directory contains platform-agnostic hardware-software architecture decisions, register interface specifications, and design methodology documents.

## Files

- `gemm-architecture.md`: Phase 0 architecture documentation — systolic array sizing methodology (parameterized N×N PEs), clock and BRAM budget assumptions, and generic PS↔AXI-Lite↔PL block diagram. Platform selection is deferred to Phase 3 (see issue #3).
- `gemm-register-map.md`: eight AXI-Lite CSRs (CTRL, STATUS, DIM_M, DIM_K, DIM_N, A_ADDR, B_ADDR, C_ADDR), offsets, bit semantics, and alignment constraints. Frozen before Phase 2 RTL work begins.
