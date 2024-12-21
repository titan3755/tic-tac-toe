#pragma once

#ifndef STATE_H
#define STATE_H

#include <SDL.h>
#undef main // Undefine main for SDL2
#include <SDL_ttf.h>
#include <string>

class State
{
public:
	State(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h);
	~State();

	void render();
	void update(std::string state);
	void setFont(TTF_Font* font);
	void setTextColor(SDL_Color color);
	// getters
	std::string getState();
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
	std::string state;
};

#endif // STATE_H