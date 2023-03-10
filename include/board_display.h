// board_display.h
// Quinton Graham
// holds function prototypes and sizings for the board

#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 600

//player 1 is black, player 2 is white, board is neither player
#define BOARD 0
#define BLACK 1
#define WHITE 2

//left is used for x positioning
//Top is used for y positioning
#define LEFT_OFFSET 100
#define TOP_OFFSET  80


//TODO make the sizings for the display overall and define them

/**
 *
 */
int create_window(SDL_Window **dest, SDL_Renderer **renderer);

/**
 *
 */
void create_game_over_window(SDL_Window **window);

/**
 *
 */
void draw_game_over_screen(SDL_Renderer **renderer, int player_1_score, int player_2_score);

/**
 *
 */
void draw_board(SDL_Renderer **renderer);

/**
 *
 */
void display_piece(SDL_Renderer **renderer, int x_pos, int y_pos, int player);

/**
 *
 */
void update_board(SDL_Renderer **renderer, int board[8][8]);

#endif