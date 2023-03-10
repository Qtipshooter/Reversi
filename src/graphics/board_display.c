// board_display.c
// Quinton Graham
// Used to display the board and open the initial window
#include <SDL2/SDL.h>
#include <board_display.h>
#include <stdio.h>
#include <text_display.h>

// creates the screen window and puts the pointer to the screen window in the dest.
int create_window(SDL_Window **dest, SDL_Renderer **renderer)
{
	//verify that video was initialized, if not initialize it
	if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
	{
		//attempt to initalize video
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			printf("Couldn't initialize Video\n");
			return -1;
		}

		//video was able to initalize
	}

	*dest = SDL_CreateWindow("Reversi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (*dest == NULL)
	{
		printf("SDL: %s\n", SDL_GetError());
		return -2;
	}

	//create the renderer to use for drawing
	*renderer = SDL_CreateRenderer(*dest, -1, 0);

	if (*renderer == NULL)
	{
		printf("SDL: %s\n", SDL_GetError());
		return -3;
	}

	return 0;
}

//create the window for the game over screen
void create_game_over_window(SDL_Window **window)
{
	//create the window
	SDL_Window *game_over = SDL_CreateWindow("Game Over", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,\
		SCREEN_WIDTH - 200, SCREEN_HEIGHT - 270, SDL_WINDOW_SHOWN);

	//pass the window back to the variable placed
	*window = game_over;
}

//draw the game over window, which will display the score and the winner (or if there is a tie)
void draw_game_over_screen(SDL_Renderer **renderer, int player_1_score, int player_2_score)
{
	//set the background color and color to it
	SDL_SetRenderDrawColor(*renderer, 65, 165, 100, 255);
	SDL_RenderFillRect(*renderer, NULL);
	display_end_game_message(renderer, player_1_score, player_2_score);
	SDL_RenderPresent(*renderer);


}

//draw the board background and lines on the board
void draw_board(SDL_Renderer **renderer)
{

	//set the background color and color to it
	SDL_SetRenderDrawColor(*renderer, 65, 165, 100, 255);
	SDL_RenderFillRect(*renderer, NULL);

	//set the board color, and fill the rectangle that represents the board
	SDL_Rect board = {.x = 100, .y = 80, .w = 401,  .h = 401};
	SDL_SetRenderDrawColor(*renderer, 200, 170, 120, 255);
	SDL_RenderFillRect(*renderer, &board);

	//create then draw the grid
	//Grid requires 7137, 7200 gives a buffer of space
	SDL_Point *lines = malloc(7200 * sizeof(SDL_Point));
	SDL_Point *head = lines;
	int count_of_points = 0;

	//fill the points
	for(int x_loc = board.x; x_loc < board.x + board.w; x_loc++)
	{
		for(int y_loc = board.y; y_loc < board.y + board.h; y_loc++)
		{
			if ((x_loc - board.x) % 50 == 0 || (y_loc - board.y) % 50 == 0)
			{
				lines -> x = x_loc;
				lines -> y = y_loc;
				lines++;
				count_of_points++;
			}
		}
	}


	SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
	SDL_RenderDrawPoints(*renderer, head, count_of_points);
	SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
	SDL_RenderPresent(*renderer);

	free(head); //the space used to determine where to draw
}

//displays a reversi piece on the screen at the location requested
/**
 * @param renderer - the renderer to use to draw
 * @param x_pos - the left most pixel of the board tile
 * @param y_pos - the top most pixel of the board tile
 * @param player - the player the piece belongs to, as defined in the header
 */
void display_piece(SDL_Renderer **renderer, int x_pos, int y_pos, int player)
{

	//array of all the points to display
	SDL_Point *points = malloc(2000*sizeof(SDL_Point));
	SDL_Point *head = points; //used to free later
	int count_of_points = 0;

	//determine points that need drawn to
	for (int x_off = -25; x_off < 25; x_off++)
	{
		for (int y_off = -25; y_off < 25; y_off++)
		{
			//x squared + y squared = r squared.  anything inside of that will be less than r squared.
			// the -1 at the end is to prevent the circle from drawing over the lines created for the board.
			if (((x_off * x_off) + (y_off * y_off)) <= (25*25) - 1)
			{
				points -> x = x_off + x_pos + 25;
				points -> y = y_off + y_pos + 25;
				points++;
				count_of_points++;
			}
		}
	}

	//set the draw color to whichever player is playing
	if(player == BLACK)
	{
		SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
	}
	else if (player == WHITE)
	{
		SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(*renderer, 200, 170, 120, 255);
	}

	//draw the piece
	SDL_RenderDrawPoints(*renderer, head, count_of_points);
	SDL_RenderPresent(*renderer);

	//free the memory used for points
	free(head);
}

//function used to update the board to its current state
void update_board(SDL_Renderer **renderer, int board[8][8])
{

	//loops for finding each spot on the board
	for (int x = 0; x < 8; ++x)
	{
		//get the left most pixel of this block to draw
		int x_pos = x * 50 + LEFT_OFFSET;
		for (int y = 0; y < 8; ++y)
		{
			//get the top most pixel of this block to draw
			int y_pos = y * 50 + TOP_OFFSET;

			//belongs to player 1
			if (board[x][y] == BLACK) display_piece(renderer, x_pos, y_pos, BLACK);

			//belongs to player 2
			else if (board[x][y] == WHITE) display_piece(renderer, x_pos, y_pos, WHITE);

			//the tile is empty
			else display_piece(renderer, x_pos, y_pos, BOARD);
		}
	}
}