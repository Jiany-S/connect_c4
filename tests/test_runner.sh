#!/bin/sh
set -e
mkdir -p build

echo "🔹 Building unit tests..."
gcc -Wall -Wextra -std=gnu11 -g -o build/test_board tests/test_board.c src/board.c
gcc -Wall -Wextra -std=gnu11 -g -o build/test_game tests/test_game.c src/board.c src/game.c

echo "🔹 Running unit tests..."
./build/test_board
./build/test_game

echo "✅ All unit tests passed."
EOF
