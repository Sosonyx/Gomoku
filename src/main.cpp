#include "Game.hpp"
#include <SDL2/SDL.h>

int	main(void)
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Error: " << SDL_GetError() << std::endl;
		return (1);
	}
	SDL_Window *win = SDL_CreateWindow("Gomoku",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,   // position
			1100, 760,                                        // taille en pixels
			SDL_WINDOW_SHOWN);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // noir
	SDL_RenderClear(ren);                       // 2. efface l'écran
	while (1) {
		SDL_Event e;
		SDL_RenderPresent(ren);                     // 3. affiche l'écran

		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				break ;
		}
	}
	return (Game::playGame());
}
