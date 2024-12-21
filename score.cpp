#include <score.h>

Score::Score(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h)
{
	this->renderer = renderer;
	this->rect = { x, y, w, h };
	this->texture = nullptr;
	this->font = font;
	this->textColor = { 255, 255, 255, 255 };
	this->score = 0;
}

Score::~Score()
{
	SDL_DestroyTexture(texture);
}

void Score::render()
{
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Score::update(int score)
{
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	this->score = score;
	SDL_Surface* surface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Score::setFont(TTF_Font* font)
{
	this->font = font;
	update(score);
}

void Score::setTextColor(SDL_Color color)
{
	this->textColor = color;
	update(score);
}

// getters

int Score::getScore()
{
	return score;
}

SDL_Rect Score::getRect()
{
	return rect;
}

SDL_Texture* Score::getTexture()
{
	return texture;
}

TTF_Font* Score::getFont()
{
	return font;
}

SDL_Color Score::getTextColor()
{
	return textColor;
}

