// src/bot_hard.h
#ifndef BOT_HARD_H
#define BOT_HARD_H

#include "game.h"

#ifdef __cplusplus
extern "C" {
#endif

// Toggle multithreading: 1 = use threads, 0 = single-threaded
#define USE_THREADS 1

#if USE_THREADS
#include <pthread.h>
#endif

// Main function to get Hard Bot move (default, single-threaded or threaded based on MT_BUILD)
int getHardMove(char board[ROWS][COLS], char player);

// Function to get Hard Bot move explicitly using threads or not
int getHardMoveWithThreads(char board[ROWS][COLS], char player, int useThreads);

#ifdef __cplusplus
}
#endif

#endif // BOT_HARD_H
