# Medium Bot Strategy (Sprint 3)

## Algorithm Overview

### Check for a winning move
- Iterate through all columns from 0 to COLS - 1.
- For each available column, simulate dropping the bot's checker.
- If the move results in an immediate win (checkWin() returns true), play that column.

### Check for a blocking move
- If no winning move exists, simulate the opponent's move for each column.
- If the opponent would win next turn by playing in a specific column, place the bot's checker there to block.

### Select a preferred column
- When no immediate win or threat exists, choose among remaining valid columns based on center-first preference.
- Preferred order (0-based): {3, 4, 2, 5, 1, 6, 0} — keeps play near the middle to increase connection opportunities.

### Fallback
- If all else fails, select the first available valid column as a safe default.
- Return the chosen column index to the main gameplay loop.

## Behavior Summary
- Looks one move ahead to either win or block.
- Plays strategically near the center for positional advantage.
- Never plays in a full column.
- Offers noticeably smarter and more challenging gameplay than the Easy Bot.

## Complexity Analysis
The bot inspects up to 7 columns, simulates a move, and checks for wins.

Each simulation involves copying the 6×7 board and calling checkWin(), which operates in constant time for a fixed-size grid.

Overall time complexity:

O(ROWS×COLS)≈O(1)

due to the fixed Connect 4 board dimensions.

## Future Improvements
**Hard Bot (Sprint 4)**:
- Expand lookahead depth using a minimax or alpha-beta pruning approach.
- This will allow evaluating multiple future turns and selecting the move that maximizes long-term advantage.