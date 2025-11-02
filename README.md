# Connect 4 in C

## 🎮 About
This project is a **C implementation of the classic Connect 4 game**, developed as part of a group project for the **Systems Programming course** at the Faculty of Arts & Sciences, Department of Computer Science.

The game runs entirely in the console on a **Linux virtual machine**.  
Two players — or one player versus a **bot (Easy or Medium)** — take turns dropping their checkers (`A` and `B`) into a 7×6 grid.  
The first player to connect four checkers horizontally, vertically, or diagonally wins.

---

## ✨ Features
- Console-based 7×6 Connect 4 board  
- **Two-player (PvP)** and **Player-vs-Bot** modes  
- **Easy Bot:** plays random valid moves  
- **Medium Bot:** plays with simple heuristics (prioritizes wins & blocks)  
- Win and draw detection  
- Fully compatible with Alpine or TinyCore Linux  
- Optional scripts for dependency installation and Valgrind testing  

---

## ⚙️ Getting Started

### Prerequisites
- A Linux distribution (Alpine or TinyCore recommended)
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

Run full Valgrind memory analysis:
```bash
make valgrind
```

Or run all Valgrind checks through:
```bash
scripts/run-valgrind-all.sh
```

---

## 📁 Repository Structure
```
connect_c4/
│
├── docs/               # Sprint documentation (Sprints 1, 2, and 3)
│
├── scripts/            # Utility scripts
│   ├── install-deps.sh
│   └── run-valgrind-all.sh
│
├── src/                # Source files
│   ├── board.c / board.h
│   ├── bot_easy.c / bot_easy.h
│   ├── bot_medium.c / bot_medium.h
│   ├── game.c / game.h
│   └── main.c
│
├── tests/              # Unit tests and runner scripts
│   ├── test_board.c
│   ├── test_game.c
│   ├── test_bot_medium.c
│   └── test_runner.sh
│
├── build/              # Compiled binaries (ignored in Git)
├── Makefile            # Build, run, debug, and test automation
├── LICENSE             # Project license
├── valgrind.log        # Valgrind report (auto-generated)
├── .gitignore
└── README.md
```

---

## 🧪 Testing & Debugging
The project was validated through:
- Automated unit tests (`test_board.c`, `test_game.c`, `test_bot_medium.c`)
- Interactive PvP and PvE gameplay
- **GDB debugging** for runtime flow inspection
- **Valgrind** confirming 0 memory leaks or invalid reads/writes

Example commands:
```bash
make debug
make gdb-run
make valgrind
```

All tests passed successfully on Alpine Linux.

---

## 🚀 Sprint Progress
### Sprint 1
- Implemented core gameplay (PvP, win/draw detection)
- Integrated Alpine VM auto-boot via `/etc/inittab`
- Verified execution directly on boot

### Sprint 2
- Added Player-vs-Bot (Easy) mode
- Implemented random move generation logic
- Added GDB and Valgrind test cycles

### Sprint 3
- Added Player-vs-Bot (Medium) mode with heuristic logic
- Added dedicated unit test: `test_bot_medium.c`
- Introduced new `scripts/` folder for automation tools
- Updated documentation and test coverage

---

## 🖥️ Demonstration
The project successfully runs on **Alpine Linux**, automatically launching on boot through:
```
tty1::respawn:/root/connect_c4/build/connect4
```

Screenshots and documentation for each sprint are located in the `docs/` folder.

---

## 👥 Team
- **Jiany Samara**  
- **Jad Mouawad**  
- **Andy Sleiman**

---

## ⚖️ License
This project was created for educational use as part of a university course.  
Not intended for commercial redistribution.

**GitHub Repository:** [https://github.com/Jiany-S/connect_c4](https://github.com/Jiany-S/connect_c4)
