// text_display.c
// Quinton Graham
// uses functions to render text as needed for the program
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <text_display.h>

void display_end_game_message(SDL_Renderer **renderer, int player_1_score, int player_2_score)
{
	//initialize TTF
	if (TTF_Init() < 0)
	{
		printf("TTF could not be initialized: %s\n", SDL_GetError());
		return;
	}

	//variables used
	char *winner;								// The string that holds the message displaying who wins
	TTF_Font *font;								// Variable that holds the font of the text
	char *score = malloc(sizeof(char) * 8);		// The string that has the score of the players total
	char *restart_prompt = "R to Restart!";		// The string to display how to restart game
	char *exit_prompt = "Any Key to Exit";		// The string to display how to exit
	int font_w = 0;								// The font size for the x plane
	int font_h = 0;								// The font size for the y plane
	SDL_Color black = {.r=0,.g=0,.b=0,.a=255};	// Black to be the text color
	SDL_Surface *text;							// Used to store the text surface
	SDL_Texture *text_txt;						// Texture to renderer the text
	SDL_Rect *dst = malloc(sizeof(SDL_Rect));	// Rectangle destination for the text

	//determine winner
	if (player_1_score > player_2_score) winner =      "Player 1 Wins!";
	else if (player_1_score < player_2_score) winner = "Player 2 Wins!";
	else winner = "The Game is a Tie!";

	//determine score
	sprintf(score, "%02d : %02d", player_1_score, player_2_score);

	//open up the font itself to use
	font = TTF_OpenFont("font/unispace.ttf", 28);

	//DISPLAY WINNER BLOCK
	text = TTF_RenderText_Solid(font, winner, black);
	text_txt = SDL_CreateTextureFromSurface(*renderer, text);
	//get the text's dimensions
	TTF_SizeText(font, winner, &font_w, &font_h);
	//create the destination rectangle
	dst -> x = 200 - font_w/2;
	dst -> y = 25;
	dst -> w = font_w;
	dst -> h = font_h;

	SDL_RenderCopy(*renderer, text_txt, NULL, dst);

	//DISPLAY SCORE BLOCK
	text = TTF_RenderText_Solid(font, score, black);
	text_txt = SDL_CreateTextureFromSurface(*renderer, text);
	//get the text's dimensions
	TTF_SizeText(font, score, &font_w, &font_h);
	//create the destination rectangle
	dst -> x = 200 - font_w/2;
	dst -> y = 100;
	dst -> w = font_w;
	dst -> h = font_h;
	SDL_RenderCopy(*renderer, text_txt, NULL, dst);

	//DISPLAY RESTART BLOCK
	text = TTF_RenderText_Solid(font, restart_prompt, black);
	text_txt = SDL_CreateTextureFromSurface(*renderer, text);
	//get the text's dimensions
	TTF_SizeText(font, restart_prompt, &font_w, &font_h);
	//create the destination rectangle
	dst -> x = 200 - font_w/2;
	dst -> y = 175;
	dst -> w = font_w;
	dst -> h = font_h;
	SDL_RenderCopy(*renderer, text_txt, NULL, dst);

	//DISPLAY EXIT BLOCK
	text = TTF_RenderText_Solid(font, exit_prompt, black);
	text_txt = SDL_CreateTextureFromSurface(*renderer, text);
	//get the text's dimensions
	TTF_SizeText(font, exit_prompt, &font_w, &font_h);
	//create the destination rectangle
	dst -> x = 200 - font_w/2;
	dst -> y = 250;
	dst -> w = font_w;
	dst -> h = font_h;
	SDL_RenderCopy(*renderer, text_txt, NULL, dst);

	//free memory
	free(score);
	free(text);
	free(text_txt);
	free(dst);
}