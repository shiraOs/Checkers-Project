#pragma once

/***** INCLUDES *****/
#include "Header1.h"

/***** TYPEDEFS *****/
typedef struct _SingleSourceMovesListCell {
	checkersPos* position;
	unsigned short captures;
	struct _SingleSourceMovesListCell* next;
}SingleSourceMovesListCell;

typedef struct _SingleSourceMovesList {
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
}SingleSourceMovesList;

/***** Function Prototypes *****/
SingleSourceMovesList* FindSingleSourceOptimalMoves(SingleSourceMovesTree* moves_tree);