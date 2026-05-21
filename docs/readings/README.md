# Reading Notes

Use this directory to track papers, articles, documentation, talks, and other sources that influence the accelerator project.

Create a new reading note from the repository root with:

```sh
./scripts/new_reading "Arrow paper"
```

For the shorter command, run the installer once:

```sh
./scripts/install_commands
new_reading "Arrow paper"
```

The command creates a Markdown file in this directory using a consistent template.

## Naming

Use short, descriptive names. The helper converts the name into a safe lowercase filename.

Examples:

- `./scripts/new_reading "Arrow paper"` creates `docs/readings/arrow-paper.md`
- `./scripts/new_reading "RVV toolchain notes"` creates `docs/readings/rvv-toolchain-notes.md`
- `new_reading "Arrow paper"` works after running `./scripts/install_commands`

## Template Principle

Every section in the note should be useful enough to fill. Keep entries concise so reading notes stay sustainable over the full project.
