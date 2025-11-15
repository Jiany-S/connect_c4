// src/test_bot_hard.c
#include <stdio.h>
#include <time.h>
#include "bot_hard.h"
#include "game.h"

#define NUM_TEST_BOARDS 3

// Example mid-game boards
char testBoards[NUM_TEST_BOARDS][ROWS][COLS] = {
    {
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, 'A', EMPTY, EMPTY, EMPTY},
        {'B', 'B', 'A', 'A', 'B', EMPTY, EMPTY}
    },
    {
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, 'A', EMPTY, EMPTY, EMPTY},
        {EMPTY, 'B', 'A', 'B', EMPTY, EMPTY, EMPTY},
        {EMPTY, 'A', 'B', 'A', EMPTY, EMPTY, EMPTY},
        {'B', 'B', 'A', 'A', 'B', EMPTY, EMPTY},
        {'A', 'A', 'B', 'B', 'A', EMPTY, EMPTY}
    },
    {
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, 'B', EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, 'A', 'A', 'B', EMPTY, EMPTY, EMPTY},
        {'B', 'B', 'A', 'A', 'B', EMPTY, EMPTY},
        {'A', 'A', 'B', 'B', 'A', EMPTY, EMPTY},
        {'B', 'A', 'B', 'A', 'B', 'A', EMPTY}
    }
};

void printMoveTime(int move, double duration) {
    printf("Bot chose column %d in %.6f seconds\n", move, duration);
}

int main() {
    printf("=== Hard Bot Multithreaded vs Single-threaded Test ===\n");

    for (int b = 0; b < NUM_TEST_BOARDS; ++b) {
        printf("\n--- Board %d ---\n", b + 1);
        char (*board)[COLS] = testBoards[b];

        // Multithreaded version
#ifdef MT_BUILD
        printf("[MT] ");
        clock_t startMT = clock();
        int moveMT = getHardMove(board, 'A');
        clock_t endMT = clock();
        printMoveTime(moveMT, (double)(endMT - startMT) / CLOCKS_PER_SEC);
#endif

        // Single-threaded version
#undef MT_BUILD
        printf("[ST] ");
        clock_t startST = clock();
        int moveST = getHardMove(board, 'A');
        clock_t endST = clock();
        printMoveTime(moveST, (double)(endST - startST) / CLOCKS_PER_SEC);
    }

    return 0;
}
