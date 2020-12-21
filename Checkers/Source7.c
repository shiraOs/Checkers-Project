#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header3.h"

/***** Static Function Prototypes *****/
static void printBoard(Board board);									//print the board
static void checkWin(Board board, Player starting_player, int* win);	//check if player got his last row

/******************* Function Implementation *******************/

void PlayGame(Board board, Player starting_player)
{
	//declaration of variables
	int win = 0;
	MultipleSourceMovesList* playerMoves = NULL;

	printBoard(board);

	while (!win)		//as long win=0, no one won , play game
	{
		playerMoves = FindAllPossiblePlayerMoves(board, starting_player);		//get a list of all moves

		if (playerMoves->head)								//if there is a list-there are moves
		{
			Turn(board, starting_player);					//update move on board and print the move
			checkWin(board, starting_player, &win);			//check if player got his last row

			if (starting_player == 'T')						//change for other player turn
				starting_player = 'B';
			else
				starting_player = 'T';
		}
		else						//no list- no more moves or no more players on board-end game
			win = 1;				//flag on- end loop

		freeMultipleMovesList(playerMoves);					//free memory of multiple moves list
	}

	if (starting_player == 'T')								//print after winning-like in the example
		printf("PLAYER_TOP_DOWN's turn\n");
	else
		printf("PLAYER_BOTTOM_UP's turn\n");
}

static void printBoard(Board board)
{//gets a board and print it
	//declaration of variables
	int i, j;
	char c = 'A';

	printf("+-+-+-+-+-+-+-+-+-+ \n");
	printf("+ |1|2|3|4|5|6|7|8| \n");
	printf("+-+-+-+-+-+-+-+-+-+ \n");

	for (i = 0; i < BOARD_SIZE; i++)
	{
		printf("|%c|", c);					//prints the row, A to H
		for (j = 0; j < BOARD_SIZE; j++)
			printf("%c|", board[i][j]);		//prints the board- T, B or white space for empty square
		c++;
		printf("\n");
		printf("+-+-+-+-+-+-+-+-+-+ \n");
	}
}

static void checkWin(Board board, Player starting_player, int* win)
{//Gets an updated bord-after turn, the player who moved and flag of win by reference.
 //Check if player got his last row and won or not. update flag win as needed.
	//declaration of variable
	int i;

	if (starting_player == 'T')						//if the player is T- last row for winning
	{
		for (i = 0; i < BOARD_SIZE; i++)
			if (board[BOARD_SIZE - 1][i] == 'T')	//there is a T player in last row
				*win = 1;							//T won, change flag to 1
	}

	if (starting_player == 'B')					//if the player is B- first row for winning
	{	
		for (i = 0; i < BOARD_SIZE; i++)
			if (board[0][i] == 'B')				//there is a B player in first row
				*win = 1;						//B won, change flag to 1
	}
}