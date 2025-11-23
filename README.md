# Connect 4 in C — Enhanced Edition

## 🎮 About
This project is a **C implementation of Connect 4**, developed for the *Systems Programming* course at the Faculty of Arts & Sciences, Department of Computer Science.

The game runs entirely in a terminal and supports:
- Two-player mode  
- Player vs **Easy Bot**  
- Player vs **Medium Bot**  
- Player vs **Hard Bot** (**Sprint 4**) — featuring **Minimax + Alpha-Beta pruning + Multithreading**
- **Network Multiplayer** (**Sprint 4**) — play remotely via IP connection

---

## ✨ Features
- 7×6 console board  
- PvP and PvE  
- **Easy Bot** → random moves  
- **Medium Bot** → heuristic scoring  
- **Hard Bot (NEW)**  
  - Minimax search  
  - Alpha-beta pruning  
  - **Multithreading on top-level moves**  
- **Network Multiplayer (NEW)**
  - Play remotely via IP connection (Linux only)
  - Simple setup - remote player just runs one command
  - Real-time board synchronization
  - Play against another player or watch them play against a bot
- Win/draw detection  
- Build, test, debug, and Valgrind support

---

## ⚙️ Getting Started

### Prerequisites
- **Linux** (the network feature only works on Linux)
- GCC  
- Make

### Build & Run

Clone the repository:
```bash
git clone https://github.com/Jiany-S/connect_c4.git
cd connect_c4
```

Build:
```bash
make
```

Run:
```bash
./build/connect4
```

Run tests:
```bash
make test
```

Valgrind:
```bash
make valgrind
```

---

## 📁 Repository Structure
```
connect_c4/
│
├── docs/               
│   ├── Sprint 1/       # Core game development
│   ├── Sprint 2/       # Easy bot implementation
│   ├── Sprint 3/       # Medium bot implementation
│   └── Sprint 4/       # Hard bot + Network feature
│       ├── README.md
│       ├── bot_hard_strategy.md
│       ├── final_sprint_report.md
│       ├── NETWORK.md
│       ├── NETWORK_ARCHITECTURE.md
│       ├── NETWORK_IMPLEMENTATION.md
│       └── testing_notes.md
│
├── src/
│   ├── board.c / board.h
│   ├── bot_easy.c / bot_easy.h
│   ├── bot_medium.c / bot_medium.h
│   ├── bot_hard.c / bot_hard.h
│   ├── network.c / network.h
│   ├── client.c
│   ├── game.c / game.h
│   └── main.c
│
├── tests/
│   ├── test_board.c
│   ├── test_game.c
│   ├── test_bot_medium.c
│   └── test_bot_hard.c
│
├── scripts/
│   ├── install-deps.sh
│   └── run-valgrind-all.sh
│
├── Makefile
├── LICENSE
├── README.md
├── NETWORK_QUICKSTART.md
└── .gitignore
```

---

## 🌐 Network Multiplayer (Linux Only)
Play Connect 4 remotely with friends over a network connection!

**See [NETWORK_QUICKSTART.md](NETWORK_QUICKSTART.md) for detailed setup instructions.**

For in-depth technical documentation, see:
- [Network Architecture](docs/Sprint%204/NETWORK_ARCHITECTURE.md)
- [Network Implementation](docs/Sprint%204/NETWORK_IMPLEMENTATION.md)

### Quick Start
**Terminal 1 (Host):**
```bash
./build/connect4
# Select mode 5 (Player vs Player) or 6-8 (vs Bot)
# The server will display the IP address and command to run
```

**Terminal 2 (Remote Player):**
```bash
./build/connect4_client 192.168.1.100 8888
# Replace with the IP address shown by the host
# That's it! The game will appear in your terminal
```

### Features
- **Simple Setup**: Remote player only needs to run one command - no game installation required
- **Real-time Updates**: Board updates automatically after each move
- **Player vs Player** or **Player vs Bot**: Host can play against a remote player or let them play against a bot
- **Linux Only**: Uses standard Unix sockets (no Windows support)

### Available Network Modes
- **Mode 5**: Network Player vs Player (Host)
- **Mode 6**: Network vs Bot Easy (Host)
- **Mode 7**: Network vs Bot Medium (Host)
- **Mode 8**: Network vs Bot Hard (Host)

The remote player uses the simple `connect4_client` program which only needs the host's IP address.

---

## 🧠 Hard Bot — Sprint 4 Overview
The new **Hard Bot** introduces an advanced competitive AI model:

### ✔️ **Minimax Algorithm**
Evaluates future board states several moves ahead.

### ✔️ **Alpha-Beta Pruning**
Reduces the number of evaluated nodes dramatically.

### ✔️ **Multithreading (NEW)**
The top-level branching factor of Connect 4 is at most **7**.

We create one thread for each possible first move.  
Each thread:
- Copies the board  
- Applies the move  
- Runs Minimax  
- Returns the score  

This makes the bot:
- Faster  
- Capable of deeper searches  
- More competitive  

---

## 🧪 Testing & Debugging
Validated through:
- Unit tests  
- Manual gameplay  
- GDB debugging  
- Full Valgrind checks  

---

## 🚀 Sprint Progress Summary

### **Sprint 1**
- Core game engine  
- PvP  
- Win/draw logic  

### **Sprint 2**
- Easy Bot  
- GDB / Valgrind  

### **Sprint 3**
- Medium Bot  
- Tests  
- Scripts  

### **Sprint 4**
- Hard Bot (Minimax + Alpha-Beta)  
- Multithreading  
- Network Multiplayer  

---

## 👥 Team
- **Jiany Samara**  
- **Jad Mouawad**  
- **Andy Sleiman**

---

## ⚖️ License
For educational use.

**GitHub Repository:** https://github.com/Jiany-S/connect_c4
