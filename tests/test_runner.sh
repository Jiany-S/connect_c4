#!/usr/bin/env bash
set -e
TARGET=$1
echo "Running unit tests..."

gcc -Wall -Wextra -std=gnu11 -g -o build/test_board tests/test_board.c src/board.c
./build/test_board

gcc -Wall -Wextra -std=gnu11 -g -o build/test_game tests/test_game.c src/board.c src/game.c
./build/test_game

echo "All tests PASSED"
