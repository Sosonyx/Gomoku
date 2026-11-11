#pragma once

#include <exception>
#include <iostream>

#include "Board.hpp"
#include "Render.hpp"

enum PlayerID: uint8_t 
{
	P1 = 0,
	P2 = 1,
};

enum PlayerType: uint8_t 
{
	AI = 0,
	HUMAN = 1
};

enum	GameMode
{
	Normal,
	Swap,
	Swap2
};

struct Settings
{
	GameMode	gameMode 	= GameMode::Normal; 
	bool		capture2 	= true;
	bool		free3		= true;
};

class Game {
	private:
		Render	_render;

		Settings	_settings;

	private:
		Game(Render &render);
		~Game();
		
		void			_routine();

		bool			_menu();
		Board::Stone	_startGame();
		void			_renderScore(Board::Stone winner);

	public:
		static uint8_t	playGame();

};
