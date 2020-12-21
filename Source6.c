#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header3.h"

/***** Static Function Prototypes *****/
static void updateBoard(unsigned char* row_in_board, unsigned short row_to_write);

/******************* Function Implementation *******************/

void LoadBoard(char* filename, Board board)
{
	//declaration of variables
	int i;
	unsigned short boardRow;					//read from file each line ob board- 16 bits- 2 bytes

	FILE* f = fopen(filename, "rb");			//open file in read mode by the name received

	if (!f)										//check if open ok
	{
		printf("Error in open file!\n");
		exit(1);
	}

	for (i = 0; i < BOARD_SIZE; i++)
	{//read from file board size of lines in board-16 bits, 2 bytes
		fread(&boardRow, sizeof(unsigned short), 1, f);					//read from file
		updateBoard(board[i], boardRow);								//update board
	}

	fclose(f);									//close the file
}

static void updateBoard(unsigned char* row_in_board, unsigned short row_to_write)
{//Gets a row from board and row from file to fill the board.
	//declaration of variables
	unsigned short maskT = 0x4000, maskB = 0x8000, maskEmpty = 0xC000; //T-0100... , B-1000... , empty-1100...
	int i;

	for (i = 0; i < BOARD_SIZE; i++)
	{//Check every two left bits in row_to_write, they can be 00, 10 or 01.
		if ((maskEmpty & row_to_write) == 0)	//no player
			row_in_board[i] = ' ';
		else if ((maskEmpty & row_to_write) == maskT)	//player T
			row_in_board[i] = 'T';
		else if ((maskEmpty & row_to_write) == maskB)	//player B
			row_in_board[i] = 'B';

		row_to_write <<= 2;
	}
}