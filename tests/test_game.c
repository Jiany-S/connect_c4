#include "../src/board.h"
#include "../src/game.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Board b;

    // Vertical win test
    initBoard(b);
    for (int i = 0; i < 4; i++) dropChecker(b, 0, PLAYER_A);
    assert(checkWin(b, PLAYER_A) == 1);

    // Horizontal win test
    initBoard(b);
    for (int i = 0; i < 4; i++) dropChecker(b, i, PLAYER_B);
    assert(checkWin(b, PLAYER_B) == 1);

    // Diagonal win test
    initBoard(b);
    dropChecker(b, 0, PLAYER_A);
    dropChecker(b, 1, PLAYER_B); dropChecker(b, 1, PLAYER_A);
    dropChecker(b, 2, PLAYER_B); dropChecker(b, 2, PLAYER_B); dropChecker(b, 2, PLAYER_A);
    dropChecker(b, 3); dropChecker(b, 3); dropChecker(b, 3); dropChecker(b, 3, PLAYER_A);
    assert(checkWin(b, PLAYER_A) == 1);

    printf("test_game PASSED\n");
    return 0;
}
