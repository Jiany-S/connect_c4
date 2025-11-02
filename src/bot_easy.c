#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bot_easy.h"
#include "board.h"

int getEasyBotMove(char board[ROWS][COLS]) {
    int col;
    do {
        col = rand() % COLS;
    } while (board[0][col] != EMPTY);

    return col;
}
