#include "../src/board.h"
#include "../src/game.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Board b;
    initBoard(b);

    // Drop four checkers in same column
    for (int i = 0; i < 4; i++) {
        dropChecker(b, 0, PLAYER_A);
    }
    assert(checkWin(b, PLAYER_A) == 1);

    // Reset board
    initBoard(b);

    // Horizontal win
    for (int i = 0; i < 4; i++) {
        dropChecker(b, i, PLAYER_B);
    }
    assert(checkWin(b, PLAYER_B) == 1);

    printf("test_game PASSED\n");
    return 0;
}
