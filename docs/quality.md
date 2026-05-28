# Quality Checks

This project uses lightweight local checks through `make verify`.

Run the checks manually:

```sh
make verify
```

## Current Checks

- File hygiene: trailing whitespace, final newlines, CRLF line endings, merge conflict markers, and files larger than 1 MiB.
- Shell scripts: syntax via `sh -n`, executable bit, `snake_case` script names, and optional `shellcheck` when installed.
- Markdown: heading order sanity.
- Reading notes: required metadata and required reading-note sections.
- C code: `-Wall -Wextra -Werror`, optional `clang-format` when installed, optional `clang-tidy` when installed, `snake_case` function names, function length under 80 lines, nesting depth at 4 or less, and source/header files under 500 lines.
- Repo structure: generated `build/` output is not committed, old root `src/` and `include/` paths stay retired, and C files stay under `software/` or `hardware/`.

These checks are intentionally modest. Tighten them only when they catch real project problems without making normal work annoying.
