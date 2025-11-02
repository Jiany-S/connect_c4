#include "../src/board.h"
#include "../src/game.h"
#include "../src/bot_medium.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Board b;
    initBoard(b);

    // Winning move for PLAYER_B in column 0 (three B's stacked)
    dropChecker(b, 0, PLAYER_B);
    dropChecker(b, 0, PLAYER_B);
    dropChecker(b, 0, PLAYER_B);
    int move = getMediumMove(b, PLAYER_B);
    assert(move == 0);

    // Blocking move: PLAYER_A threatens column 1
    initBoard(b);
    dropChecker(b, 1, PLAYER_A);
    dropChecker(b, 1, PLAYER_A);
    dropChecker(b, 1, PLAYER_A);
    move = getMediumMove(b, PLAYER_B);
    assert(move == 1);

    printf("test_bot_medium PASSED\n");
    return 0;
}
