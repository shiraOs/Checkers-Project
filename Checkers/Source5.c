#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/
#include "Header3.h"

/***** Static Function Prototypes *****/
static void updateFile(FILE* f, char* line);

/******************* Function Implementation *******************/

void StoreBoard(Board board, char *filename)
{
	//declaration of variables
	int i;
	FILE* f = fopen(filename, "wb");		//open file in write mode by the name received
	
	if (!f)									//check if open ok
	{
		printf("Error in open file!\n");
		exit(1);
	}
	
	for (i = 0; i < BOARD_SIZE; i++)		//write in file every line in board
		updateFile(f, board[i]);

	fclose(f);								//close the file
}

static void updateFile(FILE* f, char* line)
{//Gets a file to write in, and a line to write in file
	//declaration of variables
	unsigned short res = 0, mask = 0x8000;		//mask with- 1 msb and other bits 0
	int i;

	for (i = 0; i < BOARD_SIZE; i++)			//run on the line
	{
		if (line[i] == 'B')						//if player B- write 10
		{
			res |= mask;						//put 10 in res- 1000...
			mask >>= 2;							//move mask by 2 to left- 0010...
		}
		else if (line[i] == 'T')				//if player T- write 01
		{
			mask >>= 1;							//move mask by 1 to left- 0100...
			res |= mask;						//put 01 in res- 0100...
			mask >>= 1;							//move mask by 1 to left- 0010...

		}
		else									//if empty- no player in square- write 00
			mask >>= 2;							//move mask by 2 to left- 0010...
	}
	fwrite(&res, sizeof(unsigned short), 1, f);		//write res to file
}