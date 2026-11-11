#pragma once

#include <cstdint>

#include "Game.hpp"
#include "Render.hpp"

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
		enum Stone: uint8_t 
		{
			P1 = PlayerID::P1,
			P2 = PlayerID::P2,
			NONE,
		};
		
	private:
		Board::Stone 	_board[BoardSize][BoardSize];
		
		Board::Stone	_winner = Board::Stone::NONE;
		bool	_done = false;

		const Settings	&settings;

	public:
		Board(const Settings &settings);
		Board(const Board &oldBoard, const Move &playedMove);

		bool	hasLegalMove(Stone player)	const;

		bool	isMoveLegal(const Move move, Stone player)	const;
		void	playStone(const Move move, Stone player);

		bool			done(void)	const;
		Board::Stone	getWinner(void)	const;
};
