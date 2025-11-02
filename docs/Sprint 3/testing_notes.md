# Testing Notes — Sprint 3

| Tool             | Command              | Purpose                           | Result                                           |
| ---------------- | -------------------- | --------------------------------- | ------------------------------------------------ |
| **Make (Debug)** | `make debug`         | Compile with debug flags          | ✅ No warnings                                    |
| **Make (Run)**   | `make run`           | Run interactive game              | ✅ Works in all modes (PvP, Easy Bot, Medium Bot) |
| **Unit Tests**   | `make test`          | Run `test_board` & `test_game`    | ✅ All passed                                     |
| **GDB**          | `gdb build/connect4` | Step through `getMediumBotMove()` | ✅ Verified correct move selection logic          |
| **Valgrind**     | `make valgrind`      | Memory safety verification        | ✅ 0 leaks, 0 invalid accesses                    |

## Key Debugging Observations

* Verified `getMediumBotMove()` correctly detects and prioritizes winning and blocking moves.
* Ensured fallback to random valid move only occurs when no strategic move is available.
* Used breakpoints in `playGame()` to confirm smooth alternation between player and bot turns.
* Validated that win and draw detection remain consistent with previous sprints.
* Confirmed clean memory handling under Valgrind with **no leaks or invalid reads/writes**.

## Evidence

Screenshots included in `/docs/Sprint 3/Screenshots/`:

* `gdb1.png` — stepping through `getMediumBotMove()`
* `gdb2.png` — variable inspection and logic verification
* `valgrind1.png` — memory summary showing 0 leaks
* `sprint3_report.md` — full debugging and outcome report
