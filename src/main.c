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

static int read_user_column(void) {
    char buf[64];
    if (fgets(buf, sizeof buf, stdin) == NULL) return -1;
    char *p = buf;
    while (*p == ' ' || *p == '\t') ++p;
    if (*p == '\n' || *p == '\0') return 0;
    char *endptr;
    long val = strtol(p, &endptr, 10);
    if (endptr == p) return 0;
    return (int) val;
}

void playGame(int mode, char startingPlayer) {
    Board board;
    initBoard(board);
    char current = startingPlayer;
    int move_count = 0;

    while (1) {
        printBoard(board);

        int column;
        int input = 0;

        // Bot turn?
        if ((mode == 2 || mode == 3 || mode == 4) && current == PLAYER_B) {
            if (mode == 2)
                column = getEasyBotMove(board);
            else if (mode == 3)
                column = getMediumMove(board, current);
            else
                column = getHardMove(board, current);

            input = column + 1;
            printf("Bot plays column %d\n", input);
        } else {
            printf("Player %c's turn. Enter column (1-%d): ", current, COLS);
            fflush(stdout);
            int read = read_user_column();
            if (read == -1) {
                printf("\nEnd of input. Exiting game.\n");
                return;
            }
            if (read == 0 || read < 1 || read > COLS) {
                printf("Invalid input. Choose a number 1-%d.\n", COLS);
                continue;
            }
            input = read;
            column = input - 1;
        }

        int row = dropChecker(board, column, current);
        if (row == -1) {
            printf("Column full or invalid. Try again.\n");
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

    if (scanf("%d", &mode) != 1 || mode < 1 || mode > 8) {
        fprintf(stderr, "Invalid mode.\n");
        return 1;
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    char startingPlayer = PLAYER_A;

    // ONLY ask for starting player in local modes
    if (mode >= 1 && mode <= 4) {
        printf("\nWho should start first?\n");
        printf("1. Player A (You)\n");
        if (mode != 1) printf("2. Bot\n");
        else printf("2. Player B\n");

        printf("Enter choice: ");
        int startChoice;
        if (scanf("%d", &startChoice) == 1) {
            if (startChoice == 2)
                startingPlayer = PLAYER_B;
        }
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }

    if (mode >= 5 && mode <= 8) {
        int socket;
        int port = 8888;

        socket = startServer(port);
        if (socket < 0) {
            fprintf(stderr, "Failed to start server\n");
            return 1;
        }

        int botMode = 1;
        if (mode == 6) botMode = 2;
        else if (mode == 7) botMode = 3;
        else if (mode == 8) botMode = 4;

        playNetworkGame(botMode, socket, 1);
        closeConnection(socket);
    } else {
        playGame(mode, startingPlayer);
    }

    return 0;
}
