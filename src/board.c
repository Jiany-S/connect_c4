#include <stdio.h>
#include "board.h"

void initBoard(Board board) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = EMPTY; // initializes every cell to be empty
        }
    }
}

void printBoard(const Board board) {
    for (int i = 0; i < ROWS; i++) {
        printf("+");
        for (int j = 0; j < COLS; j++) {
            if (i == 0) {
                printf("   +"); // the board is open from the top
            }else {
                printf("---+");
            }
        }
        printf("\n|");
        for (int j = 0; j < COLS; j++) {
            printf(" %c |", board[i][j]); // prints the current state of each cell
        }
        printf("\n");
    }

    printf("+---+---+---+---+---+---+---+");
}