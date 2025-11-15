// src/bot_hard.c
#include <limits.h>
#include <string.h>
#include "bot_hard.h"
#include "game.h"

#define MAX_DEPTH 12
#define WIN_SCORE 100000
#define LOSS_SCORE -100000
#define DOUBLE_THREAT_BONUS 800
#define DOUBLE_THREAT_PENALTY 900
#define OPP_THREAT_PENALTY 120
#define CENTER_WEIGHT 7
#define NOVICE_TRAP_BONUS 200

#ifdef MT_BUILD
#include <pthread.h>
#include <stdlib.h>
#endif

static int isTerminalNode(char board[ROWS][COLS], char aiPlayer);
static void collectValidColumns(char board[ROWS][COLS], int validCols[COLS], int *validCount);
static void simulateMove(Board dest, char board[ROWS][COLS], int column, char player);
static int scoreWindow(char window[4], char aiPlayer);
static int countCenterBonus(char board[ROWS][COLS], char aiPlayer);
static int countThreatsForPlayer(char board[ROWS][COLS], char player);
static int evaluateNoviceTrapBottomRow(char board[ROWS][COLS], char aiPlayer);
static int evaluateBoard(char board[ROWS][COLS], char aiPlayer);
static int minimax(char board[ROWS][COLS], int depth, int alpha, int beta, int isMaximizing, char aiPlayer);

static int isTerminalNode(char board[ROWS][COLS], char aiPlayer) {
    char opponent = SWITCH_PLAYER(aiPlayer);
    if (checkWin(board, aiPlayer) || checkWin(board, opponent)) {
        return 1;
    }
    return checkDraw(board);
}

static void collectValidColumns(char board[ROWS][COLS], int validCols[COLS], int *validCount) {
    const int order[COLS] = {3, 2, 4, 1, 5, 0, 6};
    int count = 0;
    for (int i = 0; i < COLS; ++i) {
        int col = order[i];
        if (col >= 0 && col < COLS && board[0][col] == EMPTY) {
            validCols[count++] = col;
        }
    }
    *validCount = count;
}

static void simulateMove(Board dest, char board[ROWS][COLS], int column, char player) {
    memcpy(dest, board, sizeof(Board));
    dropChecker(dest, column, player);
}

static int scoreWindow(char window[4], char aiPlayer) {
    char opponent = SWITCH_PLAYER(aiPlayer);
    int aiCount = 0;
    int oppCount = 0;
    int emptyCount = 0;

    for (int i = 0; i < 4; ++i) {
        if (window[i] == aiPlayer) {
            ++aiCount;
        } else if (window[i] == opponent) {
            ++oppCount;
        } else if (window[i] == EMPTY) {
            ++emptyCount;
        }
    }

    int score = 0;
    if (aiCount == 4) {
        score += 10000;
    } else if (aiCount == 3 && emptyCount == 1) {
        score += 100;
    } else if (aiCount == 2 && emptyCount == 2) {
        score += 10;
    }

    if (oppCount == 4) {
        score -= 10000;
    } else if (oppCount == 3 && emptyCount == 1) {
        score -= 120;
    } else if (oppCount == 2 && emptyCount == 2) {
        score -= 10;
    }

    return score;
}

static int countCenterBonus(char board[ROWS][COLS], char aiPlayer) {
    int centerColumn = COLS / 2;
    int count = 0;
    for (int row = 0; row < ROWS; ++row) {
        if (board[row][centerColumn] == aiPlayer) {
            ++count;
        }
    }
    return count * CENTER_WEIGHT;
}

static int countThreatsForPlayer(char board[ROWS][COLS], char player) {
    int threats = 0;
    for (int col = 0; col < COLS; ++col) {
        if (board[0][col] != EMPTY) {
            continue;
        }
        Board temp;
        simulateMove(temp, board, col, player);
        if (checkWin(temp, player)) {
            ++threats;
        }
    }
    return threats;
}

static int evaluateNoviceTrapBottomRow(char board[ROWS][COLS], char aiPlayer) {
    int row = ROWS - 1;
    int bonus = 0;
    for (int col = 0; col <= COLS - 4; ++col) {
        char c0 = board[row][col];
        char c1 = board[row][col + 1];
        char c2 = board[row][col + 2];
        char c3 = board[row][col + 3];
        if (c0 == aiPlayer && c1 == aiPlayer && c2 == aiPlayer && c3 == EMPTY) {
            bonus += NOVICE_TRAP_BONUS;
        } else if (c0 == EMPTY && c1 == aiPlayer && c2 == aiPlayer && c3 == aiPlayer) {
            bonus += NOVICE_TRAP_BONUS;
        }
    }
    return bonus;
}

static int evaluateBoard(char board[ROWS][COLS], char aiPlayer) {
    int score = 0;
    char opponent = SWITCH_PLAYER(aiPlayer);
    char window[4];

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col <= COLS - 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                window[i] = board[row][col + i];
            }
            score += scoreWindow(window, aiPlayer);
        }
    }

    for (int col = 0; col < COLS; ++col) {
        for (int row = 0; row <= ROWS - 4; ++row) {
            for (int i = 0; i < 4; ++i) {
                window[i] = board[row + i][col];
            }
            score += scoreWindow(window, aiPlayer);
        }
    }

    for (int row = 0; row <= ROWS - 4; ++row) {
        for (int col = 0; col <= COLS - 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                window[i] = board[row + i][col + i];
            }
            score += scoreWindow(window, aiPlayer);
        }
    }

    for (int row = 3; row < ROWS; ++row) {
        for (int col = 0; col <= COLS - 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                window[i] = board[row - i][col + i];
            }
            score += scoreWindow(window, aiPlayer);
        }
    }

    score += countCenterBonus(board, aiPlayer);

    int aiThreats = countThreatsForPlayer(board, aiPlayer);
    int oppThreats = countThreatsForPlayer(board, opponent);
    if (aiThreats >= 2) {
        score += DOUBLE_THREAT_BONUS;
    }
    if (oppThreats >= 2) {
        score -= DOUBLE_THREAT_PENALTY;
    }

    score += evaluateNoviceTrapBottomRow(board, aiPlayer);
    if (oppThreats > 0) {
        score -= oppThreats * OPP_THREAT_PENALTY;
    }

    return score;
}

static int minimax(char board[ROWS][COLS], int depth, int alpha, int beta, int isMaximizing, char aiPlayer) {
    char opponent = SWITCH_PLAYER(aiPlayer);

    if (checkWin(board, aiPlayer)) {
        return WIN_SCORE - depth;
    }
    if (checkWin(board, opponent)) {
        return LOSS_SCORE + depth;
    }
    if (depth == 0 || isTerminalNode(board, aiPlayer)) {
        return evaluateBoard(board, aiPlayer);
    }

    int validCols[COLS];
    int validCount = 0;
    collectValidColumns(board, validCols, &validCount);
    if (validCount == 0) {
        return evaluateBoard(board, aiPlayer);
    }

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int i = 0; i < validCount; ++i) {
            int col = validCols[i];
            Board temp;
            simulateMove(temp, board, col, aiPlayer);
            int eval = minimax(temp, depth - 1, alpha, beta, 0, aiPlayer);
            if (eval > maxEval) {
                maxEval = eval;
            }
            if (eval > alpha) {
                alpha = eval;
            }
            if (alpha >= beta) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i = 0; i < validCount; ++i) {
            int col = validCols[i];
            Board temp;
            simulateMove(temp, board, col, opponent);
            int eval = minimax(temp, depth - 1, alpha, beta, 1, aiPlayer);
            if (eval < minEval) {
                minEval = eval;
            }
            if (eval < beta) {
                beta = eval;
            }
            if (alpha >= beta) {
                break;
            }
        }
        return minEval;
    }
}

#ifdef MT_BUILD

typedef struct {
    Board board_copy;
    int column;
    char player;
    int score;
} ThreadArg;

static void *thread_worker(void *arg_ptr) {
    ThreadArg *arg = (ThreadArg *)arg_ptr;
    arg->score = minimax(arg->board_copy, MAX_DEPTH - 1, INT_MIN, INT_MAX, 0, arg->player);
    return NULL;
}
#endif

int getHardMove(char board[ROWS][COLS], char player) {
    int validCols[COLS];
    int validCount = 0;
    collectValidColumns(board, validCols, &validCount);
    if (validCount == 0) {
        return -1;
    }

#ifdef MT_BUILD
    pthread_t *threads = malloc(sizeof(pthread_t) * validCount);
    ThreadArg *args = malloc(sizeof(ThreadArg) * validCount);
    if (!threads || !args) {
        free(threads);
        free(args);
        goto single_threaded_eval;
    }

    for (int i = 0; i < validCount; ++i) {
        int col = validCols[i];
        simulateMove(args[i].board_copy, board, col, player);
        args[i].column = col;
        args[i].player = player;
        args[i].score = INT_MIN;

        if (pthread_create(&threads[i], NULL, thread_worker, &args[i]) != 0) {
            // fallback synchronous evaluation for this column
            args[i].score = minimax(args[i].board_copy, MAX_DEPTH - 1, INT_MIN, INT_MAX, 0, player);
            threads[i] = 0;
        }
    }

    int bestColumn = -1;
    int bestScore = INT_MIN;
    int center = COLS / 2;
    int bestDist = COLS;

    for (int i = 0; i < validCount; ++i) {
        if (threads[i] != 0) {
            pthread_join(threads[i], NULL);
        }
        int score = args[i].score;
        int col = args[i].column;
        int dist = center - col;
        if (dist < 0) dist = -dist;
        if (score > bestScore || (score == bestScore && dist < bestDist)) {
            bestScore = score;
            bestColumn = col;
            bestDist = dist;
        }
    }

    free(threads);
    free(args);
    return bestColumn;

single_threaded_eval:
#endif

    {
        int bestColumn = -1;
        int bestScore = INT_MIN;
        int center = COLS / 2;
        int bestDist = COLS;

        for (int i = 0; i < validCount; ++i) {
            int col = validCols[i];
            Board temp;
            simulateMove(temp, board, col, player);
            int score = minimax(temp, MAX_DEPTH - 1, INT_MIN, INT_MAX, 0, player);
            int dist = center - col;
            if (dist < 0) {
                dist = -dist;
            }
            if (score > bestScore || (score == bestScore && dist < bestDist)) {
                bestScore = score;
                bestColumn = col;
                bestDist = dist;
            }
        }

        return bestColumn;
    }
}
