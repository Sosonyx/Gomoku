#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <stdexcept>

#include "Enums.hpp"

struct Settings;

constexpr uint16_t	BoardSize = 19;

struct Move
{
	uint16_t x = 0;
	uint16_t y = 0;
	inline Move(void) {};
	inline Move(uint16_t _x, uint16_t _y) {
		// if (x >= BoardSize || y >= BoardSize)
		// 	throw (std::runtime_error("Move(x, y) creation is oob"));
		x = _x;
		y = _y;
	}
};

using Position = Move;

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
		inline Stone	getStoneAt(const Position &pos) const{
			if (pos.x >= BoardSize || pos.y >= BoardSize)
				std::runtime_error("getStone call is OOB");
			return (_board[pos.x][pos.y]);
		}

		// Stone	getStoneAt(uint16_t x, uint16_t y)	const;

		bool	done(void)	const;
		Stone	getWinner(void)	const;
		uint16_t getCapture(PlayerID player) const;
};
