#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"
#include "game.h"
#include "bot.h"

/* Read an integer from stdin.
 * Returns:
 *   >=1 : the integer read (user-facing column, 1..COLS)
 *    0  : input line did not contain a number (invalid)
 *   -1  : EOF reached (caller should treat as quit)
 */
static int read_user_column(void) {
    char buf[64];
    if (fgets(buf, sizeof buf, stdin) == NULL) return -1; // EOF
    // trim leading spaces
    char *p = buf;
    while (*p == ' ' || *p == '\t') ++p;
    if (*p == '\n' || *p == '\0') return 0;
    char *endptr;
    long val = strtol(p, &endptr, 10);
    if (endptr == p) return 0; // no number found
    return (int) val;
}

void playGame(int mode) {
    Board board;
    initBoard(board);
    char current = PLAYER_A;
    int move_count = 0;

    while (1) {
        printBoard(board);

        int column;
        int input;
        
        if (mode == 2 && current == PLAYER_B) {
            // Bot's turn (automatic)

            column = getEasyBotMove(board);
            printf("Bot plays column %d", column+1);
        } else {
            // Human player's turn

            printf("Player %c's turn. Enter column (1-%d): ", current, COLS);
            fflush(stdout);

            
        

            input = read_user_column();
            if (input == -1) {
                // EOF (e.g., file ended when using redirected tests, or user Ctrl+D)
                printf("\nEnd of input. Exiting game.\n");
                return;
            }
            if (input == 0) {
                printf("Invalid input. Please type a column number 1-%d and press Enter.\n", COLS);
                continue;
            }

            column = input - 1; // convert to 0-based for dropChecker
        }
        int row = dropChecker(board, column, current);
        if (row == -1) {
            // dropChecker returns -1 for invalid column or full column
            if (!VALID_COLUMN(column)) {
                printf("Column %d is out of range. Choose 1-%d.\n", input, COLS);
            } else {
                printf("Column %d is full or move invalid. Try another column.\n", input);
            }
            continue;
        }

        move_count++;

        if (checkWin(board, current)) {
            printBoard(board);
            printf("Player %c wins!\n", current);
            return;
        }

        if (checkDraw(board)) {
            printBoard(board);
            printf("The game is a draw.\n");
            return;
        }

        // switch players and continue
        current = SWITCH_PLAYER(current);
    }
}

int main(void) {
    int mode;
    srand(time(NULL)); // randomize but moves once per run

    printf("Select Mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Bot (easy)\n");
    if (scanf("%d", &mode) != 1) {
        fprintf(stderr, "Invalid input for mode.\n");
        return 1;
    }
    /* Flush leftover newline */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }

    
    playGame(mode);
    return 0;
}
