// src/board.c
#include "board.h"
#include <stdio.h>

void initBoard(Board board) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            board[r][c] = EMPTY;
        }
    }
}

void printBoard(const Board board) {
    // Print column numbers
    printf("\n");
    for (int c = 0; c < COLS; c++) {
        printf(" %d", c + 1);
    }
    printf("\n");

    // Print rows
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            printf(" %c", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}
