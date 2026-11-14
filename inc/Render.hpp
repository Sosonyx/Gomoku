#pragma once

#include <SDL2/SDL.h>
#include "Board.hpp"

class Render
{
	private:
		SDL_Renderer	*_renderer; // The renderer is used to render textures to the window
		SDL_Window		*_window; // The window is where we will render our textures
		SDL_Texture 	*_texture; // The texture is like a buffer that we can render to the window
		SDL_Surface 	*_surface; // The surface is used to load images and then convert them to textures
			
		void renderBackground();
		void renderStones(Board &board);
		void renderStone(int x, int y);
	
	public:
		Render();
		~Render();
		void initRender();
		
		void renderMenu();
		void renderGame(Board &board);
		bool isStartButton(int x, int y) const;
		void detectPlayerMove(Board &board, PlayerID player);
		// void	placeStone(Move);
};
