#ifndef GAME_H
#define GAME_H

#include "board.h"

#define SWITCH_PLAYER(p) ((p) == PLAYER_A ? PLAYER_B : PLAYER_A)
#define VALID_COLUMN(c) ((c) >= 0 && (c) < COLS)
#define COLUMN_FULL(b, c) ((b)[0][(c)] != EMPTY)

// Updated to include startingPlayer
void playGame(int mode, char startingPlayer);

int dropChecker(char board[ROWS][COLS], int column, char player);
int checkWin(char board[ROWS][COLS], char player);
int checkDraw(char board[ROWS][COLS]);

#endif
