// game_logic.c
// Quinton Graham
// Contains various functions that help figure out the logic of the game
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <game_logic.h>

//function to set the board to the beggining state for playing the game
void set_board_beggining_pos(int board[8][8])
{

	//loop through the board layout and make all positions 0
	for (int x = 0; x < BOARD_SIZE; ++x)
	{
		for (int y = 0; y < BOARD_SIZE; ++y)
		{
			board[x][y] = EMPTY;
		}
	}

	//set the starting positions for the pieces
	board[3][3] = PLAYER_1;
	board[4][4] = PLAYER_1;
	board[4][3] = PLAYER_2;
	board[3][4] = PLAYER_2;
}

//returns negative if there is a piece there, and the number of pieces captured otherwise
//0 captures is an invalid move
int valid_move(SDL_Point pos, int board[8][8], int player, int capture)
{
	//variables used
	int opposing_player;
	int pieces_captured = 0;

	//get opposing player as well to use here
	if (player == PLAYER_1) opposing_player = PLAYER_2;
	else opposing_player = PLAYER_1;

	//check to make sure that the tile is not already occupied by a player
	if(board[pos.x][pos.y] == PLAYER_1 || board[pos.x][pos.y] == PLAYER_2) return -1;

	//going in order from left to right, top to bottom, check all directions for capture
	for (int direction = 0; direction < 8; direction++)
	{
		//set the offset direction to check for this iteration
		SDL_Point offset;
		switch(direction)
		{
			case 0:
				offset.x = LEFT;
				offset.y = TOP;
				break;
			case 1:
				offset.x = CENTER;
				offset.y = TOP;
				break;
			case 2:
				offset.x = RIGHT;
				offset.y = TOP;
				break;
			case 3:
				offset.x = LEFT;
				offset.y = CENTER;
				break;
			case 4:
				offset.x = RIGHT;
				offset.y = CENTER;
				break;
			case 5:
				offset.x = LEFT;
				offset.y = BOTTOM;
				break;
			case 6:
				offset.x = CENTER;
				offset.y = BOTTOM;
				break;
			case 7:
				offset.x = RIGHT;
				offset.y = BOTTOM;
				break;
			default:
				offset.x = CENTER;
				offset.y = CENTER;
		}

		//cycle through all pieces in current direction until not opposion player's piece
		while(board[pos.x + offset.x][pos.y + offset.y] == opposing_player\
			&& pos.x + offset.x >= 0 && pos.x + offset.x < BOARD_SIZE \
			&& pos.y + offset.y >= 0 && pos.y + offset.y < BOARD_SIZE) \
			increment_offset(&offset);

		//terminating piece is either current player or blank tile
		//if terminating piece is current player, capture all pieces
		if (board[pos.x + offset.x][pos.y + offset.y] == player)
		{
			//decrement back to 
			decrement_offset(&offset);

			//this will cycle in reverse from before
			while(board[pos.x + offset.x][pos.y + offset.y] == opposing_player)
			{
				//capture pieces if the signal is set to capture
				if(capture) board[pos.x + offset.x][pos.y + offset.y] = player;
				pieces_captured++;
				decrement_offset(&offset);
			}
		}
	}

	//return the number captured to prove more than zero were captured.
	return pieces_captured;
}

//check if conditions say that the game is over
int valid_move_available(int board[8][8], int player)
{
	//store opposing player aswell
	int opposing_player;
	if (player == PLAYER_1) opposing_player = PLAYER_2;
	else opposing_player = PLAYER_1;
	SDL_Point offset;

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			//get the current position
			SDL_Point pos = {.x = x, .y = y};

			//check if there is a piece, if so, skip this iteration
			if(board[pos.x][pos.y] == PLAYER_1 || board[pos.x][pos.y] == PLAYER_2) continue;

			for (int direction = 0; direction < 8; direction++)
			{
				//set the offset direction to check for this iteration
				switch(direction)
				{
					case 0:
						offset.x = LEFT;
						offset.y = TOP;
						break;
					case 1:
						offset.x = CENTER;
						offset.y = TOP;
						break;
					case 2:
						offset.x = RIGHT;
						offset.y = TOP;
						break;
					case 3:
						offset.x = LEFT;
						offset.y = CENTER;
						break;
					case 4:
						offset.x = RIGHT;
						offset.y = CENTER;
						break;
					case 5:
						offset.x = LEFT;
						offset.y = BOTTOM;
						break;
					case 6:
						offset.x = CENTER;
						offset.y = BOTTOM;
						break;
					case 7:
						offset.x = RIGHT;
						offset.y = BOTTOM;
						break;
					default:
						offset.x = CENTER;
						offset.y = CENTER;
				}

				//HACK I am doing this to check if the piece found is right beside the current piece
				SDL_Point adjecent;
				adjecent.x = offset.x;
				adjecent.y = offset.y;
		
				//cycle through all pieces in current direction until not opposion player's piece
				while(board[pos.x + offset.x][pos.y + offset.y] == opposing_player\
					&& pos.x + offset.x >= 0 && pos.x + offset.x < BOARD_SIZE \
					&& pos.y + offset.y >= 0 && pos.y + offset.y < BOARD_SIZE) \
					increment_offset(&offset);
				
				//terminating piece is either current player or blank tile
				//if terminating piece is current player, there is a valid move
				if (board[pos.x + offset.x][pos.y + offset.y] == player && (offset.x != adjecent.x || offset.y != adjecent.y))
				{
					return 1;
				}
			} /* END DIRECTION LOOP */
		} /* END Y LOOP */
	} /* END X LOOP */

	//if this position is reached, there are no valid moves for this player
	return 0;
}

//function to detect if a point is in the board, and get a point to the top left of that spot
SDL_Point * validate_position(int x, int y)
{
	//trim the inputs to know the top left of the board as 0, 0
	x = x - 100;
	y = y - 80;

	//if either is negative, its automatically outside the board
	//if either is over 399, its automatically outside the board
	if (x < 0 || y < 0 || x > 399 || y > 399) {printf("Point is invalid\n");return NULL;}

	//point is a valid point, now figure out which point the click is in
	int x_tile, y_tile;

	//integers round towards 0, so they will get the position I need (0 - 7)
	x_tile = x / 50; //SPACE SIZE
	y_tile = y / 50;

	//return the position on the board that the piece goes
	SDL_Point *point = malloc(sizeof(SDL_Point));
	point -> x = x_tile;
	point -> y = y_tile;

	//return the point
	return point;
}


//Used to increment the offset to the next position in a linear position
//This is in a function in order to keep the code its used for cleaner
void increment_offset(SDL_Point *offset)
{
	if 		(offset -> x < 0) 	offset -> x -= 1; //x is negative, Subtract 1
	else if (offset -> x > 0) 	offset -> x += 1; //x is positive, Add 1
	if 		(offset -> y < 0) 	offset -> y -= 1; //y is negative, Subtract 1
	else if (offset -> y > 0) 	offset -> y += 1; //y is positive, Add 1
}

void decrement_offset(SDL_Point *offset)
{
	if 		(offset -> x < 0) 	offset -> x += 1; //x is negative, Add 1
	else if (offset -> x > 0) 	offset -> x -= 1; //x is positive, Subtract 1
	if 		(offset -> y < 0) 	offset -> y += 1; //y is negative, Add 1
	else if (offset -> y > 0) 	offset -> y -= 1; //y is positive, Subtract 1
}

//used to update the game score each tick
void update_score(int *player_1_score, int *player_2_score, int *board_total, int board[8][8])
{
	//set the counters for each to zero
	int count_1 = 0, count_2 = 0;

	//loop through each tile of the board
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			//count each instance of each player's pieces
			if (board[x][y] == PLAYER_1) count_1++;
			else if(board[x][y] == PLAYER_2) count_2++;
		}
	}

	//update the counters
	*player_1_score = count_1;
	*player_2_score = count_2;
	*board_total = count_1 + count_2;
}