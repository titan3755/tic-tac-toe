#include <iostream>
#include <SDL.h>
#undef main // SDL2 main undefine
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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

int main(int agrc, char* argv) {
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
	success = sdl_create_window_and_renderer("Tic Tac Toe", 800, 600, &window, &renderer);
	if (success != 0) {
		return success;
	}

	// set render draw color
	SDL_Color screenColor = { 23, 55, 188, 255 };

	// event loop
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	// clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	// end program
	std::cout << "Goodbye world!" << std::endl;

	return 0;
}