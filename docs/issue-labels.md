# Issue Labels

Use labels to make project planning and future task assignment easier.

## Priority

- `priority: urgent`: early work that blocks or unlocks near-term project progress.
- `priority: long-term`: important later-phase work that should stay visible while earlier bring-up work happens.

## Phase

- `phase: research`: scoping, architecture reading, design comparison, and planning.
- `phase: bring-up`: FPGA, SoC, board, UART, timer, simulator, and baseline platform work.
- `phase: tooling`: compiler, simulator, scripts, reproducibility, and benchmark harness setup.
- `phase: runtime`: software API, driver, OS integration, context switching, and accelerator state management.

## Type

- `type: architecture`: architecture notes, design decisions, interfaces, and tradeoffs.
- `type: hardware`: FPGA, RTL, SoC, board constraints, and hardware integration.
- `type: software`: C code, runtime, driver, API, and OS-facing work.
- `type: benchmark`: benchmark programs, measurement methodology, expected output, and results.

Keep the label set small. Add a new label only when it helps decide priority, ownership, project phase, or review context.
