#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "Enums.hpp"

struct Settings;

namespace BoardConfig {
	using ssize_t	= int8_t;
	using size_t	= uint8_t;
	inline constexpr size_t nCaptureToWin	= 5u;
	inline constexpr size_t MinLenToWin		= 5u;
	inline constexpr size_t BoardSize		= 19u;
}

struct Move
{
	using	size_t = BoardConfig::size_t;
	size_t x = 0;
	size_t y = 0;
	inline Move(void):	x(0), y(0) {};
	inline Move(const size_t _x,const size_t _y):	x(_x), y(_y)  {}
};
using Position = Move;

class Board
{
	public:
		using	ssize_t	= BoardConfig::ssize_t;
		using	size_t	= BoardConfig::size_t;

		static inline constexpr size_t	nCaptureToWin	= BoardConfig::nCaptureToWin;
		static inline constexpr size_t	BoardSize		= BoardConfig::BoardSize;
		static inline constexpr size_t	MinLenToWin		= BoardConfig::MinLenToWin;


		template<typename T>
		using BoardArr = T[BoardSize][BoardSize];

	private:
		BoardArr<Stone> 	_board;

		size_t	_nCapture[2] = {0, 0};
		size_t	_nStone[2]  = {0, 0};

		// Did we have a winner last move
		Stone		_lastMoveWinner = Stone::NONE;
		Position	_lastMoveWinningPos = Position(0, 0);

		Stone	_winner = Stone::NONE;
		bool	_done = false;

		const Settings	&_settings;

	private:

		bool	_makesFree3(const Move move, const PlayerID _player)	const;
		void	_capture2(const Move move, const PlayerID _player);
	
		
		Stone	_positionHas5(const Position pos) const;
		Stone	_positionHas5(const size_t x, const size_t y) const
		{	return (_positionHas5(Position(x, y)));	};
		
		void	_iterOnWinPosition(void);

		bool	_checkWinner(const Position pos, const PlayerID player);
		bool	_checkWinBy5(const Position pos, const PlayerID player);
		bool	_checkWinByCapture(void);

		inline bool	_setWinner(const Stone stone = Stone::NONE) {
			_winner = stone;
			_done = true;
			return (true);
		}
		inline bool	_setWinner(const PlayerID player) {
			return (_setWinner(toStone(player)));
		}

	public:
		Board(const Settings &settings);
		Board(const Board &oldBoard, const Move &playedMove, const PlayerID player);

		bool	hasLegalMove(const PlayerID player)	const;

		bool	isMoveLegal(const Move move, const PlayerID player)	const;
		void	playStone(const Move move, const PlayerID player);

		
		inline Stone	getStoneAt(const size_t x, const size_t y) const{
			if (x >= BoardSize || y >= BoardSize)
				throw (std::runtime_error("getStone call is OOB"));
			return (_board[x][y]);
		}
		inline Stone	getStoneAt(const Position &pos) const{
			if (pos.x >= BoardSize || pos.y >= BoardSize)
				throw (std::runtime_error("getStone call is OOB"));
			return (_board[pos.x][pos.y]);
		}

		inline bool	done(void)	const {	return (_done);	}

		inline Stone	getWinner(void)	const {
			if (_done == false)
				throw (std::runtime_error("GetWinner call when Board is not done"));
			switch (_winner) {
				case (Stone::NONE):
					std::cout << " -- Tie -- " << std::endl;
					break ;
				case (Stone::P1):
					std::cout << " -- P1 -- " << std::endl;
					break ;
				case (Stone::P2):
					std::cout << " -- P2 -- " << std::endl;
					break ;
			}
			return (_winner);
		}
};
