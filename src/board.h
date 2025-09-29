#ifndef BOARD_H

#define BOARD_H
#define ROWS 6
#define COLS 7

typedef enum Cell {
    ., A, B
} Cell; // . is an empty cell, A is player 1, B is player 2

typedef Cell Board[ROWS][COLS];

void initBoard(Board board); // initalizes all cells to empty
void printBoard(Board board); // prints the current board

#endif
