#include <stdio.h>
#include "game.h"

static int checkSequence(char board[ROWS][COLS], char player, int startRow, int startCol, int dRow, int dCol){
    for (int i = 0; i < 4; ++i) {
        int row = startRow + i * dRow;
        int col = startCol + i * dCol;
        if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
            return 0;
        }
        if (board[row][col] != player) {
            return 0;
        }
    }
    return 1;
}

int dropChecker(char board[ROWS][COLS], int column, char player){
    if (!VALID_COLUMN(column) || COLUMN_FULL(board, column)) {
        return -1;
    }
    for (int row = ROWS - 1; row >= 0; --row) {
        if (board[row][column] == EMPTY) {
            board[row][column] = player;
            return row;
        }
    }
    return -1;
}

int checkWin(char board[ROWS][COLS], char player){
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (board[row][col] != player) {
                continue;
            }
            if (col <= COLS - 4 && checkSequence(board, player, row, col, 0, 1)) {
                return 1;
            }
            if (row <= ROWS - 4 && checkSequence(board, player, row, col, 1, 0)) {
                return 1;
            }
            if (row <= ROWS - 4 && col <= COLS - 4 && checkSequence(board, player, row, col, 1, 1)) {
                return 1;
            }
            if (row >= 3 && col <= COLS - 4 && checkSequence(board, player, row, col, -1, 1)) {
                return 1;
            }
        }
    }
    return 0;
}

int checkDraw(char board[ROWS][COLS]){
    for (int col = 0; col < COLS; ++col) {
        if (board[0][col] == EMPTY) {
            return 0;
        }
    }
    return 1;
}
