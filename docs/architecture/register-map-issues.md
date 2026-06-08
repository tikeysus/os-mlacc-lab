# Accelerator Interface Planning

The v1 AXI-attached accelerator interface is divided into four decision-focused GitHub issues. These issues refine the interface portion of issue #1 without committing to an implementation before the design is discussed.

## Issue #7: AXI-Lite Register Map

Define the software-visible registers, offsets, widths, access permissions, reset values, and required fields for the v1 accelerator.

## Issue #6: Command and Status Semantics

Define how software starts an operation, observes busy or completion state, acknowledges completion, handles reset, and responds when a command is submitted at the wrong time.

## Issue #9: Image-Buffer and Address Contract

Define source and destination addressing, pixel format, image dimensions, strides, alignment, border behavior, overlap rules, and memory assumptions.

## Issue #8: Validation and Error Behavior

Define which invalid configurations and runtime failures must be detected, how errors are reported, and how the accelerator returns to a usable state.

Together, these issues establish the contract shared by the future AXI-Lite RTL and software driver.
