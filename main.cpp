#include <iostream>
#include <vector>
#include <SDL.h>
#undef main // SDL2 main undefine
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr const char* BASE_WINDOW_TITLE = "Tic Tac Toe";

// struct for text
struct Text {
	SDL_Texture* texture;
	SDL_Rect rect;
};

// game state enum
enum class GameState {
	TITLE_SCREEN,
	PLAYING_X,
	PLAYING_O,
	GAME_OVER
};

// init sdl, sdl_image, sdl_ttf
int sdl_init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "SDL_Init Success!" << std::endl;
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "IMG_Init Success!" << std::endl;
	}
	if (TTF_Init() != 0) {
		std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "TTF_Init Success!" << std::endl;
	}
	return 0;
}

// create window and renderer
int sdl_create_window_and_renderer(const char* title, int width, int height, SDL_Window** window, SDL_Renderer** renderer) {
	*window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (*window == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "SDL_CreateWindow Success!" << std::endl;
	}
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer == nullptr) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "SDL_CreateRenderer Success!" << std::endl;
	}
	return 0;
}

// generate text for title screen, there will be choices for the user to select using keyboard
std::vector<Text> titleScreenSetup(SDL_Renderer* renderer, TTF_Font* font) {
	std::vector<Text> texts;
	int verticalTranslation = 50;
	// title text
	SDL_Color colorForTitle = { 255, 255, 255, 255 };
	SDL_Surface* surfaceTitle = TTF_RenderText_Solid(font, BASE_WINDOW_TITLE, colorForTitle);
	SDL_Texture* textureTitle = SDL_CreateTextureFromSurface(renderer, surfaceTitle);
	SDL_Rect rectTitle;
	// center the text
	rectTitle.x = SCREEN_WIDTH / 2 - surfaceTitle->w / 2;
	rectTitle.y = SCREEN_HEIGHT / 2 - surfaceTitle->h / 2 - verticalTranslation;
	rectTitle.w = surfaceTitle->w;
	rectTitle.h = surfaceTitle->h;
	SDL_FreeSurface(surfaceTitle);

	// which side do you want to play as text
	SDL_Color colorForSide = { 255, 255, 255, 255 };
	SDL_Surface* surfaceSide = TTF_RenderText_Solid(font, "Which side do you want to play as?", colorForSide);
	SDL_Texture* textureSide = SDL_CreateTextureFromSurface(renderer, surfaceSide);
	SDL_Rect rectSide;
	// center the text
	rectSide.x = SCREEN_WIDTH / 2 - surfaceSide->w / 2;
	rectSide.y = SCREEN_HEIGHT / 2 - surfaceSide->h / 2 + 50 - verticalTranslation;
	rectSide.w = surfaceSide->w;
	rectSide.h = surfaceSide->h;
	SDL_FreeSurface(surfaceSide);

	// x or o text
	SDL_Color colorForXO = { 255, 255, 255, 255 };
	SDL_Surface* surfaceXO = TTF_RenderText_Solid(font, "Press \"1\" for \"X\" or Press \"2\" for \"O\"", colorForXO);
	SDL_Texture* textureXO = SDL_CreateTextureFromSurface(renderer, surfaceXO);
	SDL_Rect rectXO;
	// center the text
	rectXO.x = SCREEN_WIDTH / 2 - surfaceXO->w / 2;
	rectXO.y = SCREEN_HEIGHT / 2 - surfaceXO->h / 2 + 100 - verticalTranslation;
	rectXO.w = surfaceXO->w;
	rectXO.h = surfaceXO->h;
	SDL_FreeSurface(surfaceXO);

	// check for errors
	if (textureTitle == nullptr || textureSide == nullptr || textureXO == nullptr) {
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return texts;
	}

	// add texts to vector
	texts.reserve(3);
	texts.push_back({ textureTitle, rectTitle });
	texts.push_back({ textureSide, rectSide });
	texts.push_back({ textureXO, rectXO });

	return texts;

}

// playing mode text setup
std::vector<Text> playingModeTextSetup(SDL_Renderer* renderer, TTF_Font* font, GameState gs) {
	// create two texts, one for the computer and one for the player
	// the computer text will be on the left side of the screen
	// the player text will be on the right side of the screen
	std::vector<Text> texts;
	
	// computer text
	SDL_Color colorForComputer = { 255, 255, 255, 255 };
	char computerText[100];
	strcpy_s(computerText, "Computer: undefined");
	if (gs == GameState::PLAYING_X) {
		strcpy_s(computerText, "Computer: O");
	}
	else if (gs == GameState::PLAYING_O) {
		strcpy_s(computerText, "Computer: X");
	}
	SDL_Surface* surfaceComputer = TTF_RenderText_Solid(font, computerText, colorForComputer);
	SDL_Texture* textureComputer = SDL_CreateTextureFromSurface(renderer, surfaceComputer);
	SDL_Rect rectComputer;
	// left side of the screen
	rectComputer.x = 0;
	rectComputer.y = 0;
	rectComputer.w = surfaceComputer->w;
	rectComputer.h = surfaceComputer->h;
	SDL_FreeSurface(surfaceComputer);

	// player text
	SDL_Color colorForPlayer = { 255, 255, 255, 255 };
	char playerText[100];
	strcpy_s(playerText, "Player: undefined");
	if (gs == GameState::PLAYING_X) {
		strcpy_s(playerText, "Player: X");
	}
	else if (gs == GameState::PLAYING_O) {
		strcpy_s(playerText, "Player: O");
	}
	SDL_Surface* surfacePlayer = TTF_RenderText_Solid(font, playerText, colorForPlayer);
	SDL_Texture* texturePlayer = SDL_CreateTextureFromSurface(renderer, surfacePlayer);
	SDL_Rect rectPlayer;
	// right side of the screen
	rectPlayer.x = SCREEN_WIDTH - surfacePlayer->w;
	rectPlayer.y = 0;
	rectPlayer.w = surfacePlayer->w;
	rectPlayer.h = surfacePlayer->h;
	SDL_FreeSurface(surfacePlayer);

	// check for errors
	if (textureComputer == nullptr || texturePlayer == nullptr) {
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return texts;
	}

	// add texts to vector
	texts.reserve(2);
	texts.push_back({ textureComputer, rectComputer });
	texts.push_back({ texturePlayer, rectPlayer });

	return texts;
}

SDL_Texture* renderGridProportionalToScreenSize(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
	// Reserve space at the top of the screen for UI elements (e.g., scores, player/computer selection)
	const int topSpace = 100;  // 100px reserved for UI elements at the top

	// Adjust the grid position and size based on the remaining screen area
	int gridWidth = screenWidth * 0.6;  // Grid will take up 60% of screen width
	int gridHeight = screenHeight * 0.6;  // Grid will take up 60% of screen height

	// Grid's position, starting below the UI area
	int gridX = (screenWidth - gridWidth) / 2;
	int gridY = topSpace + (screenHeight - topSpace - gridHeight) / 2;

	// Cell width and height (divide grid into 3 equal parts)
	int cellWidth = gridWidth / 3;
	int cellHeight = gridHeight / 3;

	// Create an SDL_Surface to draw the grid onto
	SDL_Surface* gridSurface = SDL_CreateRGBSurfaceWithFormat(0, gridWidth, gridHeight, 32, SDL_PIXELFORMAT_RGBA32);
	if (gridSurface == nullptr) {
		std::cerr << "SDL_CreateRGBSurfaceWithFormat Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Set the surface to transparent (fill with alpha 0)
	SDL_FillRect(gridSurface, nullptr, SDL_MapRGBA(gridSurface->format, 0, 0, 0, 0));  // Transparent background

	// Set up SDL_Renderer for the surface (a temporary renderer)
	SDL_Renderer* tempRenderer = SDL_CreateSoftwareRenderer(gridSurface);
	if (tempRenderer == nullptr) {
		std::cerr << "SDL_CreateSoftwareRenderer Error: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(gridSurface);
		return nullptr;
	}

	// Set render color to white for the grid lines (opaque)
	SDL_SetRenderDrawColor(tempRenderer, 255, 255, 255, 255);  // White color for grid lines

	// Draw vertical lines (2 lines)
	for (int i = 1; i < 3; ++i) {
		int x = i * cellWidth;
		SDL_RenderDrawLine(tempRenderer, x, 0, x, gridHeight);
	}

	// Draw horizontal lines (2 lines)
	for (int i = 1; i < 3; ++i) {
		int y = i * cellHeight;
		SDL_RenderDrawLine(tempRenderer, 0, y, gridWidth, y);
	}

	// Create texture from the surface
	SDL_Texture* gridTexture = SDL_CreateTextureFromSurface(renderer, gridSurface);
	SDL_FreeSurface(gridSurface);  // Free the surface now that we have the texture

	// Clean up temporary renderer
	SDL_DestroyRenderer(tempRenderer);

	return gridTexture;
}

int main(int argc, char* argv[]) {
	// start program
	std::cout << "Hello world!" << std::endl;

	// init sdl, sdl_image, sdl_ttf
	int success = sdl_init();
	if (success != 0) {
		return success;
	}

	// create window and renderer
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	success = sdl_create_window_and_renderer(BASE_WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, &window, &renderer);
	if (success != 0) {
		return success;
	}

	// set render draw color
	SDL_Color screenColor = { 23, 55, 188, 255 };

	// load font
	TTF_Font* font = TTF_OpenFont("assets/BrotesqueRegular.ttf", 24);
	if (font == nullptr) {
		std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "TTF_OpenFont Success!" << std::endl;
	}

	// title screen setup
	std::vector<Text> texts = titleScreenSetup(renderer, font);

	// playing mode text setup
	std::vector<Text> playingModeTexts;

	// grid texture
	SDL_Texture* gridTexture = renderGridProportionalToScreenSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	// grid rect ; position at center of the screen
	SDL_Rect gridRect;
	gridRect.x = (SCREEN_WIDTH - 500) / 2;
	gridRect.y = (SCREEN_HEIGHT - 600) / 2 + 75;
	gridRect.w = 500;
	gridRect.h = 500;

	// game state
	GameState gameState = GameState::TITLE_SCREEN;

	// event loop
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (gameState == GameState::TITLE_SCREEN) {
					if (event.key.keysym.sym == SDLK_1) {
						std::cout << "You selected X" << std::endl;
						// destroy texts
						for (auto& text : texts) {  
							if (text.texture != nullptr) {
								SDL_DestroyTexture(text.texture);
								text.texture = nullptr;
							}
						}
						gameState = GameState::PLAYING_X;
						playingModeTexts = playingModeTextSetup(renderer, font, gameState);
					}
					else if (event.key.keysym.sym == SDLK_2) {
						std::cout << "You selected O" << std::endl;
						// destroy texts
						for (auto& text : texts) {  
							if (text.texture != nullptr) {
								SDL_DestroyTexture(text.texture);
								text.texture = nullptr;
							}
						}
						gameState = GameState::PLAYING_O;
						playingModeTexts = playingModeTextSetup(renderer, font, gameState);
					}
				}
				else if (gameState == GameState::PLAYING_X || gameState == GameState::PLAYING_O) {
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						gameState = GameState::TITLE_SCREEN;
					}
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);
		SDL_RenderClear(renderer);

		// --> main rendering here

		// render texts
		if (gameState == GameState::TITLE_SCREEN) {
			for (const auto& text : texts) {
				SDL_RenderCopy(renderer, text.texture, nullptr, &text.rect);
			}
		}

		// playing mode x
		if (gameState == GameState::PLAYING_X) {
			// render texts
			for (const auto& text : playingModeTexts) {
				SDL_RenderCopy(renderer, text.texture, nullptr, &text.rect);
			}
			// render grid
			if (gridTexture != nullptr) {
				SDL_RenderCopy(renderer, gridTexture, nullptr, &gridRect);
			}
		}

		// playing mode o
		if (gameState == GameState::PLAYING_O) {
			// render texts
			for (const auto& text : playingModeTexts) {
				SDL_RenderCopy(renderer, text.texture, nullptr, &text.rect);
			}
			// render grid
			if (gridTexture != nullptr) {
				SDL_RenderCopy(renderer, gridTexture, nullptr, &gridRect);
			}
		}

		// gameover mode
		if (gameState == GameState::GAME_OVER) {
			// render game over
		}

		// <-- main rendering here

		// present renderer
		SDL_RenderPresent(renderer);
	}

	// clean up
	// check if texts already freed, if not then free them
	for (auto& text : texts) {
		if (text.texture != nullptr) {
			SDL_DestroyTexture(text.texture);
			text.texture = nullptr;
		}
	}
	for (auto& text : playingModeTexts) {
		if (text.texture != nullptr) {
			SDL_DestroyTexture(text.texture);
			text.texture = nullptr;
		}
	}
	if (gridTexture != nullptr) {
		SDL_DestroyTexture(gridTexture);
		gridTexture = nullptr;
	}
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	// end program
	std::cout << "Goodbye world!" << std::endl;

	return 0;
}