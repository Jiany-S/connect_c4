// src/network.c - Linux only
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "network.h"
#include "board.h"
#include "game.h"
#include "bot_easy.h"
#include "bot_medium.h"
#include "bot_hard.h"

// Get local IP address
static void getLocalIP(char *ipBuffer, size_t bufSize) {
    char hostName[256];
    struct hostent *host_entry;
    
    if (gethostname(hostName, sizeof(hostName)) == 0) {
        host_entry = gethostbyname(hostName);
        if (host_entry != NULL && host_entry->h_addr_list[0] != NULL) {
            snprintf(ipBuffer, bufSize, "%s", 
                    inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])));
            return;
        }
    }
    snprintf(ipBuffer, bufSize, "127.0.0.1");
}

int startServer(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Failed to create socket");
        return -1;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Failed to bind socket");
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 1) < 0) {
        perror("Failed to listen on socket");
        close(serverSocket);
        return -1;
    }

    char ipBuffer[64];
    getLocalIP(ipBuffer, sizeof(ipBuffer));
    printf("\n=================================\n");
    printf("Server started!\n");
    printf("IP Address: %s\n", ipBuffer);
    printf("Port: %d\n", port);
    printf("=================================\n");
    printf("Waiting for remote player to connect...\n");
    printf("\nRemote player can connect by running:\n");
    printf("  ./connect4_client %s %d\n", ipBuffer, port);
    printf("=================================\n\n");

    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    
    if (clientSocket < 0) {
        perror("Failed to accept connection");
        close(serverSocket);
        return -1;
    }

    printf("Remote player connected from %s\n\n", inet_ntoa(clientAddr.sin_addr));
    close(serverSocket);
    
    return clientSocket;
}

int connectToServer(const char *host, int port) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Failed to create socket");
        return -1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &serverAddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address or address not supported\n");
        close(clientSocket);
        return -1;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return -1;
    }

    printf("Connected to server at %s:%d\n\n", host, port);
    return clientSocket;
}

int sendBoardUpdate(int socket, const Board board, char currentPlayer) {
    char buffer[ROWS * COLS + 10];
    int pos = 0;
    
    buffer[pos++] = MSG_BOARD_UPDATE;
    buffer[pos++] = currentPlayer;
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            buffer[pos++] = board[i][j];
        }
    }
    
    int sent = send(socket, buffer, pos, 0);
    return (sent == pos) ? 0 : -1;
}

int sendMoveRequest(int socket, char player) {
    char buffer[2];
    buffer[0] = MSG_MOVE_REQUEST;
    buffer[1] = player;
    
    int sent = send(socket, buffer, 2, 0);
    return (sent == 2) ? 0 : -1;
}

int receiveMoveResponse(int socket, int *column) {
    char buffer[2];
    int received = recv(socket, buffer, 2, 0);
    
    if (received != 2 || buffer[0] != MSG_MOVE_RESPONSE) {
        return -1;
    }
    
    *column = buffer[1];
    return 0;
}

int sendMove(int socket, int column) {
    char buffer[2];
    buffer[0] = MSG_MOVE_RESPONSE;
    buffer[1] = (char)column;
    
    int sent = send(socket, buffer, 2, 0);
    return (sent == 2) ? 0 : -1;
}

int receiveMove(int socket) {
    char buffer[2];
    int received = recv(socket, buffer, 2, 0);
    
    if (received != 2 || buffer[0] != MSG_MOVE_RESPONSE) {
        return -1;
    }
    
    return buffer[1];
}

static int receiveBoardUpdate(int socket, Board board, char *currentPlayer) {
    char buffer[ROWS * COLS + 10];
    int received = recv(socket, buffer, ROWS * COLS + 2, 0);
    
    if (received < 2 || buffer[0] != MSG_BOARD_UPDATE) {
        return -1;
    }
    
    *currentPlayer = buffer[1];
    int pos = 2;
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (pos >= received) return -1;
            board[i][j] = buffer[pos++];
        }
    }
    
    return 0;
}

void closeConnection(int socket) {
    close(socket);
}

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

void playNetworkGame(int mode, int socket, int isServer) {
    Board board;
    initBoard(board);
    char current = PLAYER_A;
    int move_count = 0;

    char localPlayer = isServer ? PLAYER_A : PLAYER_B;
    char remotePlayer = isServer ? PLAYER_B : PLAYER_A;

    while (1) {
        // Client always waits for board update at the start of turn
        if (!isServer) {
            if (receiveBoardUpdate(socket, board, &current) != 0) {
                fprintf(stderr, "Lost connection to server\n");
                return;
            }
        }

        printBoard(board);

        int column;
        int input = 0;

        if (current == localPlayer) {
            // Local turn (server or client)
            printf("Your turn (Player %c). Enter column (1-%d): ", current, COLS);
            fflush(stdout);

            int read = read_user_column();
            if (read == -1) {
                printf("\nEnd of input. Exiting game.\n");
                return;
            }
            if (read == 0) {
                printf("Invalid input. Please type a column number 1-%d and press Enter.\n", COLS);
                continue;
            }

            input = read;
            column = input - 1;

            if (!isServer) {
                // Client sends move to server
                if (sendMove(socket, column) != 0) {
                    fprintf(stderr, "Failed to send move to server\n");
                    return;
                }
                // Wait for next board update from server
                continue;
            }
        } else if ((mode >= 2 && mode <= 4) && current == PLAYER_B && isServer) {
            // Server's bot turn
            if (mode == 2) column = getEasyBotMove(board);
            else if (mode == 3) column = getMediumMove(board, current);
            else column = getHardMove(board, current);

            input = column + 1;
            printf("Bot plays column %d\n", input);
        } else if (current == remotePlayer && isServer) {
            // Server waiting for remote player (client) move
            printf("Waiting for remote player (Player %c) to make a move...\n", current);

            if (receiveMove(socket, &column) != 0) {
                fprintf(stderr, "Failed to receive move from remote player\n");
                return;
            }

            input = column + 1;
            printf("Remote player plays column %d\n", input);
        }

        // Validate and apply move (server only)
        if (isServer) {
            int row = dropChecker(board, column, current);
            if (row == -1) {
                if (!VALID_COLUMN(column)) {
                    printf("Column %d is out of range. Choose 1-%d.\n", input, COLS);
                } else {
                    printf("Column %d is full or move invalid. Try another column.\n", input);
                }
                continue;
            }

            move_count++;

            // Check win/draw before sending board update
            int gameWon = checkWin(board, current);
            int gameDraw = checkDraw(board);

            // Send board update to client
            if (!isServer || socket >= 0) {
                if (sendBoardUpdate(socket, board, SWITCH_PLAYER(current)) != 0) {
                    fprintf(stderr, "Failed to send board update\n");
                    return;
                }
            }

            if (gameWon) {
                printBoard(board);
                printf("Player %c wins!\n", current);
                if (current == localPlayer) printf("You win!\n");
                else if ((mode >= 2 && mode <= 4) && current == PLAYER_B) printf("Bot wins!\n");
                else printf("Remote player wins!\n");
                return;
            }

            if (gameDraw) {
                printBoard(board);
                printf("The game is a draw.\n");
                return;
            }

            current = SWITCH_PLAYER(current);
        }
    }
}
