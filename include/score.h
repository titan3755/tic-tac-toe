#pragma once

#ifndef SCORE_H
#define SCORE_H

#include <SDL.h>
#undef main // Undefine main for SDL2
#include <SDL_ttf.h>
#include <string>

class Score
{
public:
	Score(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h);
	~Score();

	void render();
	void update(int score);
	void setFont(TTF_Font* font);
	void setTextColor(SDL_Color color);
	// getters
	int getScore();
	SDL_Rect getRect();
	SDL_Texture* getTexture();
	TTF_Font* getFont();
	SDL_Color getTextColor();
	
private:
	SDL_Renderer* renderer;
	SDL_Rect rect;
	SDL_Texture* texture;
	TTF_Font* font;
	SDL_Color textColor;
	int score;
};

#endif // SCORE_H