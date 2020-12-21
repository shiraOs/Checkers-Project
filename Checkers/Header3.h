#pragma once

/***** INCLUDES *****/
#include "Header2.h"

/***** TYPEDEFS *****/
typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct _multipleSourceMovesListCell* next;
}MultipleSourceMovesListCell;

typedef struct _multipleSourceMovesList {
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
}MultipleSourceMovesList;

/***** Function Prototypes *****/
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);	//q. 3
void Turn(Board board, Player player);												//q. 4
void StoreBoard(Board board, char *filename);										//q. 5
void LoadBoard(char* filename, Board board);										//q. 6
void PlayGame(Board board, Player starting_player);									//q. 7

SingleSourceMovesList* FindMaxCaptures(MultipleSourceMovesList* MovesList);			//q. 4 and q. 7

void freeTree(SingleSourceMovesTree* playerTree);							//free tree memory
void freeMultipleMovesList(MultipleSourceMovesList* playerList);			//free multiple lists memory
