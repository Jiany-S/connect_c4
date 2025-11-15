# Final Sprint Report — Connect 4 (Systems Programming course)

## Project
Connect 4 console game implemented in C with player vs player and player vs bot modes (Easy, Medium, Hard).

## Team
- Jiany Samara
- Jad Mouawad
- Andy Sleiman

## Sprint Goals (final sprint)
- Integrate medium and hard bots into main game loop.
- Add unit tests and ensure Valgrind shows no leaks.
- Document repository and provide automation scripts (Makefile).
- Add optional multithreaded hard-bot evaluation for bonus credit.

## Completed Work
- Core gameplay: drop logic, win/draw detection (horizontal, vertical, diagonal).
- Bots:
  - **Easy**: random valid moves.
  - **Medium**: heuristics — detect immediate win/block and prefer center columns.
  - **Hard**: minimax with alpha-beta, windowed evaluation, center bonus, threat detection, and special-case novice-trap detection.
- Tests:
  - `test_board.c`, `test_game.c`, `test_bot_medium.c` (unit tests).
  - `test_runner.sh` to run tests automatically.
- Dev tooling:
  - Makefile targets: `all`, `debug`, `multithread`, `test`, `valgrind`, `gdb-run`.
  - `scripts/` for dependency/valgrind automation.
- Documentation:
  - README (updated).
  - `bot_hard_strategy.md`.
  - `testing_notes.md`.
  - `final_sprint_report.md` (this file).

## Multithreading (bonus)
- Implemented optional multithreaded top-level move evaluation in `bot_hard.c` (guarded by `-DMT_BUILD`).
- Build with `make multithread` to enable `-pthread`.
- Threading is per-candidate column: each thread simulates the top-level move and runs minimax on its board copy.

## Tests & Validation
- All unit tests pass locally (Alpine VM).
- Valgrind checks (when run with debug build) show no leaks or invalid reads/writes (see `valgrind.log`).
- GDB backtrace tested with `gdb-run` for sanity.

## Known issues & Limitations
- Hard bot search is still bounded by `MAX_DEPTH` — can miss tactical lines deeper than configured.
- No transposition table: repeated states are recomputed.
- Multithreaded build increases CPU usage; thread count can be optimized further.

## Future work
- Add transposition table (Zobrist hashing) and iterative deepening.
- Implement time-limited search and a thread pool to limit concurrency to CPU count.
- Add a small CLI flag for bot difficulty and time limit.
- Create a simple UI (ncurses or minimal Web UI) for better demonstration.

## Appendix
- Build: `make multithread` (enables pthread and -DMT_BUILD)
- Run: `make run`
- Tests: `make test`
