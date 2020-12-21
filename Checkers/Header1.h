#pragma once

/***** INCLUDES *****/
#include <stdio.h>
#include <stdlib.h>

/***** DEFINES *****/
#define BOARD_SIZE 8

/***** TYPEDEFS *****/
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];
typedef unsigned char Player;

typedef struct _checkersPos {
	char row;
	char col;
}checkersPos;

typedef struct _SingleSourceMovesTreeNode {
	Board board;
	checkersPos* pos;
	unsigned short total_captures_so_far;				//num of dilug
	struct _SingleSourceMovesTreeNode* nextMove[2];		//destination to move right or left
}SingleSourceMovesTreeNode;

typedef struct _SingleSourceMovesTree {
	SingleSourceMovesTreeNode* source;
}SingleSourceMovesTree;

/***** Function Prototypes *****/
void buildBoard(Board board);						//building the starting board
checkersPos* createCheckersPos(int row, int col);	//create a position on board
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);	//q. 1
void checkAllocation(void* ptr);					//check allocation memory