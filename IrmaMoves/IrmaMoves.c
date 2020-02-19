//  Nicholas Asbaty | 5591

#include "IrmaMoves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//Function implementation

//Converts a given char to corresponding integer
int convCharToInt(char col)
{
    int x = 0;

    switch(col)
    {
        case 'a': x = 0; break;
        case 'b': x = 1; break;
        case 'c': x = 2; break;
        case 'd': x = 3; break;
        case 'e': x = 4; break;
        case 'f': x = 5; break;
        case 'g': x = 6; break;
        case 'h': x = 7; break;
        default: return -1;
    }

    return x;
}

//Converts a given integer to corresponding char
char convIntToChar(int i)
{
    switch(i)
    {
        case 0: return 'a';
        case 1: return 'b';
        case 2: return 'c';
        case 3: return 'd';
        case 4: return 'e';
        case 5: return 'f';
        case 6: return 'g';
        case 7: return 'h';
        default: return 'n';
    }
    return 'n';
}

//Dynamically allocates the board
char **createMapBoard(void)
{
	char **board = NULL; //declares the board
	board = malloc(sizeof(char*) * 8); //allocates the rows
	if (board == NULL)
			printf("Initial malloc failed to allocate memory");
	for (int i = 0; i < 8; i++)
			board[i] = malloc(sizeof(char) * 9); //fills the rows with memory

    //creating the board
	strncpy(board[0],"FF      ", 9);
	strncpy(board[1]," F      ", 9);
	strncpy(board[2]," FF     ", 9);
	strncpy(board[3],"  F     ", 9);
	strncpy(board[4],"  K     ", 9);
	strncpy(board[5],"C  B    ", 9);
	strncpy(board[6]," CC D   ", 9);
	strncpy(board[7],"  C  DD ", 9);

	return board; //returns the board
}

//frees the board from memory
char **destroyMapBoard(char **board)
{
	for (int i = 0; i < 8; i++)
	{
		free(board[i]); //free the memory in each row
	}
	free(board); //frees the board
	return NULL;
}

//prints a given board
void printMapBoard(char **board)
{
	printf("========\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%s\n", board[i]); //prints row i
	}
	printf("========\n");
	printf("\n");
}

//calculates irma's wind gust and speed and updates irma's position with the parameters in the irma struct
char **predictIrmaChange (char* str, Move *irmaMove)
{
    char **board = createMapBoard(); //creates a board
    parseNotationString(str, irmaMove); //parses the algebraic string

    //places irma on the map
    int c = convCharToInt(irmaMove->current_loc.col);
    int r = (irmaMove->current_loc.row);
    board[r][c] = 'I';
    printMapBoard(board);

    //setting flags to determine the direction irma is moving
    int currCol, toCol, colFlag, rowFlag, x, y;
    currCol = convCharToInt(irmaMove->current_loc.col);
    toCol = convCharToInt(irmaMove->to_loc.col);

    x = toCol-currCol;
    y = irmaMove->to_loc.row - irmaMove->current_loc.row;

    if(x < 0)
        colFlag = 0; //moving left
    else
        colFlag = 1; //moving right

    if(y < 0)
        rowFlag = 0; //moving up
    else
        rowFlag = 1; //moving down



    int temp = irmaMove->current_loc.col - 'a'; //variable to help change irma's column
    while(irmaMove->current_loc.col != irmaMove->to_loc.col)
    {
        if(colFlag == 0)
        {
            if(board[irmaMove->current_loc.row][convCharToInt(irmaMove->current_loc.col)-1] == ' ')
            {
                irmaMove->irma.ws += 10; //changing windspeed
                irmaMove->irma.wg += 5;  //changing windgust
            }
            if(board[irmaMove->current_loc.row][convCharToInt(irmaMove->current_loc.col)-1] != ' ')
            {
                irmaMove->irma.ws -= 15; //changing windspeed
                irmaMove->irma.wg -= 10; //changing windgust
            }

            temp -= 1;


        }

        if(colFlag == 1)
        {
            if(board[irmaMove->current_loc.row][convCharToInt(irmaMove->current_loc.col)+1] == ' ')
            {
                irmaMove->irma.ws += 10; //changing windspeed
                irmaMove->irma.wg += 5; //changing windgust
            }

            if(board[irmaMove->current_loc.row][convCharToInt(irmaMove->current_loc.col)+1] != ' ')
            {
                irmaMove->irma.ws -= 15; //changing windspeed
                irmaMove->irma.wg -= 10; //changing windgust
            }

            temp += 1;
        }
        char tempC = convIntToChar(temp);
        irmaMove->current_loc.col = tempC; //updating the column
    }


    int temp2 = irmaMove->current_loc.row; //variable to help change irma's row
    while(irmaMove->current_loc.row != irmaMove->to_loc.row)
    {
        if(rowFlag == 0)
        {
            if(board[irmaMove->current_loc.row - 1][convCharToInt(irmaMove->current_loc.col)] == ' ')
            {
               irmaMove->irma.ws += 6; //changing windspeed
               irmaMove->irma.wg += 3; //changing windgust
            }

            if(board[irmaMove->current_loc.row - 1][convCharToInt(irmaMove->current_loc.col)] != ' ')
            {
               irmaMove->irma.ws -= 2; //changing windspeed
               irmaMove->irma.wg -= 1; //changing windgust
            }
            temp2 -= 1;
        }

        if(rowFlag == 1)
        {
            if(board[irmaMove->current_loc.row + 1][convCharToInt(irmaMove->current_loc.col)] == ' ')
            {
               irmaMove->irma.ws += 6; //changing windspeed
               irmaMove->irma.wg += 3; //changing windgust
            }

            if(board[irmaMove->current_loc.row + 1][convCharToInt(irmaMove->current_loc.col)] != ' ')
            {
               irmaMove->irma.ws -= 2; //changing windspeed
               irmaMove->irma.wg -= 1; //changing windgust
            }
            temp2 += 1;

        }
        irmaMove->current_loc.row = temp2; //updating the row
    }

    //updating the final map
    c = convCharToInt(irmaMove->current_loc.col);
    r = (irmaMove->current_loc.row);
    board = createMapBoard();
    board[r][c] = 'I';
    printMapBoard(board);
    return board;
}

void parseNotationString(char *str, Move *Irma)
{
    //initializing irma
    Irma->from_loc.col = 'x';
	Irma->from_loc.row = -1;
	Irma->current_loc.col = 'x';
	Irma->current_loc.row = -1;
	Irma->to_loc.col = 'x';
	Irma->to_loc.row = -1;


	char *stringCopy = malloc(sizeof(char) * strlen(str));
    strcpy(stringCopy, str);

    //using strtok() to seperate the given string into its four parts
	char *start = strtok(stringCopy, " ");
	char *speed = strtok(NULL, " ");
	char *gust = strtok(NULL, " ");
	char *end = strtok(NULL, " ");

	//assigning irma with given parameters
	Irma->from_loc.col = start[0];
	Irma->from_loc.row = (start[1] - '0');
	Irma->irma.ws = atoi(speed);
	Irma->irma.wg = atoi(gust);
	Irma->to_loc.col = end[0];
	Irma->to_loc.row = (end[1]- '0');
	Irma->current_loc.col = start[0];
	Irma->current_loc.row = (start[1] - '0');
}

double difficultyRating(void)
{
    return 3.0;
}

double hoursSpent(void)
{
	return 20.0;
}

