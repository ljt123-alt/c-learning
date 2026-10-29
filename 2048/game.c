#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void printBoard(int board[SIZE][SIZE]) {
    //打印当前棋盘状态
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                printf("%5s", ".");
            } else {
                printf("%5d", board[i][j]);
            }
        }
        printf("\n");
    }
}

void compressLeft(int board[SIZE][SIZE]) {
//向左压缩数字
    for (int i = 0; i < SIZE; i++) {
        int temp[SIZE] = {0};
        int k = 0;
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) {
                temp[k++] = board[i][j];
            }
        }
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = temp[j];
        }
    }
}
void mergeLeft(int board[SIZE][SIZE],int *s){
    //向左合并相同数字
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            if(board[i][j] != 0 && board[i][j] == board[i][j + 1]) {
                board[i][j] *= 2;
                *s += board[i][j];
                board[i][j + 1] = 0;
                j++; 
            }
        }
    }
}
void moveLeft(int board[SIZE][SIZE],int *s) {
    compressLeft(board);
    mergeLeft(board,s);
    compressLeft(board);
}
void compressRight(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        int temp[SIZE] = {0};
        int k = SIZE - 1;
        for (int j = SIZE - 1; j >= 0; j--) {
            if (board[i][j] != 0) {
                temp[k--] = board[i][j];
            }
        }
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = temp[j];
        }
    }
}

void mergeRight(int board[SIZE][SIZE],int *s) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 1; j > 0; j--) {
            if (board[i][j] != 0 && board[i][j] == board[i][j - 1]) {
                board[i][j] *= 2;
                *s += board[i][j];
                board[i][j - 1] = 0;
                j--; 
            }
        }
    }
}
void moveRight(int board[SIZE][SIZE],int *s) {
    compressRight(board);
    mergeRight(board,s);
    compressRight(board);
}
void compressUp(int board[SIZE][SIZE]) {
    for (int j = 0; j < SIZE; j++) {
        int temp[SIZE] = {0};
        int k = 0;
        for (int i = 0; i < SIZE; i++) {
            if (board[i][j] != 0) {
                temp[k++] = board[i][j];
            }
        }
        for (int i = 0; i < SIZE; i++) {
            board[i][j] = temp[i];
        }
    }
}
void mergeUp(int board[SIZE][SIZE],int *s) {
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE - 1; i++) {
            if (board[i][j] != 0 && board[i][j] == board[i + 1][j]) {
                board[i][j] *= 2;
                *s += board[i][j];
                board[i + 1][j] = 0;
                i++;
            }
        }
    }
}
void moveUp(int board[SIZE][SIZE],int *s) {
    compressUp(board);
    mergeUp(board,s);
    compressUp(board);
}
void compressDown(int board[SIZE][SIZE]) {
    for (int j = 0; j < SIZE; j++) {
        int temp[SIZE] = {0};
        int k = SIZE - 1;
        for (int i = SIZE - 1; i >= 0; i--) {
            if (board[i][j] != 0) {
                temp[k--] = board[i][j];
            }
        }
        for (int i = 0; i < SIZE; i++) {
            board[i][j] = temp[i];
        }
    }
}
void mergeDown(int board[SIZE][SIZE],int *s) {
    for (int j = 0; j < SIZE; j++) {
        for (int i = SIZE - 1; i > 0; i--) {
            if (board[i][j] != 0 && board[i][j] == board[i - 1][j]) {
                board[i][j] *= 2;
                *s += board[i][j];
                board[i - 1][j] = 0;
                i--;
            }
        }
    }
}
void moveDown(int board[SIZE][SIZE],int *s) {
    compressDown(board);
    mergeDown(board,s);
    compressDown(board);
}
int boardsEqual(int a[SIZE][SIZE], int b[SIZE][SIZE]){
    //检测棋盘是否发生变化
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (a[i][j] != b[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

int hasMoves(int board[SIZE][SIZE]){
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                return 1;
            }else if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) {
                return 1;
            }else if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return 1;
            }
        }
        
    }
    return 0;
}

int hasWon(int board[SIZE][SIZE]){
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] >= 2048)
            {
                return 1;
            }
            
        }
        
    }
    return 0;
}

int addRandomTile(int board[SIZE][SIZE]){
    int emptyCount = 0;
    int n;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                emptyCount += 1;
            }
            
        }
        
    }
    if (!emptyCount){
        return 0;
    }
    n = rand() % emptyCount;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            
            if (board[i][j] == 0)
            {
                if (!n)
                {
                    int probability = rand() % 10;
                    if ( probability < 9)
                    {
                        board[i][j] = 2;
                    }else{
                        board[i][j] = 4;
                    }
                    return 1;
                }
                n--;
            }  
        }
        
    }
    return 0;
}