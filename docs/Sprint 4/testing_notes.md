# Testing Notes — Sprint 4

| Tool             | Command              | Purpose                        | Result                                             |
| ---------------- | -------------------- | ------------------------------ | -------------------------------------------------- |
| **Make (Debug)** | `make debug`         | Compile with debug flags       | ✅ No warnings                                      |
| **Make (Run)**   | `make run`           | Run interactive game           | ✅ Hard Bot (Mode 4) runs correctly                 |
| **Unit Tests**   | `make test`          | Run `test_board` & `test_game` | ✅ All passed                                       |
| **GDB**          | `gdb build/connect4` | Step through `getHardMove()`   | ✅ Verified correct move selection and bot behavior |
| **Valgrind**     | `make valgrind`      | Memory safety verification     | ✅ 0 leaks, 0 invalid accesses                      |

## Key Debugging Observations

* Verified that Mode 4 correctly calls `getHardMove()` for the Hard Bot.
* Stepped through bot logic in GDB to confirm valid column selection and no out-of-range indexing.
* Confirmed smooth alternation between player and bot turns inside `playGame()`.
* Tested edge cases including full columns, forced wins, and forced blocks.
* Under Valgrind, the program showed **no memory leaks or invalid reads/writes**.
* Ensured Hard Bot never selects invalid columns and behaves consistently with intended strategy.

## Evidence

Screenshots included in `/docs/Sprint 4/Screenshots/`:

* `gdb1.png` — stepping through `getHardMove()`
* `gdb2.png` — variable inspection and logic verification
* `valgrind1.png` — memory summary showing 0 leaks
* `sprint4_report.md` — full debugging and outcome report