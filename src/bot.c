#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bot.h"
#include "board.h"

int getEasyBotMove(char board[ROWS][COLS]) {
    int col;
    do {
        col = rand() % COLS;
    } while (board[0][col] != ' ');

    printf("Bot plays column %d\n", col + 1);
    return col;
}
