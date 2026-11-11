#pragma once

#include <cstdint>
#include <cstring>
#include <exception>

#include "Game.hpp"
#include "Enums.hpp"

struct Settings;

struct Move
{
	uint16_t x;
	uint16_t y;
};


constexpr uint16_t	BoardSize = 19;

class Board
{
	public:

		
	private:
		Stone 	_board[BoardSize][BoardSize];
		
		Stone	_winner = Stone::NONE;
		bool	_done = false;

		const Settings	&_settings;

	public:
		Board(const Settings &settings);
		Board(const Board &oldBoard, const Move &playedMove);

		bool	hasLegalMove(Stone player)	const;

		bool	isMoveLegal(const Move move, Stone player)	const;
		void	playStone(const Move move, Stone player);

		bool	done(void)	const;
		Stone	getWinner(void)	const;
};
