# Network Multiplayer (Linux Only)

Remote players can join a Connect 4 match simply by opening a terminal and running the lightweight client binary—no full game install needed. The host runs the regular game, selects one of the network modes, and shares the printed IP/port. The system is purposely Linux-only so it can rely on standard BSD sockets without extra abstractions.

## Key Capabilities
- Remote Player vs Player or Player vs Bot matches (host can pair the remote user against Easy/Medium/Hard bots).
- Server-authoritative board logic: host validates all moves, evaluates wins, and broadcasts the canonical board.
- Lightweight client (~200 LOC) handles just I/O and networking, keeping distribution friction-free.
- Simple TCP protocol (port 8888 by default) with predictable message types for updates, move requests, responses, and game-over notices.


## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                     Network Multiplayer                      │
│                        (Linux Only)                          │
└─────────────────────────────────────────────────────────────┘

    Computer 1 (Host)              Computer 2 (Remote Player)
┌─────────────────────┐           ┌───────────────────────┐
│  ./build/connect4   │ TCP/IP    │  ./build/connect4_client │
│  (Game + Server)    │ Port 8888 │  (Client Only)           │
└─────────────────────┘           └───────────────────────┘
```

### Host Responsibilities (`build/connect4`)
- Full gameplay stack: board, win/draw detection, local/bot turns.
- Starts a TCP server on port 8888, prints IP/connection command, and blocks until a client connects.
- After each move (local, remote, or bot) the host sends the refreshed board state to the client.
- Validates every incoming move and rejects invalid requests before they affect the board.

### Client Responsibilities (`build/connect4_client`)
- Minimal binary (≈200 lines) with no embedded game logic.
- Connects via `./build/connect4_client <IP> 8888`, renders the latest board, and prompts for moves when instructed.
- Sends only the chosen column index back to the host.

## Message Protocol

```c
#define MSG_BOARD_UPDATE   1  // Server → Client: 6x7 board + active player
#define MSG_MOVE_REQUEST   2  // Server → Client: prompt for next move
#define MSG_MOVE_RESPONSE  3  // Client → Server: column selection (0-6)
#define MSG_GAME_OVER      4  // Server → Client: result + final board
```

**Board Update Payload:** message type, current player indicator, and 42 bytes representing the board row-by-row. **Move Response Payload:** message type followed by the column number the remote player selected. Server ignores malformed columns and asks again.

## Host & Client Flow

1. Host selects a network mode (5–8) and the server prints the IP/port plus a ready-made client command.
2. Remote player runs the printed command; once connected, the host acknowledges and broadcasts the initial empty board.
3. Turns alternate:
   - Local player or bot moves immediately within the host process.
   - Remote player waits for `MSG_MOVE_REQUEST`, enters a column (1–7), and the client sends `MSG_MOVE_RESPONSE`.
   - Host validates the move, applies it, and emits a `MSG_BOARD_UPDATE`.
4. When a win/draw occurs, host sends `MSG_GAME_OVER`, shows the result locally, and tears down the socket.

## Game Modes (Host Only)
- **Mode 5** – Network Player vs Player (host is Player A, remote is Player B).
- **Mode 6** – Host vs Easy Bot with remote observer/player controlling Player A.
- **Mode 7** – Host vs Medium Bot.
- **Mode 8** – Host vs Hard Bot.

> The previous "client joins and runs the entire game" flow was removed. The remote user only runs the client binary now, matching the requirement that they simply open a terminal.

## Building & Distribution

```bash
make clean
make            # builds host (build/connect4) and client (build/connect4_client)
make client     # optional: rebuild client alone
```

To share the client without exposing the full repo:
```bash
scp build/connect4_client user@remote:~/connect4_client
```

Remote user then runs `./build/connect4_client <host-ip> 8888` and is ready to play.

## Testing & Troubleshooting
- `make test` ensures board/game/bot logic stay intact; networking piggybacks on that validated core.
- Manual checklist:
  - Host boots server and prints IP/command.
  - Client connects and receives initial board.
  - Invalid columns (e.g., 10 or full column) are rejected server-side.
  - Board stays synchronized after every move.
  - Win/draw/endgame messages reach the client.
- Common issues:
  - **Connection refused** – Host not running or firewall blocking port 8888.
  - **No route** – Machines not on same network or missing port-forwarding for WAN play.
  - **Dropped link** – Network hiccup terminates the socket; restart the session.

## Security & Limitations
- Educational scope: no authentication or encryption, single client per host session, no resume support.
- Linux-only code path (Unix sockets); Windows-specific branches and Winsock dependencies were intentionally removed for simplicity.
- Host CPU usage grows when pairing the network mode with the multithreaded Hard Bot, but remains bounded by available cores.

## File Map
- `src/network.h / src/network.c` – Socket helpers, message serialization, IP display utilities.
- `src/client.c` – Standalone client entry point; depends only on libc and BSD sockets.
- `src/main.c` – Mode selection and integration with the rest of the game loop.
- `scripts/connect_remote.sh` – Optional helper to auto-build and launch the client.

