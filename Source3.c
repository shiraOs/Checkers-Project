#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header3.h"

/***** Static Function Prototypes *****/
static void InitializeList(MultipleSourceMovesList* lst);					//return by reference empty list
static MultipleSourceMovesListCell* createNewListCell(SingleSourceMovesList* lst_to_add);					//return a new list cell
static void AddToList(MultipleSourceMovesList* list, MultipleSourceMovesListCell* cell_to_add);				//check where to add to the list
static void AddToEmptyList(MultipleSourceMovesList* list, MultipleSourceMovesListCell* cell_to_add);		//only if head=null
static void AddToBeginningOfList(MultipleSourceMovesList* list, MultipleSourceMovesListCell* cell_to_add);	//add to the head of list
static void freeTreeAux(SingleSourceMovesTreeNode* root);					//free tree memory recursivly


/******************* Function Implementation *******************/

MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{//return empty list- head and tail NULL if no possible moves
	//declaration of variables
	int i, j;
	SingleSourceMovesTree* playerTree = NULL;
	SingleSourceMovesList* playerList = NULL;
	checkersPos* playerPos = NULL;
	MultipleSourceMovesList* res = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	checkAllocation(res);
	InitializeList(res);

	for (i = 0; i < BOARD_SIZE; i++)	//run on the board-only gray squares
	{//check for even place
		if (i % 2 == 0)
		{
			for (j = 1; j < BOARD_SIZE; j = j + 2)
				if (board[i][j] == player)					//check if its the same player
				{
					playerPos = createCheckersPos(i, j);					//create checker position
					playerTree = FindSingleSourceMoves(board, playerPos);	//build its moves tree else return NULL
					if (playerTree)											//check if there is tree- there are moves 
					{
						playerList = FindSingleSourceOptimalMoves(playerTree);		//build a list of the best play from tree
						AddToList(res, createNewListCell(playerList));				//add the move list to the list of playes
						freeTree(playerTree);		//free memory of tree
					}
					free(playerPos);				//free memory of checkers pos
				}
		}
		else
		{
			for (j = 0; j < BOARD_SIZE; j = j + 2)
				if (board[i][j] == player)					//check if its the same player
				{
					playerPos = createCheckersPos(i, j);					//create checker position
					playerTree = FindSingleSourceMoves(board, playerPos);	//build its moves tree else return NULL
					if (playerTree)											//check if there is tree- there are moves
					{
						playerList = FindSingleSourceOptimalMoves(playerTree);		//build a list of the best play from tree
						AddToList(res, createNewListCell(playerList));				//add the moves list to the list of playes
						freeTree(playerTree);		//free memory of tree
					}
					free(playerPos);				//free memory of checkers pos
				}
		}
	}

	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void InitializeList(MultipleSourceMovesList* lst)
{//Gets a list and put null in its head and tail
	lst->head = lst->tail = NULL;
}

static void AddToList(MultipleSourceMovesList* list, MultipleSourceMovesListCell* cell_to_add)
{//Gets a list and check if empty add to empty list else add at the beginning
	if (!list->head)
		AddToEmptyList(list, cell_to_add);
	else
		AddToBeginningOfList(list, cell_to_add);
}

static void AddToEmptyList(MultipleSourceMovesList* list, MultipleSourceMovesListCell* cell_to_add)
{//Gets a list and put the cell to add at head and tail
	list->head = list->tail = cell_to_add;
}

static void AddToBeginningOfList(MultipleSourceMovesList* list, MultipleSourceMovesListCell* cell_to_add)
{//Gets a list and add the cell to add to the beginning
	cell_to_add->next = list->head;
	list->head = cell_to_add;
}

static MultipleSourceMovesListCell* createNewListCell(SingleSourceMovesList* lst_to_add)
{//Gets a list of move and create a cell to the list of all moves
	MultipleSourceMovesListCell* res = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	checkAllocation(res);

	res->single_source_moves_list = lst_to_add;
	res->next = NULL;

	return res;
}

/////////////////////////// FREE FUNCTIONS //////////////////////////////////////////////////////

void freeTree(SingleSourceMovesTree* playerTree)
{
	freeTreeAux(playerTree->source);
}

static void freeTreeAux(SingleSourceMovesTreeNode* root)
{
	if (root)
	{
		freeTreeAux(root->nextMove[0]);
		freeTreeAux(root->nextMove[1]);

		free(root->pos);
		free(root);
	}
}