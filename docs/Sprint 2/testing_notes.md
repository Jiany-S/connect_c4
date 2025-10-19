# Testing Notes — Sprint 2

| Tool | Command | Purpose | Result |
|------|----------|----------|---------|
| **Make (Debug)** | `make debug` | Compile with debug flags | ✅ No warnings |
| **Make (Run)** | `make run` | Run interactive game | ✅ Works with both modes |
| **Unit Tests** | `make test` | Run `test_board` & `test_game` | ✅ All passed |
| **GDB** | `gdb build/connect4` | Step through `playGame()` | ✅ Verified control flow |
| **Valgrind** | `make valgrind` | Memory safety verification | ✅ 0 leaks, 0 invalid accesses |

## Key Debugging Observations
- Verified `dropChecker()` correctly rejects full columns.
- Checked that `getEasyBotMove()` only selects non-full columns.
- Used breakpoints in `playGame()` to confirm correct mode branching.
- No memory leaks or buffer overflows detected.

## Evidence
Screenshots included in `/docs/screenshots/`:
- `build_run.png` — compilation and game start
- `gdb_trace.png` — debugger trace
- `valgrind_output.png` — leak-free summary