# Agent Instructions

This repository is an OS and ML accelerator lab for a RISC-V or Arrow-style vector accelerator. Keep the repo navigable and lightweight: prefer small, explicit files and workflows that make future hardware, software, and reading work easy to resume.

## Working Style

- Make practical changes directly when the request is clear.
- Keep structure aligned with the roadmap: research notes in `docs/`, hardware work in `hardware/`, software work in `software/`, and reusable helpers in `scripts/`.
- Prefer durable, low-friction workflows over heavyweight process.
- Commit and push when the user explicitly asks for it.

## Quality Checks

- Run `make verify` before committing code or workflow changes.
- Use `clang-tidy` for Clippy-like C checks when it is available locally.
- Keep shell scripts POSIX `sh` unless there is a clear reason to require another shell.
- Make executable helper scripts live in `scripts/` and use `snake_case` names.
- Keep generated build output out of git.

## Style Conventions

- C functions and variables should use `snake_case`.
- C files should stay small and focused; avoid functions over 80 lines, nesting deeper than 4 levels, and source files over 500 lines unless there is a documented reason.
- Use spaces for C, shell, and Markdown indentation. Makefile recipes must use tabs.
- Markdown notes should have orderly headings and no filler sections.

## Reading Notes

- Reading notes live in `docs/readings/`.
- Create notes with `new_reading "Name"` after installing commands, or `./scripts/new_reading "Name"` from the repo root.
- Every reading note should keep the required metadata and sections filled when it becomes a real note.

## Documentation Bias

- Capture decisions separately from readings when choices become concrete.
- Favor concise architecture, bring-up, benchmark, and runtime notes that can be linked back to GitHub issues.
- Avoid adding broad templates or process files unless they support a repeated workflow.
