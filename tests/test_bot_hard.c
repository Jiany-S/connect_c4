// tests/test_bot_hard.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "bot_hard.h"
#include "game.h"

#define NUM_BOARDS 14

typedef struct {
    Board board;
    char player;
    int move;
    double elapsed;
} BoardThreadArg;

void initializeBoards(Board boards[NUM_BOARDS]) {
    // Randomly populate boards with some moves
    for (int b = 0; b < NUM_BOARDS; ++b) {
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < COLS; ++j)
                boards[b][i][j] = EMPTY;

        // Random initial moves (optional, can adjust for testing)
        int moves = rand() % 5 + 1; // 1-5 moves
        for (int m = 0; m < moves; ++m) {
            int col = rand() % COLS;
            char player = (m % 2 == 0) ? 'A' : 'B';
            dropChecker(boards[b], col, player);
        }
    }
}

void* botThreadFunc(void* arg_ptr) {
    BoardThreadArg* arg = (BoardThreadArg*)arg_ptr;
    clock_t start = clock();
    arg->move = getHardMove(arg->board, arg->player);
    clock_t end = clock();
    arg->elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    return NULL;
}

int main() {
    srand(time(NULL));

    printf("=== Hard Bot Multithreaded vs Single-threaded Test ===\n\n");

    Board boards[NUM_BOARDS];
    initializeBoards(boards);

    BoardThreadArg mtArgs[NUM_BOARDS];
    BoardThreadArg stArgs[NUM_BOARDS];

    pthread_t threads[NUM_BOARDS];

    // --- MULTITHREADED: one thread per board ---
    for (int i = 0; i < NUM_BOARDS; ++i) {
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                mtArgs[i].board[r][c] = boards[i][r][c];
        mtArgs[i].player = 'A';
        pthread_create(&threads[i], NULL, botThreadFunc, &mtArgs[i]);
    }

    for (int i = 0; i < NUM_BOARDS; ++i)
        pthread_join(threads[i], NULL);

    double totalMT = 0;
    for (int i = 0; i < NUM_BOARDS; ++i) {
        printf("[MT] Board %d Bot chose column %d in %.6f s\n", i + 1, mtArgs[i].move, mtArgs[i].elapsed);
        totalMT += mtArgs[i].elapsed;
    }

    printf("\n");

    // --- SINGLE-THREADED: sequential processing ---
    clock_t stStart = clock();
    for (int i = 0; i < NUM_BOARDS; ++i) {
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
                stArgs[i].board[r][c] = boards[i][r][c];
        stArgs[i].player = 'A';

        clock_t start = clock();
        stArgs[i].move = getHardMove(stArgs[i].board, stArgs[i].player);
        clock_t end = clock();
        stArgs[i].elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    }
    clock_t stEnd = clock();

    double totalST = 0;
    for (int i = 0; i < NUM_BOARDS; ++i) {
        printf("[ST] Board %d Bot chose column %d in %.6f s\n", i + 1, stArgs[i].move, stArgs[i].elapsed);
        totalST += stArgs[i].elapsed;
    }

    printf("\nMultithreaded total time: %.6f s\n", totalMT);
    printf("Single-threaded total time: %.6f s\n", totalST);

    return 0;
}
