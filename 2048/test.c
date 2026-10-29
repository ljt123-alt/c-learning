
#include <stdio.h>
#include "game.h"

// 测试某个方向移动后的棋盘和得分
static int runMoveTest(char direction,
                       int expected[SIZE][SIZE],
                       int expectedScore) {
    int board[SIZE][SIZE] = {
        {2, 2, 2, 2},
        {2, 0, 2, 2},
        {4, 4, 8, 8},
        {2, 2, 4, 4}
    };

    int score = 0;

    switch (direction) {
        case 'L':
            moveLeft(board, &score);
            break;
        case 'R':
            moveRight(board, &score);
            break;
        case 'U':
            moveUp(board, &score);
            break;
        case 'D':
            moveDown(board, &score);
            break;
        default:
            printf("Invalid direction: %c\n", direction);
            return 0;
    }

    int passed = boardsEqual(board, expected)
                 && score == expectedScore;

    printf("%c: %s (score: %d, expected: %d)\n",
           direction,
           passed ? "PASS" : "FAIL",
           score,
           expectedScore);

    if (!passed) {
        printf("Actual board:\n");
        printBoard(board);

        printf("Expected board:\n");
        printBoard(expected);
    }

    return passed;
}

int main(void) {
    // 向左移动
    int expectedLeft[SIZE][SIZE] = {
        {4, 4, 0, 0},
        {4, 2, 0, 0},
        {8, 16, 0, 0},
        {4, 8, 0, 0}
    };

    // 向右移动
    int expectedRight[SIZE][SIZE] = {
        {0, 0, 4, 4},
        {0, 0, 2, 4},
        {0, 0, 8, 16},
        {0, 0, 4, 8}
    };

    // 向上移动
    int expectedUp[SIZE][SIZE] = {
        {4, 2, 4, 4},
        {4, 4, 8, 8},
        {2, 2, 4, 4},
        {0, 0, 0, 0}
    };

    // 向下移动
    int expectedDown[SIZE][SIZE] = {
        {0, 0, 0, 0},
        {4, 2, 4, 4},
        {4, 4, 8, 8},
        {2, 2, 4, 4}
    };

    int passed = 0;

    passed += runMoveTest('L', expectedLeft, 48);
    passed += runMoveTest('R', expectedRight, 48);
    passed += runMoveTest('U', expectedUp, 12);
    passed += runMoveTest('D', expectedDown, 12);

    printf("\nPassed: %d / 4\n", passed);

    return passed == 4 ? 0 : 1;
}
