# Sprint 1 Report – Connect 4 in C

## Overview
This report documents the work completed for **Sprint 1** of the Connect 4 project in CMPS 241 – Systems Programming.  
The goal of Sprint 1 was to implement a console-based two-player Connect Four game that runs in a Linux virtual machine without a graphical interface.  
The game launches automatically when the VM boots.

---

## Design Strategy

### Board Representation
- The board was implemented as a 2D array of size 6x7 (rows x columns).
- Each cell can contain one of three values:
  - `A` → Player A’s checker
  - `B` → Player B’s checker
  - `.` → Empty slot
- Constants for dimensions and tokens are declared in `board.h` for consistency.

### Game Flow
- The game alternates between Player A and Player B.
- On each turn:
  1. The player selects a column.
  2. The checker is placed in the lowest available row of that column.
  3. The board is updated and reprinted.
- The game ends when:
  - A player aligns four checkers horizontally, vertically, or diagonally.
  - The board is full (draw).

### Win Detection
- A helper function `checkSequence()` verifies consecutive four-in-a-row patterns in any direction.
- `checkWin()` checks horizontally, vertically, and diagonally using this helper.
- `checkDraw()` confirms if the top row is full with no winner.

---

## Testing Strategy

### Unit Testing
- **Board tests**: verified initialization and printing of an empty board.  
- **Gameplay tests**: verified correct placement of checkers, invalid move handling, and alternating turns.  
- **Win/draw tests**: tested scenarios for horizontal, vertical, and diagonal wins, as well as full-board draws.

### Debugging Tools
- **GDB**: used to trace program execution, check function calls, and resolve segmentation faults.  
- **Valgrind**: confirmed that the program runs without memory leaks or invalid memory access.

---

## VM Integration
- The project was compiled and executed inside an Alpine Linux virtual machine.  
- A **Makefile** automates compilation (`make build`), execution (`make run`), and cleanup (`make clean`).  
- The VM was configured to auto-launch the game on startup by adding the executable run command to the system init script.

---

## Work Distribution
- **Andy**: Implemented board representation and display (`board.c`).  
- **Jiany**: Implemented gameplay logic including input handling, win/draw detection (`game.c`).  
- **Jad**: Set up the Makefile, VM integration, and performed testing with GDB/Valgrind.  

---

## Deliverables Completed
- Two-player Connect Four game working in console.  
- Proper code structure with modular files (`board.c`, `game.c`, `main.c`).  
- Testing strategy implemented and validated with GDB/Valgrind.  
- VM integration completed with auto-launch on boot.  
- GitHub repository maintained with meaningful commits.  

---

## Next Steps
- Sprint 2 will focus on implementing a simple bot opponent that plays against a human player.  
- The bot will initially use random valid moves, with potential improvements in strategy in later sprints.
