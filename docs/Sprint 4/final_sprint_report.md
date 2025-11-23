# Sprint 4 Final Report — Connect 4

## Team
- Jiany Samara
- Jad Mouawad
- Andy Sleiman

## Sprint Overview
Sprint 4 delivered two headline capabilities for our C-based Connect 4 console game:

1. **Advanced AI (Hard Bot)** — Minimax with alpha-beta pruning, optional multithreading, center weighting, and tactical heuristics.
2. **Network Multiplayer** — Linux-only server/client flow that lets a remote player join with nothing more than a terminal session.

Alongside these features we finalized documentation, automated builds/tests, and ensured memory-safety via Valgrind.

## Documentation Bundle (all under `docs/Sprint 4/`)
- `bot_hard_strategy.md` — Breakdown of the Minimax + alpha-beta + multithreading approach.
- `NETWORK.md` — Single-source overview, architecture, and implementation guidance for the Linux-only multiplayer stack (pairs with root-level `NETWORK_QUICKSTART.md`).
- `testing_notes.md` and `Screenshots/` — Validation evidence and visual captures.
- `NETWORK_QUICKSTART.md` (kept in repo root for easy discovery) — Step-by-step host/client setup.
- This consolidated `final_sprint_report.md` — canonical narrative of Sprint 4 outcomes.

## Sprint Goals & Status
- Integrate medium and hard bots into the main game loop ✅
- Ship a Linux-only remote multiplayer mode with remote terminal client ✅
- Harden quality via unit tests, Valgrind runs, and scripted automation ✅
- Provide multithreaded evaluation path for the Hard Bot ✅

## Feature Highlights

### Hard Bot (Advanced AI)
- Minimax search with alpha-beta pruning and configurable depth.
- Optional multithreaded top-level move evaluation (`make multithread` adds `-pthread` and `-DMT_BUILD`).
- Heuristics: center-column bias, threat detection, and special-case novice traps.
- Reuses existing Easy/Medium scaffolding so all bot tiers coexist in the main loop.

### Network Multiplayer
- Linux-only implementation using Unix sockets for predictable behavior.
- Server runs in the primary game binary; a lightweight (~200 LOC) client connects via terminal using `./build/connect4_client <IP> 8888`.
- Supports remote Player vs Player and Player vs Bot matches with real-time board state syncing and interactive move prompts.
- Designed so the remote participant needs only a terminal—no full game build required on their side.

## Completed Engineering Work
- Core gameplay loop: gravity drop logic plus horizontal/vertical/diagonal win checks and draw detection.
- Bot lineup: Easy (random valid moves), Medium (win/block + center ordering), Hard (described above).
- Tests: `test_board.c`, `test_game.c`, `test_bot_medium.c`, `test_bot_hard.c`, `test_bot_hard_perf.c`, orchestrated via `tests/test_runner.sh`.
- Tooling: Makefile targets `all`, `debug`, `multithread`, `test`, `valgrind`, `gdb-run`; helper scripts for dependency install and Valgrind execution.

## Multithreading Notes
- Each top-level candidate column can spawn a thread that simulates the move and runs the minimax search, improving responsiveness on multi-core hosts.
- Threading remains optional; default builds stay single-threaded for portability.

## Tests & Validation
- Unit suite passes on the Alpine Linux VM target.
- `make valgrind` (debug configuration) shows zero leaks or invalid memory accesses; logs archived in `valgrind.log`.
- Manual sanity checks with `gdb-run` verify stack traces and symbol info.

## Known Issues & Limitations
- Hard Bot depth is bounded by `MAX_DEPTH`; extremely deep tactics may still slip through.
- No transposition table yet, so repeated positions are recomputed.
- Multithreaded mode increases CPU usage; thread pooling remains future work.

## Future Work
- Add Zobrist hashing with iterative deepening and a time manager.
- Introduce a thread pool plus command-line flags for bot depth/time limits.
- Explore a lightweight UI (ncurses or web front-end) to visualize games.

## Quick Reference
- Build (clean): `make clean && make`
- Multithreaded build: `make multithread`
- Run host (select network modes 5–8 inside menu): `./build/connect4`
- Run remote client: `./build/connect4_client <IP> 8888`
- Automated tests: `make test`

## Appendix
- Scripts: `scripts/install-deps.sh`, `scripts/run-valgrind-all.sh`
- Networking instructions duplication avoided by linking to `NETWORK_QUICKSTART.md`
