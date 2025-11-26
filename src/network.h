#ifndef NETWORK_H
#define NETWORK_H

#include "board.h"

// Network mode types
#define NET_SERVER 1
#define NET_CLIENT 2

// Message types
#define MSG_BOARD_UPDATE 1
#define MSG_MOVE_REQUEST 2
#define MSG_MOVE_RESPONSE 3
#define MSG_GAME_OVER 4
#define MSG_CONNECT 5

// Network functions
int startServer(int port);
int connectToServer(const char *host, int port);
int sendBoardUpdate(int socket, const Board board, char currentPlayer);
int sendMoveRequest(int socket, char player);
int receiveMoveResponse(int socket, int *column);
int sendMove(int socket, int column);
int receiveMove(int socket);
int receiveBoardUpdate(int socket, Board board, char *currentPlayer);
void closeConnection(int socket);

/* Updated to allow the host/user to choose who starts first */
void playNetworkGame(int mode, int socket, int isServer, char startingPlayer);

#endif
