# Bring-Up Notes

Use this area to record reproducible setup and board bring-up details.
Platform is TBD (Kria KV260 or Arty A7 + RISC-V); files below are written once the board is chosen.

Files to add:

- `toolchain.md`: compiler setup (aarch64-linux-gnu-gcc for Kria, or riscv32-unknown-elf-gcc for Arty), simulator commands, expected output
- `fpga-board.md`: synthesis steps, board constraints, pin assignments, AXI-Lite base address, resource limits
- `iverilog-sim.md`: steps to run hardware/sim/run_sim on any machine without a Vivado license
