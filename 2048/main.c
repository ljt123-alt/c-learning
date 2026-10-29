#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "game.h"


int runMoveTest(char direction,
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
            return 0;
    }

    int passed = boardsEqual(board, expected)
                 && score == expectedScore;

    printf("%c: %s (实际得分: %d)\n",
           direction,
           passed ? "PASS" : "FAIL",
           score);

    if (!passed) {
        printf("实际棋盘:\n");
        printBoard(board);
    }

    return passed;
}


int main(void){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    printf("编码测试：当前得分\n");
    int score = 0;
    char command;
    int board[SIZE][SIZE] = {
        {0}
    };
    srand((unsigned int)time(NULL));
    addRandomTile(board);
    addRandomTile(board);
    while (1)
    {
        printf("========== 2048 ==========\n");
        int oldBoard[SIZE][SIZE];
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                oldBoard[i][j] = board[i][j];
            }
        }
        printBoard(board);
        printf("当前得分：%d\n", score);
        printf("请输入移动方向 (w: 上, s: 下, a: 左, d: 右): ");
        if (scanf(" %c", &command) != 1) {
            printf("输入错误\n");
            return 1;
        }
        switch (command) {
            case 'w':
            case 'W':
                moveUp(board,&score);
                break;
            case 's':
            case 'S':
                moveDown(board,&score);
                break;
            case 'a':
            case 'A':
                moveLeft(board,&score);
                break;
            case 'd':
            case 'D':
                moveRight(board,&score);
                break;
            case 'q':
            case 'Q':
                printf("游戏已退出\n");
                return 0;
            default:
                printf("无效的输入\n");
                continue; 
        }
        if (hasWon(board))
        {
            printf("\n========== 最终棋盘 ==========\n");
            printBoard(board);
            printf("游戏成功,最终得分为%d\n",score);
            return 0;
        }
        if (!boardsEqual(oldBoard, board)) {
            addRandomTile(board);
        }
        if (!hasMoves(board))
        {
            printf("\n========== 最终棋盘 ==========\n");
            printBoard(board);
            printf("游戏结束,最终得分为%d\n",score);
            return 0;
        }
        
        
    }
    //addRandomTile(board);
    //addRandomTile(board);
    //moveLeft(board);
    //moveRight(board);
    return 0;
}