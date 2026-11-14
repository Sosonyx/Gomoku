#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Board.hpp"
#include "Enums.hpp"

class Render
{
	private:
		SDL_Renderer	*_renderer; // The renderer is used to render textures to the window
		SDL_Window		*_window; // The window is where we will render our textures
		SDL_Texture 	*_texture; // The texture is like a buffer that we can render to the window
		SDL_Surface 	*_surface; // The surface is used to load images and then convert them to textures
		TTF_Font		*_font;
		Move			_hoveredMove;
		bool			_hoveredMoveLegal;
		Stone			_hoveredStone;
			
		void renderBackground();
		void renderGrid();
		void renderStones(Board &board);
		void renderStone(int x, int y, Stone stone, bool preview = false);
		void renderText(const char *text, int x, int y, int size = 32,
			bool light = false);
		Move pixelToGrid(int x, int y) const;
	
	public:
		Render();
		~Render();
		void initRender();

		void renderMenu(PlayerType player1, PlayerType player2);
		void renderGame(Board &board, PlayerID player, double moveTime,
			uint16_t player1Captures, uint16_t player2Captures);
		bool isStartButton(int x, int y) const;
		bool isExitButton(int x, int y) const;
		bool isPlayerButton(PlayerID player, int x, int y) const;
		Move detectPlayerMove(Board &board, PlayerID player,
			PlayerID lastPlayer, double moveTime,
			uint16_t player1Captures, uint16_t player2Captures);
		// void	placeStone(Move);
};
