#pragma once

#include <exception>
#include <iostream>

#include <cstdint>

#include "Render.hpp"
#include "Board.hpp"
#include "Enums.hpp"

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
		Render	&_render;

		Settings	_settings;
		PlayerType	_players[2] = {PlayerType::HUMAN, PlayerType::HUMAN};

	private:
		Game(Render &render);
		~Game();
		
		void	_routine();

		bool	_menu();
		Stone	_startGame();
		void	_renderScore(Stone winner);

	public:
		static uint8_t	playGame();

};
