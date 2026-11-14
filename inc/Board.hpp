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
constexpr uint16_t	nCaptureToWin = 5;

class Board
{
	public:
		template<typename T>
		using BoardArr = T[BoardSize][BoardSize];

	private:
		BoardArr<Stone> 	_board;

		uint16_t	_nCapture[2] = {0, 0};
		uint16_t	_nStone[2]  = {0, 0};
		
		Stone	_winner = Stone::NONE;
		bool	_done = false;

		const Settings	&_settings;

	private:

		bool	_makesFree3(const Move move, Stone player)	const;
		void	_capture2(const Move move, Stone player);
	
		bool	_checkWinner(const Move move, PlayerID player);

	public:
		Board(const Settings &settings);
		Board(const Board &oldBoard, const Move &playedMove, PlayerID player);

		bool	hasLegalMove(PlayerID player)	const;

		bool	isMoveLegal(const Move move, PlayerID player)	const;
		void	playStone(const Move move, PlayerID player);

		
		inline Stone	getStoneAt(const uint16_t x, const uint16_t y) const{
			if (x >= BoardSize || y >= BoardSize)
				std::runtime_error("getStone call is OOB");
			return (_board[x][y]);
		}

		bool	done(void)	const;
		Stone	getWinner(void)	const;
};

