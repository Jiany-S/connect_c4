// src/main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "board.h"
#include "game.h"
#include "bot_easy.h"
#include "bot_medium.h"
#include "bot_hard.h"
#include "network.h"

/* If compiled with -DMT_BUILD and -pthread, bot_hard will evaluate top-level moves
 * in parallel using pthreads. Use `make multithread` to build that version.
 */

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

/* playGame now accepts a startingPlayer so caller can choose who begins. */
void playGame(int mode, char startingPlayer) {
    Board board;
    initBoard(board);
    char current = startingPlayer;
    int move_count = 0;

    while (1) {
        printBoard(board);

        int column = -1;
        int input = 0; // keep defined for error messages

        if ((mode == 2 || mode == 3 || mode == 4) && current == PLAYER_B) {
            // Bot's turn (automatic)
            if (mode == 2) {
                column = getEasyBotMove(board);
            } else if (mode == 3){
                column = getMediumMove(board, current);
            } else { // mode == 4
                column = getHardMove(board, current);
            }
            input = column + 1;
            printf("Bot plays column %d\n", input);
        } else {
            // Human player's turn
            printf("Player %c's turn. Enter column (1-%d): ", current, COLS);
            fflush(stdout);

            int read = read_user_column();
            if (read == -1) {
                // EOF (e.g., file ended when using redirected tests, or user Ctrl+D)
                printf("\nEnd of input. Exiting game.\n");
                return;
            }
            if (read == 0) {
                printf("Invalid input. Please type a column number 1-%d and press Enter.\n", COLS);
                continue;
            }

            input = read;
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
    srand(time(NULL));

    printf("Select Mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Bot (easy)\n");
    printf("3. Player vs Bot (medium)\n");
    printf("4. Player vs Bot (hard)\n");
    printf("5. Network Game (Host - Player vs Player)\n");
    printf("6. Network vs Bot (easy) - Host\n");
    printf("7. Network vs Bot (medium) - Host\n");
    printf("8. Network vs Bot (hard) - Host\n");
    
    if (scanf("%d", &mode) != 1) {
        fprintf(stderr, "Invalid input for mode.\n");
        return 1;
    }

    if (mode < 1 || mode > 8) {
        fprintf(stderr, "Invalid mode. Please choose between 1 and 8.\n");
        return 1;
    }
    
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }

    /* Ask who should start first - apply to both local and network modes.
     * For network server mode: Host is Player A, remote is Player B.
     * For local modes: Player A / Player B correspond to how your code prints them.
     */
    int starter_choice = 1; // default to Player A
    char startingPlayer = PLAYER_A;

    printf("\nWho should start first?\n");
    printf("1. Player A (default)\n");
    printf("2. Player B\n");
    printf("Enter choice (1-2): ");
    if (scanf("%d", &starter_choice) == 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        if (starter_choice == 2) startingPlayer = PLAYER_B;
        else startingPlayer = PLAYER_A;
    } else {
        // if invalid input, default to Player A
        while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
        startingPlayer = PLAYER_A;
    }

    // Handle network modes
    if (mode >= 5 && mode <= 8) {
        int sock;
        int port = 8888;
        
        sock = startServer(port);
        if (sock < 0) {
            fprintf(stderr, "Failed to start server\n");
            return 1;
        }
        
        // Determine bot difficulty
        int botMode = 1; // Player vs Player by default
        if (mode == 6) botMode = 2; // Easy bot
        else if (mode == 7) botMode = 3; // Medium bot
        else if (mode == 8) botMode = 4; // Hard bot
        
        // Pass startingPlayer to the network game
        playNetworkGame(botMode, sock, 1, startingPlayer);
        closeConnection(sock);
    } else {
        // Local game modes
        playGame(mode, startingPlayer);
    }
    
    return 0;
}
