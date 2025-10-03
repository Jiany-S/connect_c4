# VM setup — how to run the Connect4 game in the VM

## Dependencies
Run:
sudo apt update
sudo apt install -y build-essential gdb valgrind git expect

## Build
From repo root:
make debug    # builds bin/connect4

## Run interactive
./bin/connect4

## Auto-run options
Option A (recommended for desktop VM):
Copy `autostart/connect4-autostart.desktop` to `~/.config/autostart/` and edit Exec path.

Option B (systemd; for advanced users):
Copy `autostart/connect4.service` to `/etc/systemd/system/`, update paths, then:
sudo systemctl daemon-reload
sudo systemctl enable connect4.service
sudo systemctl start connect4.service

Notes:
- The program is interactive; .desktop (autostart on login) is safer and simpler.
