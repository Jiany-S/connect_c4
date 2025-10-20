# Alpine Linux Auto-Boot Setup Report

## Objective
Configure the Alpine Linux VM to automatically launch the Connect 4 program at startup.

## Steps Followed
1. Installed Alpine Linux in VirtualBox using the official ISO.
2. Mounted the project directory in `/root/connect_c4`.
3. Built the program inside the VM using:
   ```bash
   cd /root/connect_c4
   make
   ```
   This produced the executable at `/root/connect_c4/build/connect4`.
4. Edited the system initialization file:
   ```bash
   nano /etc/inittab
   ```
   Added the following line under the `# Set up a couple of getty's` section:
   ```
   tty1::respawn:/root/connect_c4/build/connect4
   ```
5. Saved the file and rebooted the VM:
   ```bash
   reboot
   ```
6. Upon startup, Alpine automatically launches the Connect 4 game.

## Verification
The following screenshots confirm successful setup:
- **alpine1.jpg** — shows Alpine booting up with OpenRC services and the game prompt appearing automatically.
- **alpine2.jpg** — displays the `/etc/inittab` with the Connect 4 startup command.

## Result
The Alpine VM now runs the Connect 4 game automatically at startup, fulfilling the project requirement to demonstrate execution on a minimal Linux distribution.