#ifndef GAME_H
#define GAME_H

#define SIZE 4

void printBoard(int board[SIZE][SIZE]);
void moveLeft(int board[SIZE][SIZE], int *score);
void moveRight(int board[SIZE][SIZE], int *score);
void moveUp(int board[SIZE][SIZE], int *score);
void moveDown(int board[SIZE][SIZE], int *score);

int addRandomTile(int board[SIZE][SIZE]);
int boardsEqual(int a[SIZE][SIZE], int b[SIZE][SIZE]);
int hasMoves(int board[SIZE][SIZE]);
int hasWon(int board[SIZE][SIZE]);

#endif