# Runtime and API

Use this area for the software interface that will eventually invoke the accelerator.

Expected early content:

- C API wrappers for vector add, dot product, GEMV, and small matrix multiply
- register or MMIO access helpers
- completion, timeout, and error handling
- accelerator state save/restore definitions
- one-user-at-a-time or queued-job policy code
