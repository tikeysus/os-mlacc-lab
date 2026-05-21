# OS ML Accelerator Lab

This repository is organized around a 10-12 week path for building an Arrow-style ML-oriented accelerator, starting from scalar bring-up and ending with OS/runtime integration and benchmarking.

## Repository Map

- `docs/`: roadmap, reading notes, architecture notes, issue planning, and final-report material
- `hardware/`: FPGA projects, RTL, simulation assets, constraints, and testbenches
- `software/`: scalar benchmarks, accelerator runtime/API code, and software tests
- `tools/`: setup scripts and reproducibility helpers
- `build/`: generated binaries and local build output

## Current Baseline

The current checked-in software baseline is a host-buildable scalar dot product benchmark:

```sh
make
make run
```

This gives the project a simple correctness baseline while the FPGA, simulator, and RISC-V toolchain pieces are added.

## Reading Notes

Track papers, articles, and documentation in `docs/readings/`:

```sh
./new_reading "Arrow paper"
```

Each note includes the source metadata, core idea, project relevance, action items, open questions, and one key quote or figure.
