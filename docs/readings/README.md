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

The command creates a folder in this directory using a consistent template:

```text
docs/readings/arrow-paper/
  notes.md
  paper.pdf
```

Keep the note in `notes.md`. Put the source PDF for the paper at `paper.pdf` so the note and paper stay together and can be committed together.

## Naming

Use short, descriptive names. The helper converts the name into a safe lowercase folder name.

Examples:

- `./scripts/new_reading "Arrow paper"` creates `docs/readings/arrow-paper/notes.md`
- `./scripts/new_reading "RVV toolchain notes"` creates `docs/readings/rvv-toolchain-notes/notes.md`
- `new_reading "Arrow paper"` works after running `./scripts/install_commands`

## PDFs

Store the paper PDF beside the note as `paper.pdf`. These PDFs are intended to be staged and committed when they are part of the shared reading context:

```sh
git add docs/readings/arrow-paper/notes.md docs/readings/arrow-paper/paper.pdf
```

Keep PDFs reasonably small. If a paper PDF is unusually large, prefer a compressed copy or discuss Git LFS before committing it.

## Template Principle

Every section in the note should be useful enough to fill. Keep entries concise so reading notes stay sustainable over the full project.
