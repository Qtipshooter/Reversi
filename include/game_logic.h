// game_logic.h
// Quinton Graham
// Contains prototypes that help figure out the logic of the game

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

//macros for each direction and their corresponding values
#define TOP   -1
#define BOTTOM 1
#define LEFT  -1
#define RIGHT  1
#define CENTER 0

//capture and check mode for valid move function
#define CAPTURE 1
#define CHECK   0

//board size, used to show that the board is an 8 squares long in both directions
#define BOARD_SIZE 8

//player 1 is black, player 2 is white, empty is neither player
#define EMPTY    0
#define PLAYER_1 1
#define PLAYER_2 2

/**Sets the board variable given to the function to the default starting position
 * @param board - the variable to stroe the board layout in
 */
void set_board_beggining_pos(int board[8][8]);

/**
 * 
 */
int valid_move(SDL_Point pos, int board[8][8], int player, int capture);

/**
 *
 */
int valid_move_available(int board[8][8], int player);

/**Validates the position of a click as a place in the game board
 * @param x - the x position of the click
 * @param y - the y position of the click
 * @return - an SDL_Point with the cordinates of the location clicked in, or a NULL pointer if invalid
 */
SDL_Point * validate_position(int x, int y);

/**This function increases the offset used to determine the position of pieces
 * @param offset - the location in which to get information and replace information into
 */
void increment_offset(SDL_Point *offset);

/**This function decreases the offset used to determine the position of pieces
 * @param offset - the location in which to get information and replace information into
 */
void decrement_offset(SDL_Point *offset);

/**This function reads the board, and updates the score to its current state
 * @param player_1_score - the location to put player 1's score in
 * @param player_2_score - the location to put player 2's score in
 * @param board_total - the location to put the number of pieces total on the board in
 * @param board - the current layout of the board
 */
void update_score(int *player_1_score, int *player_2_score, int *board_total, int board[8][8]);


#endif