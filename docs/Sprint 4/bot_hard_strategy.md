# Hard Bot Strategy - Connect 4

## Overview
The Hard Bot is designed to play at a high level using **minimax with alpha-beta pruning**. It evaluates the board by considering multiple heuristics and potential threats. The bot also supports **multithreading** to parallelize the evaluation of possible moves, speeding up decision-making in deeper game scenarios.

---

## 1. Core Algorithm

### Minimax
- The bot recursively explores possible future moves up to a maximum depth (`MAX_DEPTH`).
- It alternates between **maximizing** the bot’s score and **minimizing** the opponent’s score.
- Alpha-beta pruning is used to **cut off unnecessary branches**, improving efficiency.

### Evaluation Function
The bot scores board positions using the following heuristics:

1. **Window Scoring**
   - All horizontal, vertical, and diagonal windows of length 4 are analyzed.
   - Scores are assigned based on the number of AI pieces, opponent pieces, and empty cells in the window:
     - 4 AI pieces → +10000
     - 3 AI pieces + 1 empty → +100
     - 2 AI pieces + 2 empty → +10
     - 3 opponent pieces + 1 empty → -120
     - 4 opponent pieces → -10000
     - 2 opponent pieces + 2 empty → -10

2. **Center Column Bonus**
   - Pieces in the center column are more valuable as they contribute to more potential wins.
   - Score = `CENTER_WEIGHT * number of AI pieces in center column`.

3. **Double Threat Detection**
   - If the AI can create 2+ simultaneous threats, it gains `DOUBLE_THREAT_BONUS`.
   - If the opponent can create 2+ threats, it incurs `DOUBLE_THREAT_PENALTY`.

4. **Novice Trap Bonus**
   - Detects patterns in the bottom row that may lead to traps or easy wins.
   - Provides `NOVICE_TRAP_BONUS` for 3-in-a-row with an open fourth slot in the bottom row.

---

## 2. Move Selection

### Single-Threaded
- Each valid move is simulated individually.
- Minimax evaluates the resulting board.
- The move with the highest score is selected.
- If multiple moves have the same score, the one closest to the center is chosen.

### Multithreaded
- Each valid move is assigned to a separate thread.
- Threads run **minimax independently** for each move.
- After all threads finish, the best-scoring move is selected.
- This speeds up **deep calculations** when branching is high.

---

## 3. Thread Management
- `ThreadArg` structure stores board copy, column, player, and evaluation score.
- Worker threads run `thread_worker`, which calls minimax for a specific move.
- Threads are joined before selecting the best move.
- Optional: If thread creation fails, fallback to single-threaded evaluation.

---

## 4. Implementation Notes
- `MAX_DEPTH` controls the difficulty. Higher values increase computation time exponentially.
- `USE_THREADS` macro allows switching between multithreaded and single-threaded mode.
- The bot **does not modify the original board** during simulations.
- Minimax uses `INT_MIN` / `INT_MAX` for alpha-beta pruning bounds.

---

## 5. Summary of Heuristics and Weights

| Heuristic                       | Weight / Bonus       |
|---------------------------------|-------------------|
| 4-in-a-row (AI)                  | +10000            |
| 3-in-a-row (AI)                  | +100              |
| 2-in-a-row (AI)                  | +10               |
| 4-in-a-row (Opponent)            | -10000            |
| 3-in-a-row (Opponent)            | -120              |
| 2-in-a-row (Opponent)            | -10               |
| Double AI Threat                 | +800              |
| Double Opponent Threat           | -900              |
| Novice Trap (Bottom Row)         | +200              |
| Center Column Bonus              | +7 per piece      |

---

## 6. Notes for Usage
- For **real game scenarios**, increase `MAX_DEPTH` to 8–10 for a challenging bot.
- Use multithreading (`USE_THREADS = 1`) only if the computation per move is large enough to overcome thread overhead.
- Works seamlessly with `getHardMove` for normal gameplay or `getHardMoveWithThreads` for testing multithreaded performance.

---

## 7. References
- [Minimax Algorithm](https://en.wikipedia.org/wiki/Minimax)
- [Alpha-Beta Pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)
- Connect 4 strategies and heuristics from classic AI tutorials.
