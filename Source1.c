#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header1.h"

/***** Static Function Prototypes *****/

//buile recursively the moves tree according to board
static SingleSourceMovesTreeNode* FindSingleSourceMovesRec(Board board, int row, int col, unsigned char c, int totalCapture);
static void copyBoard(Board board_to_fill, Board board);		//copy recived board for new tree node
static SingleSourceMovesTreeNode* createTreeNode(Board board, unsigned short total_captures_so_far, checkersPos* pos);	//return new tree node
static int CheckValidRight(Board board, int row, int col);		//check for right move
static int CheckValidLeft(Board board, int row, int col);		//check for left move
static int checkLeftMoveT(Board board, int row, int col);		//check for T-left move
static int checkRightMoveT(Board board, int row, int col);		//check for T-right move
static int checkLeftMoveB(Board board, int row, int col);		//check for B-left move
static int checkRightMoveB(Board board, int row, int col);		//check for B-left move

/******************* Function Implementation *******************/

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src)
{//gets board and position, build a tree of moves from this position and return the tree, return NULL if no possible moves
	//declaration of variables
	SingleSourceMovesTree* res;
	int row, col;

	row = (src->row - 'A');				//changing to int num
	col = (src->col - '0' - 1);			//changing to int num

	if (row < 0 || row > (BOARD_SIZE-1) || col < 0 || col > (BOARD_SIZE-1))	//check if the position incorrect- return NULL
		return NULL;

	if (board[row][col] == ' ')			//check if there isn't a player- return NULL
		return NULL;

	if (!CheckValidLeft(board, row, col) && !CheckValidRight(board, row, col))
	//check if the player has no where to go- return NULL
		return NULL;

	res = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	checkAllocation(res);

	//there is left or right move-create tree and return it
	res->source = FindSingleSourceMovesRec(board, row, col, board[row][col], 0);
	return res;
}

static SingleSourceMovesTreeNode* FindSingleSourceMovesRec(Board board, int row, int col, unsigned char c, int totalCapture)
{//check the flag for move, val_r and val_l: 0-no where to move-do nothing, 1-regular move-add move to tree,
	//2-capture move-call recursivly to check place after capture
	//declaration of variables
	SingleSourceMovesTreeNode* res, *resRight = NULL, *resLeft = NULL;
	int val_r, val_l;
	res = createTreeNode(board, totalCapture, createCheckersPos(row, col));		//create the root of current place on board
	
	if (c == 'T')
	{//check for valid left and right moves for T
		val_r = checkRightMoveT(board, row, col);
		val_l = checkLeftMoveT(board, row, col);

		if (val_l == 2)						//check if left move is capture
			resLeft = FindSingleSourceMovesRec(board, row + 2, col - 2, c, totalCapture + 1);
		else if (val_l == 1 && res->total_captures_so_far == 0)		//left move is regular and no capture before
			resLeft = createTreeNode(board, totalCapture, createCheckersPos(row + 1, col - 1));

		if (val_r == 2)						//check if right move is capture
			resRight = FindSingleSourceMovesRec(board, row + 2, col + 2, c, totalCapture + 1);	//move down
		else if (val_r == 1 && res->total_captures_so_far == 0)		//right move is regular and so far no capture
			resRight = createTreeNode(board, totalCapture, createCheckersPos(row + 1, col + 1));
	}

	if (c == 'B')
	{//check for valid left and right moves for B
		val_r = checkRightMoveB(board, row, col);
		val_l = checkLeftMoveB(board, row, col);

		if (val_l == 2)			//check if left move is capture
			resLeft = FindSingleSourceMovesRec(board, row - 2, col - 2, c, totalCapture + 1);	//move up
		else if (val_l == 1 && res->total_captures_so_far == 0)		//left move is regular and no capture before
			resLeft = createTreeNode(board, totalCapture, createCheckersPos(row - 1, col - 1));

		if (val_r == 2)			//check if right move is capture
			resRight = FindSingleSourceMovesRec(board, row - 2, col + 2, c, totalCapture + 1);	//move up
		else if (val_r == 1 && res->total_captures_so_far == 0)		//right move is regular and so far no capture
			resRight = createTreeNode(board, totalCapture, createCheckersPos(row - 1, col + 1));
	}

	res->nextMove[0] = resLeft;					//put the left sub tree to the root
	res->nextMove[1] = resRight;				//put the right sub tree to the root
	return res;									//returns the root of the tree
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

static SingleSourceMovesTreeNode* createTreeNode(Board board, unsigned short total_captures_so_far, checkersPos* pos)
{//create tree node
	//declaration of variable
	SingleSourceMovesTreeNode* res = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkAllocation(res);

	copyBoard(res->board, board);							//copies the given board to the board in the tree node
	res->nextMove[0] = res->nextMove[1] = NULL;				//put null in the next moves
	res->total_captures_so_far = total_captures_so_far;		//fill by the given num of total capture so far.
	res->pos = pos;											//fill by the given checkersPos.

	return res;
}

static void copyBoard(Board board_to_fill, Board board) //copy board
{//Gets a board and copy it to a new one, update by reference
	//declaration of variables
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
			board_to_fill[i][j] = board[i][j];
	}
}

/////////////////////////////////// CHECK T MOVES //////////////////////////////////////////////////////
//The functions gets board and position on board, returning  0-no where to move, 1-regular move, 2-capture move//

static int checkLeftMoveT(Board board, int row, int col)
{//check for moves to the left
	if (col == 0 || row == (BOARD_SIZE-1))		//If its the left col or the last row no where to move
		return 0;
	else if (board[row + 1][col - 1] == ' ')	//regular move to the left is available
		return 1;
	else if ((col > 1) && (row < (BOARD_SIZE-2)) && board[row + 1][col - 1] == 'B' && 
		board[row + 2][col - 2] == ' ')			//check its not the edge of board and can move to eat
		return 2;								//capture is possible to the left
	
	return 0;
}

static int checkRightMoveT(Board board, int row, int col)
{//check for moves to the right
	if (col == (BOARD_SIZE-1) || row == (BOARD_SIZE-1))			//If its the right col or the last row no where to move
		return 0;
	else if (board[row + 1][col + 1] == ' ')	//regular move to the right is available
		return 1;
	else if ((col < (BOARD_SIZE-2)) && (row < (BOARD_SIZE-2)) && board[row + 1][col + 1] == 'B' && 
		board[row + 2][col + 2] == ' ')			//check its not the edge of board and can move to eat
		return 2;								//capture is possible to the right

	return 0;
}

/////////////////////////////////// CHECK B MOVES //////////////////////////////////////////////////////
//The functions gets board and position on board, returning  0-no where to move, 1-regular move, 2-capture move//

static int checkLeftMoveB(Board board, int row, int col)
{//check for moves to the left
	if (col == 0 || row == 0)					//If its the left col or the first row no where to move
		return 0;
	else if (board[row - 1][col - 1] == ' ')	//regular move to the left is available 
		return 1;
	else if ((col > 1) && (row > 1) && board[row - 1][col - 1] == 'T' && 
		board[row - 2][col - 2] == ' ')			//check its not the edge of board and can move to eat
		return 2;								//capture is possible to the left

	return 0;
}

static int checkRightMoveB(Board board, int row, int col)
{//check for moves to the right
	if (col == (BOARD_SIZE-1) || row == 0)		//If its the right col or the first row no where to move
		return 0;
	else if (board[row - 1][col + 1] == ' ')	//regular move to the right is available 
		return 1;
	else if ((col < (BOARD_SIZE-2)) && (row > 1) && board[row - 1][col + 1] == 'T' && 
		board[row - 2][col + 2] == ' ')			//check its not the edge of board and can move to eat
		return 2;								//capture is possible to the right

	return 0;
}

/////////////////////////////////////MAIN CHACK FOR MOVES////////////////////////////////////////
//The functions gets board and position on board, returning  0-no where to move, 1-regular move, 2-capture move//

static int CheckValidLeft(Board board, int row, int col)
{//check for moves to the left
	if (board[row][col] == 'B')
		return (checkLeftMoveB(board, row, col));		//check only left move for B
	else
		return (checkLeftMoveT(board, row, col));		//check only left move for T
}

static int CheckValidRight(Board board, int row, int col)
{//check for moves to the right
	if (board[row][col] == 'B')
		return (checkRightMoveB(board, row, col));		//check only right move for B
	else
		return (checkRightMoveT(board, row, col));		//check only right move for T
}

/////////////////////////////////////////////////////////////////////////////////////////////

void buildBoard(Board board)
{//get empty board and fill it by reference for a starting board
	//declaration of variables
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++)
	{//check for even row
		if (i % 2 == 0)
		{
			for (j = 1; j < BOARD_SIZE; j = j + 2)//puts the game players
			{
				if (i < 3)		//put T on board
					(board[i][j]) = 'T';
				else if (i > (BOARD_SIZE-4))	//put B on board
					(board[i][j]) = 'B';
				else			//no players - two middle rows
					board[i][j] = ' ';
			}

			for (j = 0; j < BOARD_SIZE; j = j + 2) //keep empty-white squares
				board[i][j] = ' ';
		}
		else
		{
			for (j = 0; j < BOARD_SIZE; j = j + 2)	//puts the game players
			{
				if (i < 3)		//put T on board
					(board[i][j]) = 'T';
				else if (i > (BOARD_SIZE-4))	//put B on board
					(board[i][j]) = 'B';
				else			//no players - two middle rows
					board[i][j] = ' ';
			}

			for (j = 1; j < BOARD_SIZE; j = j + 2)	//keep empty-white squares
				board[i][j] = ' ';
		}
	}
}

checkersPos* createCheckersPos(int row, int col)
{//Gets int num of row and column, return the checkersPos according to the col and row received
	//declaration of variable
	checkersPos* res = (checkersPos*)malloc(sizeof(checkersPos));
	checkAllocation(res);

	res->col = col + '0' + 1;			//convert the int num to char
	res->row = 'A' + row;				//convert the int num to char

	return res;
}

void checkAllocation(void* ptr)
{//gets a pointer and check if the memory allocation was ok else exit by -1
	if (!ptr)
	{
		printf("Memory allocation error!\n");
		exit(-1);
	}
}
