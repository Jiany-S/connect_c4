# Connect 4 in C

## About
This project is a simple implementation of the classic **Connect 4** game written in C.  
It was developed as part of a group project for the Systems Programming course at the Faculty of Arts & Sciences, Department of Computer Science.

The game runs in the console on a Linux virtual machine. Two players take turns dropping their checkers (`A` and `B`) into a 7x6 grid.  
The first player to connect four in a row horizontally, vertically, or diagonally wins.

---

## Features
- Console-based 7x6 Connect 4 board
- Two-player turn-based gameplay
- Real-time board updates after each move
- Win and draw detection

---

## Getting Started

### Prerequisites
- A Linux distribution (TinyCore or Alpine recommended)
- GCC compiler
- Make

### Build and Run
Clone the repository:
```bash
git clone https://github.com/Jiany-S/connect_c4.git
cd connect_c4
```

Build the game:
```bash
make build
```

Run the game:
```bash
make run
```

Clean build files:
```bash
make clean
```

---

## Repository Structure
```
connect4-c/
│
├── src/                 # Source files
│   ├── board.c          # Board initialization and printing
│   ├── board.h
│   ├── game.c           # Gameplay logic (dropChecker, checkWin, checkDraw)
│   ├── game.h
│   └── main.c           # Entry point (game loop integration)
│
├── tests/               # Test cases and drivers
│   ├── test_board.c
│   ├── test_game.c
│   └── test_runner.sh
│
├── docs/                # Documentation for each sprint
│   ├── sprint1_report.md
│   └── testing_notes.md
│
├── build/               # Compiled output (ignored in git)
├── Makefile             # Build and run automation
├── .gitignore           # Ignore build artifacts
└── README.md            # Project documentation (this file)
```

---

## Compilation & Execution
Make sure you have **GCC** installed in your Linux VM.

```bash
# Build the project
make build

# Run the game
make run

# Clean build files
make clean
```

When the VM boots, the game is configured to launch automatically.

---

## Testing & Debugging
The project was tested with a combination of unit test files and manual playthroughs.  
Debugging was done using **GDB**, and **Valgrind** was used to check for memory errors and leaks.

---

## Sprint Deliverables
- Fully working **two-player console Connect 4**  
- Runs on Linux VM without GUI  
- Auto-launches on VM boot  
- GitHub repo with structured commits  
- Testing strategy documented with GDB and Valgrind  

---

## Team
- Jiany Samara  
- Jad Mouawad 
- Andy Sleiman

---

## License
This project is for educational purposes as part of a university course.  
Not intended for commercial use.
