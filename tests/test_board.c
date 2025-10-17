#include "../src/board.h"
#include <assert.h>
#include <stdio.h>

int main() {
    Board b;
    initBoard(b);

    // Check all cells empty
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            assert(b[r][c] == EMPTY);
        }
    }

    printf("test_board PASSED\n");
    return 0;
}
