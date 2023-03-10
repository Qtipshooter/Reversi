// reversi.c
// Quinton Graham
//
#include <stdio.h>
#define  SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <board_display.h>
#include <game_logic.h>
#include <text_display.h>

// GLOBAL VARIABLES (mostly global for ease of focus and programming, they aren't used outside of the scope of main)
int game_on = 1;					// 1 for true, 0 for false
int current_player = PLAYER_1;		// Player 1 goes first, then this is alternated if the move is proved valid and the piece is placed
int opposing_player = PLAYER_2;		// Stores the alternate player (used for flipping tiles)
int player_1_score = 0;				// Tracks the number of pieces player 1 has on the board
int player_2_score = 0;				// Tracks the number of pieces player 2 has on the board
int pieces_on_board = 0;			// Tracks total pieces on board, when at 64 the game ends
int board[8][8];					// Tracks the board in itself

//Function used to alternate player in one line, just easier on me
void alternate_player();


int main(int argc, char *argv[])
{
	//variable initialization
	SDL_Window *game_screen = NULL;
	SDL_Window *game_over_screen = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Renderer *game_over_renderer = NULL;
	SDL_Event *event = malloc(sizeof(SDL_Event));

	//SDL Subsystem initialization
	//Video is needed to display the graphics and use the renderer
	//Events is needed to read clicks and read the exit signal
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		printf("%s: %s\n", "Couldn't initialize SDL Video and Events", SDL_GetError());
		exit(-1);
	}

	//attempt to draw the game board
	if(create_window(&game_screen, &renderer) != 0)
	{
		printf("%s\n", "Could not create window or renderer");
		exit(-1);
	}

//used for play again after the game ends
game_startup:

	//draw the board and background
	draw_board(&renderer);

	//set the board to the initial position, set the current player, and update the score
	set_board_beggining_pos(board);
	current_player = PLAYER_1;
	opposing_player = PLAYER_2;
	update_score(&player_1_score, &player_2_score, &pieces_on_board, board);

	//make sure that game is set to on
	game_on = 1;
	
	//main game loop
	while(game_on)
	{
		//events loops that handles all events while the game is running
		while(SDL_PollEvent(event))
		{
			switch(event -> type)
			{
				//Game Close event
				case SDL_QUIT:
					exit(0);
					break;

				//mouse clicked (then adjust to left click)
				case SDL_MOUSEBUTTONDOWN:
					if (event -> button.button == SDL_BUTTON_LEFT)
					{

						//check that the move is a valid position on the board, if so, move will have the cordinates of the space
						SDL_Point *move = validate_position(event -> button.x, event -> button.y);

						//if the move is invalid, break the switch and go to next iteration
						if(move == NULL)
						{
							printf("Invalid Move\n");
							break;
						}

						int num_captured;
						//check if there were pieces that were captured (captures pieces if there were any)
						if((num_captured = valid_move(*move, board, current_player, CAPTURE)) <= 0)
						{
							printf("Invalid Move\n");
							break;
						}

						//display how many pieces were captured
						printf("Player %d captured %d pieces!\n", current_player, num_captured);

						//place current piece also
						board[move -> x][move -> y] = current_player;

						//update score to be used for check
						update_score(&player_1_score, &player_2_score, &pieces_on_board, board);

						//if board is full, game is over
						if (pieces_on_board == 64)
						{
							printf("Board is full: Game Over\n");
							update_board(&renderer, board);
							goto game_over;
						}

						//switch to next player
						alternate_player();

						//check if the new player has available moves, if not, display such
						if (!valid_move_available(board, current_player))
						{
							printf("No moves available for player %d, Player %d's turn\n", current_player, opposing_player);

							//if the current player has no available moves, swap back to first player again
							alternate_player();

							//if there are still no valid moves, the game is over
							if (!valid_move_available(board, current_player))
							{
								printf("No valid moves left for either player, game over\n");
								update_board(&renderer, board);
								goto game_over;
							}
						}


						//free the memory used for the move point
						free(move);
					}
					break; /* End MOUSE PRESS Case */

				//keyboard button pressed
					//R -> reset game board
					//E -> end game
					//U -> undo **If I implement this
					//X -> Fix graphics(redraw board)
					//esc -> end game
				case SDL_KEYDOWN:

					// Switch on the key that was pressed for different functions
					switch(event -> key.keysym.sym)
					{
						//e key is pressed
						case SDLK_e:
							//end game button
							goto game_over;

						//r key is pressed
						case SDLK_r:
							//restart game button
							printf("Reseting Game. . .\n");
							goto game_startup;

						//x key is pressed
						case SDLK_x:
							//draw the board again so that it all displays
							//this is used to fix graphics
							draw_board(&renderer);
							break;
						default:
							break;

					} /* End KEYBOARD KEY switch */

					break; /* End KEY PRESS Case */
				default:
					break;



			} /* End SWITCH on event type */
		} /* End EVENT loop */

		//update board display
		update_board(&renderer, board);
	} /* End GAME ON loop */


//label used to jump out of the game as it is being played
game_over:

	//Display game over screen
	create_game_over_window(&game_over_screen);

	//create renderer for game over screen
	game_over_renderer = SDL_CreateRenderer(game_over_screen, -1, 0);

	//draw the screen with the renderer
	draw_game_over_screen(&game_over_renderer, player_1_score, player_2_score);

	//game over, display screen
	while(1) //option to escape will be provided
	{
		while(SDL_PollEvent(event))
		{
			if (event -> type == SDL_KEYDOWN)
			{
				//game was selected to be restarted.
				if(event -> key.keysym.sym == SDLK_r)
				{
					SDL_DestroyWindow(game_over_screen);
					goto game_startup;
				}
				else
				{
					//end game functions
					SDL_DestroyWindow(game_over_screen);
					SDL_DestroyWindow(game_screen);
					SDL_Quit();

					//ended successfully
					exit(0);
				}
			} /* END IF KEYDOWN */
		} /* END EVENT LOOP */
	} /* END ENDGAME WHILE LOOP */
}

void alternate_player()
{
	if (current_player == PLAYER_1) 
	{
		current_player = PLAYER_2;
		opposing_player = PLAYER_1;
	}
	else 
	{
		current_player = PLAYER_1;
		opposing_player = PLAYER_2;
	}
}

