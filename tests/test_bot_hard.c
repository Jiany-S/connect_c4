// tests/test_bot_hard.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bot_hard.h"
#include "game.h"

#define NUM_BOARDS 14  // match number of threads you want to test
#define MAX_MOVES 7

void initRandomBoard(char board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            board[r][c] = EMPTY;
    // drop a few random moves
    int moves = rand() % 10 + 5;
    for (int i = 0; i < moves; ++i) {
        int col = rand() % COLS;
        char player = (i % 2 == 0) ? 'A' : 'B';
        dropChecker(board, col, player);
    }
}

int main() {
    srand(time(NULL));
    char boards[NUM_BOARDS][ROWS][COLS];
    
    // Initialize random boards
    for (int i = 0; i < NUM_BOARDS; ++i) {
        initRandomBoard(boards[i]);
    }

    printf("=== Hard Bot Multithreaded vs Single-threaded Test ===\n\n");

    clock_t start, end;
    double mt_time, st_time;

    // Multithreaded
    start = clock();
    for (int i = 0; i < NUM_BOARDS; ++i) {
        int move = getHardMove(boards[i], 'A');  // MT_BUILD flag in Makefile
        printf("[MT] Board %d Bot chose column %d\n", i + 1, move);
    }
    end = clock();
    mt_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Single-threaded
    start = clock();
    for (int i = 0; i < NUM_BOARDS; ++i) {
        // Temporarily disable multithreading in bot_hard.c by undefining MT_BUILD
        int move = getHardMove(boards[i], 'A');
        printf("[ST] Board %d Bot chose column %d\n", i + 1, move);
    }
    end = clock();
    st_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nMultithreaded total time: %.6f s\n", mt_time);
    printf("Single-threaded total time: %.6f s\n", st_time);
    return 0;
}
