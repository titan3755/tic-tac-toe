#include <state.h>

State::State(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h)
{
	this->renderer = renderer;
	this->rect = { x, y, w, h };
	this->texture = nullptr;
	this->font = font;
	this->textColor = { 255, 255, 255, 255 };
	this->state = "";
}

State::~State()
{
	SDL_DestroyTexture(texture);
}

void State::render()
{
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void State::update(std::string state)
{
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	this->state = state;
	SDL_Surface* surface = TTF_RenderText_Solid(font, state.c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void State::setFont(TTF_Font* font)
{
	this->font = font;
	update(state);
}

void State::setTextColor(SDL_Color color)
{
	this->textColor = color;
	update(state);
}

// getters

std::string State::getState()
{
	return state;
}

SDL_Rect State::getRect()
{
	return rect;
}

SDL_Texture* State::getTexture()
{
	return texture;
}

TTF_Font* State::getFont()
{
	return font;
}

SDL_Color State::getTextColor()
{
	return textColor;
}