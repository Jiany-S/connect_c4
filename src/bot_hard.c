#include <limits.h>
#include <string.h>
#include "bot_hard.h"
#include "game.h"

#define MAX_DEPTH 6
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

typedef char Board[ROWS][COLS];

static int isTerminalNode(char board[ROWS][COLS], char aiPlayer);
static void collectValidColumns(char board[ROWS][COLS], int validCols[COLS], int *validCount);
static void simulateMove(Board dest, char board[ROWS][COLS], int column, char player);
static int scoreWindow(char window[4], char aiPlayer);
static int countCenterBonus(char board[ROWS][COLS], char aiPlayer);
static int countThreatsForPlayer(char board[ROWS][COLS], char player);
static int evaluateNoviceTrapBottomRow(char board[ROWS][COLS], char aiPlayer);
static int evaluateBoard(char board[ROWS][COLS], char aiPlayer);
static int minimax(char board[ROWS][COLS], int depth, int alpha, int beta, int isMaximizing, char aiPlayer);

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

// New test-friendly function: allows forcing MT or ST
int getHardMoveWithThreads(char board[ROWS][COLS], char player, int useThreads) {
    int validCols[COLS];
    int validCount = 0;
    collectValidColumns(board, validCols, &validCount);
    if (validCount == 0) return -1;

#ifdef MT_BUILD
    if (useThreads) {
        pthread_t *threads = malloc(sizeof(pthread_t) * validCount);
        ThreadArg *args = malloc(sizeof(ThreadArg) * validCount);
        if (!threads || !args) { free(threads); free(args); goto single_threaded_eval; }

        for (int i = 0; i < validCount; ++i) {
            simulateMove(args[i].board_copy, board, validCols[i], player);
            args[i].column = validCols[i];
            args[i].player = player;
            args[i].score = INT_MIN;
            if (pthread_create(&threads[i], NULL, thread_worker, &args[i]) != 0) {
                args[i].score = minimax(args[i].board_copy, MAX_DEPTH - 1, INT_MIN, INT_MAX, 0, player);
                threads[i] = 0;
            }
        }

        int bestCol = -1, bestScore = INT_MIN, center = COLS / 2, bestDist = COLS;
        for (int i = 0; i < validCount; ++i) {
            if (threads[i]) pthread_join(threads[i], NULL);
            int score = args[i].score;
            int col = args[i].column;
            int dist = abs(center - col);
            if (score > bestScore || (score == bestScore && dist < bestDist)) {
                bestScore = score; bestCol = col; bestDist = dist;
            }
        }

        free(threads);
        free(args);
        return bestCol;
    }
#endif

single_threaded_eval:
    {
        int bestCol = -1, bestScore = INT_MIN, center = COLS / 2, bestDist = COLS;
        for (int i = 0; i < validCount; ++i) {
            Board temp;
            simulateMove(temp, board, validCols[i], player);
            int score = minimax(temp, MAX_DEPTH - 1, INT_MIN, INT_MAX, 0, player);
            int dist = abs(center - validCols[i]);
            if (score > bestScore || (score == bestScore && dist < bestDist)) {
                bestScore = score; bestCol = validCols[i]; bestDist = dist;
            }
        }
        return bestCol;
    }
}

// Normal gameplay function
int getHardMove(char board[ROWS][COLS], char player) {
    return getHardMoveWithThreads(board, player, USE_THREADS);
}

// --- Helper functions below ---
static int isTerminalNode(char board[ROWS][COLS], char aiPlayer) {
    char opponent = SWITCH_PLAYER(aiPlayer);
    return checkWin(board, aiPlayer) || checkWin(board, opponent) || checkDraw(board);
}

static void collectValidColumns(char board[ROWS][COLS], int validCols[COLS], int *validCount) {
    const int order[COLS] = {3, 2, 4, 1, 5, 0, 6};
    int count = 0;
    for (int i = 0; i < COLS; ++i) {
        int col = order[i];
        if (board[0][col] == EMPTY) validCols[count++] = col;
    }
    *validCount = count;
}

static void simulateMove(Board dest, char board[ROWS][COLS], int column, char player) {
    memcpy(dest, board, sizeof(Board));
    dropChecker(dest, column, player);
}

static int scoreWindow(char window[4], char aiPlayer) {
    char opponent = SWITCH_PLAYER(aiPlayer);
    int aiCount = 0, oppCount = 0, emptyCount = 0;
    for (int i = 0; i < 4; ++i) {
        if (window[i] == aiPlayer) ++aiCount;
        else if (window[i] == opponent) ++oppCount;
        else if (window[i] == EMPTY) ++emptyCount;
    }
    int score = 0;
    if (aiCount == 4) score += 10000;
    else if (aiCount == 3 && emptyCount == 1) score += 100;
    else if (aiCount == 2 && emptyCount == 2) score += 10;

    if (oppCount == 4) score -= 10000;
    else if (oppCount == 3 && emptyCount == 1) score -= 120;
    else if (oppCount == 2 && emptyCount == 2) score -= 10;
    return score;
}

static int countCenterBonus(char board[ROWS][COLS], char aiPlayer) {
    int centerCol = COLS / 2, count = 0;
    for (int r = 0; r < ROWS; ++r) if (board[r][centerCol] == aiPlayer) ++count;
    return count * CENTER_WEIGHT;
}

static int countThreatsForPlayer(char board[ROWS][COLS], char player) {
    int threats = 0;
    for (int col = 0; col < COLS; ++col) {
        if (board[0][col] != EMPTY) continue;
        Board temp;
        simulateMove(temp, board, col, player);
        if (checkWin(temp, player)) ++threats;
    }
    return threats;
}

static int evaluateNoviceTrapBottomRow(char board[ROWS][COLS], char aiPlayer) {
    int row = ROWS - 1, bonus = 0;
    for (int col = 0; col <= COLS - 4; ++col) {
        char c0 = board[row][col], c1 = board[row][col+1], c2 = board[row][col+2], c3 = board[row][col+3];
        if ((c0==aiPlayer && c1==aiPlayer && c2==aiPlayer && c3==EMPTY) ||
            (c0==EMPTY && c1==aiPlayer && c2==aiPlayer && c3==aiPlayer)) bonus += NOVICE_TRAP_BONUS;
    }
    return bonus;
}

static int evaluateBoard(char board[ROWS][COLS], char aiPlayer) {
    int score = 0;
    char opponent = SWITCH_PLAYER(aiPlayer);
    char window[4];

    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c <= COLS-4; ++c) {
            for (int i = 0; i < 4; ++i) window[i] = board[r][c+i];
            score += scoreWindow(window, aiPlayer);
        }

    for (int c = 0; c < COLS; ++c)
        for (int r = 0; r <= ROWS-4; ++r) {
            for (int i = 0; i < 4; ++i) window[i] = board[r+i][c];
            score += scoreWindow(window, aiPlayer);
        }

    for (int r = 0; r <= ROWS-4; ++r)
        for (int c = 0; c <= COLS-4; ++c) {
            for (int i = 0; i < 4; ++i) window[i] = board[r+i][c+i];
            score += scoreWindow(window, aiPlayer);
        }

    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c <= COLS-4; ++c) {
            for (int i = 0; i < 4; ++i) window[i] = board[r-i][c+i];
            score += scoreWindow(window, aiPlayer);
        }

    score += countCenterBonus(board, aiPlayer);

    int aiThreats = countThreatsForPlayer(board, aiPlayer);
    int oppThreats = countThreatsForPlayer(board, opponent);
    if (aiThreats >= 2) score += DOUBLE_THREAT_BONUS;
    if (oppThreats >= 2) score -= DOUBLE_THREAT_PENALTY;
    score += evaluateNoviceTrapBottomRow(board, aiPlayer);
    if (oppThreats > 0) score -= oppThreats * OPP_THREAT_PENALTY;

    return score;
}

static int minimax(char board[ROWS][COLS], int depth, int alpha, int beta, int isMax, char aiPlayer) {
    char opponent = SWITCH_PLAYER(aiPlayer);

    if (checkWin(board, aiPlayer)) return WIN_SCORE - depth;
    if (checkWin(board, opponent)) return LOSS_SCORE + depth;
    if (depth == 0 || isTerminalNode(board, aiPlayer)) return evaluateBoard(board, aiPlayer);

    int validCols[COLS], validCount=0;
    collectValidColumns(board, validCols, &validCount);
    if (validCount == 0) return evaluateBoard(board, aiPlayer);

    if (isMax) {
        int maxEval = INT_MIN;
        for (int i=0;i<validCount;i++) {
            Board temp;
            simulateMove(temp, board, validCols[i], aiPlayer);
            int eval = minimax(temp, depth-1, alpha, beta, 0, aiPlayer);
            if (eval>maxEval) maxEval=eval;
            if (eval>alpha) alpha=eval;
            if (alpha>=beta) break;
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i=0;i<validCount;i++) {
            Board temp;
            simulateMove(temp, board, validCols[i], opponent);
            int eval = minimax(temp, depth-1, alpha, beta, 1, aiPlayer);
            if (eval<minEval) minEval=eval;
            if (eval<beta) beta=eval;
            if (alpha>=beta) break;
        }
        return minEval;
    }
}
