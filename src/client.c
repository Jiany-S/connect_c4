// Simple Connect 4 Network Client - Linux only
// Usage: ./connect4_client <server_ip> [port]
// Example: ./connect4_client 192.168.1.100 8888

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ROWS 6
#define COLS 7
#define EMPTY '.'
#define PLAYER_A 'A'
#define PLAYER_B 'B'

#define MSG_BOARD_UPDATE 1
#define MSG_MOVE_REQUEST 2
#define MSG_MOVE_RESPONSE 3
#define MSG_GAME_OVER 4

typedef char Board[ROWS][COLS];

void printBoard(const Board board) {
    printf("\n");
    for (int col = 1; col <= COLS; col++) {
        printf("  %d ", col);
    }
    printf("\n");
    
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            printf("| %c ", board[row][col]);
        }
        printf("|\n");
    }
    
    for (int col = 0; col < COLS; col++) {
        printf("+---");
    }
    printf("+\n\n");
}

int receiveBoard(int socket, Board board, char *currentPlayer) {
    char buffer[ROWS * COLS + 10];
    int received = recv(socket, buffer, sizeof(buffer), 0);
    
    if (received <= 0) {
        return -1;
    }
    
    if (buffer[0] != MSG_BOARD_UPDATE) {
        if (buffer[0] == MSG_GAME_OVER) {
            return -2;
        }
        return -1;
    }
    
    *currentPlayer = buffer[1];
    
    int pos = 2;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = buffer[pos++];
        }
    }
    
    return 0;
}

int sendMove(int socket, int column) {
    char buffer[2];
    buffer[0] = MSG_MOVE_RESPONSE;
    buffer[1] = (char)column;
    
    int sent = send(socket, buffer, 2, 0);
    return (sent == 2) ? 0 : -1;
}

int readColumn() {
    char buf[64];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return -1;
    }
    
    char *p = buf;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\n' || *p == '\0') return -2;
    
    char *endptr;
    long val = strtol(p, &endptr, 10);
    if (endptr == p) return -2;
    
    return (int)val;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_ip> [port]\n", argv[0]);
        fprintf(stderr, "Example: %s 192.168.1.100 8888\n", argv[0]);
        return 1;
    }
    
    const char *server_ip = argv[1];
    int port = (argc >= 3) ? atoi(argv[2]) : 8888;
    
    printf("Connecting to server %s:%d...\n", server_ip, port);
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Failed to create socket");
        return 1;
    }
    
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &serverAddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address\n");
        close(sock);
        return 1;
    }
    
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }
    
    printf("Connected! You are Player B\n");
    printf("Waiting for game to start...\n\n");
    
    Board board;
    char currentPlayer;
    
    while (1) {
        int result = receiveBoard(sock, board, &currentPlayer);
        
        if (result == -2) {
            printf("Game over!\n");
            break;
        }
        
        if (result < 0) {
            fprintf(stderr, "Connection lost\n");
            break;
        }
        
        printBoard(board);
        
        if (currentPlayer == PLAYER_B) {
            printf("Your turn (Player B). Enter column (1-%d): ", COLS);
            fflush(stdout);
            
            int input = readColumn();
            if (input == -1) {
                printf("\nExiting...\n");
                break;
            }
            
            if (input == -2 || input < 1 || input > COLS) {
                printf("Invalid input. Please enter a number between 1 and %d.\n", COLS);
                continue;
            }
            
            if (sendMove(sock, input - 1) < 0) {
                fprintf(stderr, "Failed to send move\n");
                break;
            }
        } else {
            printf("Waiting for Player A to move...\n");
        }
    }
    
    close(sock);
    return 0;
}
