#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header2.h"

/***** Static Function Prototypes *****/
//check recursively the tree for the best move, add cells to the list with position, update list by reference
static void FindSingleSourceOptimalMovesRec(unsigned int* hightTree, SingleSourceMovesTreeNode* moves_tree, SingleSourceMovesList* lst);
static unsigned int getHeightAux(SingleSourceMovesTreeNode* root);		//return the tree height recursively- root height is 0
static void InitializeList(SingleSourceMovesList* lst);					//return by reference empty list
static SingleSourceMovesListCell* createNewListCell(checkersPos* position, unsigned short captures);	//return a new list cell
static void AddToList(SingleSourceMovesList* list, SingleSourceMovesListCell* cell_to_add);				//check where to add to the list
static void AddToEmptyList(SingleSourceMovesList* list, SingleSourceMovesListCell* cell_to_add);		//only if head=null
static void AddToBeginningOfList(SingleSourceMovesList* list, SingleSourceMovesListCell* cell_to_add);	//add to the head of list

/******************* Function Implementation *******************/

SingleSourceMovesList* FindSingleSourceOptimalMoves(SingleSourceMovesTree* moves_tree)
{//Gets a tree and make a list with the best move, return empty list- head and tail NULL if no possible moves
	unsigned int treeHeight;
	SingleSourceMovesList* res = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	checkAllocation(res);
	InitializeList(res);			//make empty list

	if (moves_tree)					//if there is a tree- check his height- else it will return empty list
	{//if there is tree-there is at least one move-height 1 minimum, there is root.
		treeHeight = getHeightAux(moves_tree->source);							//get height
		FindSingleSourceOptimalMovesRec(&treeHeight, moves_tree->source, res);	//build list
	}
	return res;							//return empty list if no tree or full list
}

static void FindSingleSourceOptimalMovesRec(unsigned int* treeLevel, SingleSourceMovesTreeNode* moves_tree, SingleSourceMovesList* lst)
{//gets the height of the tree, tree root and a list.
	if (!moves_tree->nextMove[0] && !moves_tree->nextMove[1])			//if its a leaf
	{
		if (*treeLevel == moves_tree->total_captures_so_far)			//if the tree height equals total capture so far
		{//add to the list and decrease the height-check for next capture to add the list
			AddToList(lst, createNewListCell(moves_tree->pos, moves_tree->total_captures_so_far));
			(*treeLevel)--;
		}
	}
	else
	{//first check for best moves with captures-when height equals total capture from leaf to root
		if (moves_tree->nextMove[0])	//check for the left
			FindSingleSourceOptimalMovesRec(treeLevel, moves_tree->nextMove[0], lst);
		//check if not found (treeLevel has not changed-didnt get the leaf with captures equals level) at the left
		if (*treeLevel != moves_tree->total_captures_so_far)
			if (moves_tree->nextMove[1])						//check for the right
				FindSingleSourceOptimalMovesRec(treeLevel, moves_tree->nextMove[1], lst);
	 //if not found eating moves-situation of height 1 and total capture 0- one move with no capture	
		if (*treeLevel == 1 && moves_tree->total_captures_so_far == 0)
		{
			if (moves_tree->nextMove[0])			//add to list the left move if no left then the right move
				AddToList(lst, createNewListCell(moves_tree->nextMove[0]->pos, moves_tree->nextMove[0]->total_captures_so_far));
			else
				AddToList(lst, createNewListCell(moves_tree->nextMove[1]->pos, moves_tree->nextMove[1]->total_captures_so_far));
		}

		//add the current node to list
		AddToList(lst, createNewListCell(moves_tree->pos, moves_tree->total_captures_so_far));
		(*treeLevel)--;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static unsigned int getHeightAux(SingleSourceMovesTreeNode* root)
{//Gets the tree root and check recursivly its height
	int left_subtree_result = -1;
	int right_subtree_result = -1;

	if (root->nextMove[0])			//If there is a left sub tree, check its level
		left_subtree_result = getHeightAux(root->nextMove[0]);
	if (root->nextMove[1])			//If there is a right sub tree, check its level
		right_subtree_result = getHeightAux(root->nextMove[1]);

	return (1 + max(left_subtree_result, right_subtree_result));		//the biggest plus the root
}

/////////////////////////////////////////////////////////////////////////////////////////////////

static void InitializeList(SingleSourceMovesList* lst)
{//Gets a list and put null in its head and tail
	lst->head = lst->tail = NULL;
}

static void AddToList(SingleSourceMovesList* list, SingleSourceMovesListCell* cell_to_add)
{//Gets a list and check if empty add to empty list else add at the beginning
	if (!list->head)
		AddToEmptyList(list, cell_to_add);
	else
		AddToBeginningOfList(list, cell_to_add);
}

static void AddToEmptyList(SingleSourceMovesList* list, SingleSourceMovesListCell* cell_to_add)
{//Gets a list and put the cell to add at head and tail
	list->head = list->tail = cell_to_add;
}

static void AddToBeginningOfList(SingleSourceMovesList* list, SingleSourceMovesListCell* cell_to_add)
{//Gets a list and add the cell to add to the beginning
	cell_to_add->next = list->head;
	list->head = cell_to_add;
}

static SingleSourceMovesListCell* createNewListCell(checkersPos* position, unsigned short captures)
{//Gets a position and num of captures and build a list cell with the move 
	SingleSourceMovesListCell* res = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	checkAllocation(res);
	res->position = (checkersPos*)malloc(sizeof(checkersPos));
	checkAllocation(res->position);

	res->captures = captures;
	res->position->col = position->col;
	res->position->row = position->row;
	res->next = NULL;

	return res;
}