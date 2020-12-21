#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header3.h"

/***** Static Function Prototypes *****/
static void printMove(SingleSourceMovesList* BestMove, Player player);				//print the best move
static void updateBoard(Board board, SingleSourceMovesList* Move, Player player);	//update move in board
//delete if its a capture move
static void deleteFromBoard(SingleSourceMovesListCell* Prev, SingleSourceMovesListCell* Curr, Board board);
static void freeSingleMoveList(SingleSourceMovesList* playerList);					//free single list memory

/******************* Function Implementation *******************/

void Turn(Board board, Player player)
{
	//declaration of variable
	//gets all the moves for the player
	MultipleSourceMovesList* MovesList = FindAllPossiblePlayerMoves(board, player);
	
	if (MovesList->head)
	{//the list is not empty-there are moves
		//gets the list of the best move-most captures if have
		SingleSourceMovesList* BestMove = FindMaxCaptures(MovesList);
		printMove(BestMove, player);		//print the best move
		//update the board as needed-delete after capture and change position for player
		updateBoard(board, BestMove, player);
	}

	freeMultipleMovesList(MovesList);			//free memory of all moves list
}

SingleSourceMovesList* FindMaxCaptures(MultipleSourceMovesList* MovesList)
{//gets list of moves and return the list with the best move-most captures
	//declaration of variables
	MultipleSourceMovesListCell* Curr = MovesList->head;
	unsigned short maxCapture = 0, currCapture;
	SingleSourceMovesList* maxCaptureList = Curr->single_source_moves_list;

	while (Curr)
	{
		//gets the num of captures from the curr move
		currCapture = Curr->single_source_moves_list->tail->captures;		//At the tail of list-the total capture in move
		if (currCapture > maxCapture)
		{//check if curr captures bigger then max captures-if so change the max capture and max list
			maxCapture = currCapture;
			maxCaptureList = Curr->single_source_moves_list;
		}
		Curr = Curr->next;
	}

	return maxCaptureList;
}

static void printMove(SingleSourceMovesList* BestMove, Player player)
{//gets a list of the best move and the player who play now
	//declaration of variable
	SingleSourceMovesListCell* curr = BestMove->head;

	if (player == 'T')								//printing whos turn
		printf("PLAYER_TOP_DOWN's turn \n");
	else
		printf("PLAYER_BOTTOM_UP's turn \n");

	//printing the move
	printf("%c%c", curr->position->row, curr->position->col);
	curr = curr->next;
	while (curr)				//print all positions at lists- every step in the move
	{
		printf("->%c%c", curr->position->row, curr->position->col);
		curr = curr->next;
	}
	putchar('\n');
}

static void updateBoard(Board board, SingleSourceMovesList* Move, Player player)
{//gets the board, a move list and the player
	//declaration of variables
	SingleSourceMovesListCell* Curr = Move->head, *Prev = NULL;
	int rowCurr, colCurr;
	
	while (Curr)
	{
		if (Curr->captures > 0)
		//if there is capture to update board, prev won't be null-capture>0 from second cell at the list
		//delete the player from square of capture
			deleteFromBoard(Prev, Curr, board);

		Prev = Curr;
		Curr = Curr->next;
	}

	//update the start position
	rowCurr = (int)(Move->head->position->row - 'A');
	colCurr = (int)(Move->head->position->col - '0' - 1);
	board[rowCurr][colCurr] = ' ';

	//update the end position
	rowCurr = (int)(Move->tail->position->row - 'A');
	colCurr = (int)(Move->tail->position->col - '0' - 1);
	board[rowCurr][colCurr] = player;
}

static void deleteFromBoard(SingleSourceMovesListCell* Prev, SingleSourceMovesListCell* Curr, Board board)
{//gets a prev, curr cell and the board, updae the capture between two of them to ' '- make capture-eat
	//declaration of variables
	int rowPrev, colPrev, rowCurr, colCurr, rowDel, colDel;
	
	rowPrev = (int)(Prev->position->row - 'A');			//change from char to int
	colPrev = (int)(Prev->position->col - '0' - 1);		//change from char to int
	rowCurr = (int)(Curr->position->row - 'A');			//change from char to int
	colCurr = (int)(Curr->position->col - '0' - 1);		//change from char to int
	
	if (rowPrev - rowCurr < 0)	//move down
		rowDel = rowPrev + 1;	
	else						//move up
		rowDel = rowPrev - 1;

	if (colPrev - colCurr < 0)	//move right
		colDel = colPrev + 1;
	else						//move left
		colDel = colPrev - 1;

	board[rowDel][colDel] = ' ';	//"eat" the palyer
}

/////////////////////////// FREE FUNCTIONS //////////////////////////////////////////////////////

void freeMultipleMovesList(MultipleSourceMovesList* playerList)
{
	MultipleSourceMovesListCell* curr = playerList->head, *saver;

	while (curr)
	{
		saver = curr->next;
		freeSingleMoveList(curr->single_source_moves_list);
		free(curr);
		curr = saver;
	}
}

static void freeSingleMoveList(SingleSourceMovesList* playerList)
{
	SingleSourceMovesListCell* curr = playerList->head, *saver;

	while (curr)
	{
		saver = curr->next;
		free(curr->position);
		free(curr);
		curr = saver;
	}
}