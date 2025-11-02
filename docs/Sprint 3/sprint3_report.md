# Sprint 3 Report

## Objective

Implement a **Medium Bot** mode for Connect 4 that makes smarter, non-random moves based on basic heuristics and board evaluation.

## Design Summary

* Added a new game mode: **Player vs Bot (Medium)**, selectable from the main menu.
* Implemented `getMediumBotMove()` in `bot_medium.c`, which:

  * Checks for immediate **winning moves** for the bot.
  * Blocks the player’s potential **winning moves** in the next turn.
  * Falls back to a random valid move when no strategic move is available.
* Updated `playGame()` to integrate medium bot logic alongside easy bot and player modes.
* Reused existing validation, win, and draw detection functions from previous sprints to maintain modularity.

## Testing & Debugging

* Compiled using:

  ```bash
  make clean
  make CFLAGS="-O0 -g -Wall -Wextra -Wshadow"
  ```

  → No warnings or errors produced.
* Validated program behavior through manual gameplay in all modes (PvP, Easy Bot, Medium Bot).
  Confirmed correct alternation of turns and bot responses.
* Used **GDB** to trace `getMediumBotMove()` decision-making and verify correct detection of blocking/winning opportunities.
* Ran **Valgrind** memory analysis:

  ```
  All heap blocks were freed -- no leaks are possible
  ERROR SUMMARY: 0 errors from 0 contexts
  ```

  → **0 memory leaks, 0 invalid reads/writes.**
* All existing test files (`test_board.c`, `test_game.c`) passed successfully.
  Added new tests to verify `getMediumBotMove()` decision logic.

## Outcome

* **Medium Bot** successfully prioritizes winning and blocking moves.
* Game remains **stable, leak-free, and logically sound** under Valgrind and GDB analysis.
* Code organization continues to follow modular structure from previous sprints, ensuring maintainability.

---

**Next Steps:**

* Sprint 4 will focus on implementing a **Hard Bot** with deeper move evaluation (minimax or heuristic scoring) and further optimizations for move efficiency.
