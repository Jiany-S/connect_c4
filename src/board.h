#ifndef BOARD_H
#define BOARD_H

#define ROWS 6
#define COLS 7

#define PLAYER_A 'A'
#define PLAYER_B 'B'
#define EMPTY    '.'

typedef char Board[ROWS][COLS];

void initBoard(Board board);
void printBoard(const Board board);

#endif
