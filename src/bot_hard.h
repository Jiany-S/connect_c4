// src/bot_hard.h
#ifndef BOT_HARD_H
#define BOT_HARD_H

#include "game.h"

#ifdef __cplusplus
extern "C" {
#endif

// Toggle multithreading: 1 = use threads, 0 = single-threaded
#define USE_THREADS 1

#ifdef USE_THREADS
#define MT_BUILD
#endif

// Main function to get Hard Bot move
int getHardMove(char board[ROWS][COLS], char player);

#ifdef __cplusplus
}
#endif

#endif // BOT_HARD_H
