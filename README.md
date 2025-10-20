# Connect 4 in C

## About
This project is a **C implementation of the classic Connect 4 game**, developed as part of a group project for the **Systems Programming course** at the Faculty of Arts & Sciences, Department of Computer Science.

The game runs entirely in the console on a Linux virtual machine.  
Two players (or one player versus a bot) take turns dropping their checkers (`A` and `B`) into a 7x6 grid.  
The first player to connect four in a row horizontally, vertically, or diagonally wins.

---

## Features
- Console-based 7x6 Connect 4 board
- Two-player (PvP) and Player-vs-Bot (Easy) gameplay
- Randomized bot logic that plays valid moves only
- Win and draw detection
- Fully compatible with Alpine Linux auto-boot configuration

---

## Getting Started

### Prerequisites
- A Linux distribution (TinyCore or Alpine recommended)
- GCC compiler
- Make build tool

### Build and Run
Clone the repository:
```bash
git clone https://github.com/Jiany-S/connect_c4.git
cd connect_c4
```

Build the game:
```bash
make debug
```

Run the game:
```bash
make run
```

Run automated tests:
```bash
make test
```

Clean build files:
```bash
make clean
```

---

## Repository Structure
```
connect_c4/
│
├── src/                 # Source files
│   ├── board.c          # Board initialization and printing
│   ├── board.h
│   ├── game.c           # Gameplay logic (dropChecker, checkWin, checkDraw, playGame)
│   ├── game.h
│   ├── bot.c            # Easy bot logic (random valid column selection)
│   ├── bot.h
│   └── main.c           # Entry point (game loop and mode selection)
│
├── tests/               # Unit tests and runners
│   ├── test_board.c
│   ├── test_game.c
│   └── test_runner.sh
│
├── docs/                # Documentation and sprint reports
│   ├── sprint1_report.md
│   ├── sprint2_report.md
│   ├── testing_notes.md
│   ├── bot_strategy.md
│   └── alpine_report.md
│
├── build/               # Compiled binaries (ignored in Git)
├── Makefile             # Build, run, test, valgrind, and debug automation
├── .gitignore           # Ignore build artifacts and binaries
└── README.md            # Project overview (this file)
```

---

## Testing & Debugging
The project was verified through:
- Automated unit tests (`test_board.c`, `test_game.c`)
- Interactive gameplay validation in both PvP and PvE modes
- **GDB debugging** for runtime behavior and control flow inspection
- **Valgrind** analysis confirming 0 memory leaks or invalid accesses

Example commands:
```bash
make debug
make gdb-run
make valgrind
```

All tools reported stable, leak-free execution.

---

## Sprint Progress
### Sprint 1
- Implemented core gameplay: PvP, win/draw detection
- Integrated Alpine VM auto-boot using `/etc/inittab`
- Verified execution directly on VM startup

### Sprint 2
- Added Player-vs-Bot (Easy) mode
- Implemented random valid bot logic in `bot.c`
- Added full GDB and Valgrind testing cycle
- Updated Makefile, test files, and documentation

---

## Demonstration
The project successfully runs on **Alpine Linux**, automatically launching on boot through `/etc/inittab` configuration:
```
tty1::respawn:/root/connect_c4/build/connect4
```
Screenshots of boot and auto-start behavior are included in `docs/screenshots/`.

---

## Team
- **Jiany Samara**
- **Jad Mouawad**
- **Andy Sleiman**

---

## License
This project was created for educational use as part of a university course.  
Not intended for commercial redistribution.

GitHub Repository: [https://github.com/Jiany-S/connect_c4](https://github.com/Jiany-S/connect_c4)