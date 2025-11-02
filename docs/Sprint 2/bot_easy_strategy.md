# Easy Bot Strategy (Sprint 2)

## Algorithm Overview
1. Randomly select a column between 0 and COLS - 1.  
2. Check if the column is full using `COLUMN_FULL(board, col)`.  
3. If full, retry until a valid column is found.  
4. Return the valid column index to `playGame()`.  
5. Display the chosen move (1-based index).

## Behavior Summary
- Makes only valid moves.
- Does not plan ahead; purely random among available columns.
- Provides a simple opponent for testing core mechanics.

## Future Improvements
- **Medium Bot:** Simulate immediate winning and blocking moves.
- **Hard Bot:** Add a scoring system to evaluate move outcomes 1–2 turns ahead.