# Sprint 2 Report

## Objective
Implement an "Easy Bot" mode for Connect 4 that plays random valid moves against the player.

## Design Summary
- Added a mode selector for Player vs Player and Player vs Bot (easy).
- Implemented `getEasyBotMove()` in `bot_easy.c` that chooses a random non-full column.
- Integrated bot logic into `playGame()` for alternating human/bot turns.
- Retained the same win and draw detection logic from Sprint 1.

## Testing & Debugging
- Compiled project with `make debug` — no warnings or errors.
- Ran manual games in both PvP and PvE modes — correct gameplay flow.
- Verified logic flow and variable states using GDB.
- Ran Valgrind memory checks — **0 leaks, 0 invalid reads/writes.**
- Confirmed all unit tests (`test_board.c`, `test_game.c`) passed successfully.

## Outcome
- Stable, leak-free bot mode implemented.
- Bot correctly avoids full columns and alternates turns.
- Codebase remains modular and consistent with Sprint 1 conventions.

---
**Next Steps:**  
- Sprint 3 will introduce a Medium Bot, using basic heuristics and lookahead evaluation.
