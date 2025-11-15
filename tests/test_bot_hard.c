#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bot_hard.h"

#define NUM_BOARDS 10

void copyBoard(char dest[ROWS][COLS], char src[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            dest[r][c] = src[r][c];
}

int main() {
    printf("=== Hard Bot Multithreaded vs Single-threaded Test ===\n");

    // Example: random boards for testing
    char boards[NUM_BOARDS][ROWS][COLS];
    for (int b = 0; b < NUM_BOARDS; b++)
        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                boards[b][r][c] = (rand() % 3 == 0) ? 'A' : ((rand() % 2 == 0) ? 'B' : '.');

    double mtTotal = 0.0, stTotal = 0.0;

    for (int b = 0; b < NUM_BOARDS; b++) {
        char boardCopy[ROWS][COLS];
        copyBoard(boardCopy, boards[b]);

        clock_t mtStart = clock();
        int mtMove = getHardMoveWithThreads(boardCopy, 'A', 1); // MT
        clock_t mtEnd = clock();
        double mtTime = (double)(mtEnd - mtStart) / CLOCKS_PER_SEC;
        mtTotal += mtTime;
        printf("[MT] Board %d Bot chose column %d in %.6f s\n", b + 1, mtMove, mtTime);

        copyBoard(boardCopy, boards[b]);
        clock_t stStart = clock();
        int stMove = getHardMoveWithThreads(boardCopy, 'A', 0); // ST
        clock_t stEnd = clock();
        double stTime = (double)(stEnd - stStart) / CLOCKS_PER_SEC;
        stTotal += stTime;
        printf("[ST] Board %d Bot chose column %d in %.6f s\n", b + 1, stMove, stTime);
    }

    printf("\nMultithreaded total time: %.6f s\n", mtTotal);
    printf("Single-threaded total time: %.6f s\n", stTotal);

    return 0;
}
