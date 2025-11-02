#include <string.h>
#include "bot_medium.h"
#include "game.h"

static int wouldWin(char board[ROWS][COLS], int column, char player) {
    if (column < 0 || column >= COLS) return 0;
    if (board[0][column] != EMPTY) return 0;

    Board simulated;
    memcpy(simulated, board, sizeof(Board));
    dropChecker(simulated, column, player);
    return checkWin(simulated, player);
}

int getMediumMove(char board[ROWS][COLS], char player) {
    const int preferredColumns[] = {3, 4, 2, 5, 1, 6, 0};
    char opponent = SWITCH_PLAYER(player);

    for (int col = 0; col < COLS; ++col) {
        if (wouldWin(board, col, player)) {
            return col;
        }
    }

    for (int col = 0; col < COLS; ++col) {
        if (wouldWin(board, col, opponent)) {
            return col;
        }
    }

    for (int i = 0; i < (int)(sizeof(preferredColumns) / sizeof(preferredColumns[0])); ++i) {
        int col = preferredColumns[i];
        if (board[0][col] == EMPTY) {
            return col;
        }
    }

    for (int col = 0; col < COLS; ++col) {
        if (board[0][col] == EMPTY) {
            return col;
        }
    }

    return -1;
}
