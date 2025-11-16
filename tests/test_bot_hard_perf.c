// tests/test_bot_hard_perf.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bot_hard.h"
#include "game.h"

#define NUM_TESTS 50
#define DEPTH_TEST 8  // increase depth to make MT worthwhile

// Generate a random partially filled board
void randomBoard(char board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = EMPTY;

    // Drop random moves
    int numMoves = rand() % (ROWS * COLS / 2);
    for (int i = 0; i < numMoves; i++) {
        int col = rand() % COLS;
        char player = (rand() % 2) ? 'A' : 'B';
        dropChecker(board, col, player);
    }
}

int main() {
    srand(time(NULL));
    clock_t start, end;
    double totalMT = 0.0, totalST = 0.0;

    for (int t = 0; t < NUM_TESTS; t++) {
        char board[ROWS][COLS];
        randomBoard(board);

        start = clock();
        int mtMove = getHardMoveWithThreads(board, 'A', 1); // MT
        end = clock();
        totalMT += (double)(end - start) / CLOCKS_PER_SEC;

        start = clock();
        int stMove = getHardMoveWithThreads(board, 'A', 0); // ST
        end = clock();
        totalST += (double)(end - start) / CLOCKS_PER_SEC;

        printf("Test %2d: MT=%d  ST=%d\n", t+1, mtMove, stMove);
    }

    printf("\n--- Performance Summary ---\n");
    printf("Multithreaded total time: %.3f s\n", totalMT);
    printf("Single-threaded total time: %.3f s\n", totalST);
    printf("MT/ ST ratio: %.2f\n", totalMT / totalST);

    return 0;
}
