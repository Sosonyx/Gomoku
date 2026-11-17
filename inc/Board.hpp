#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <stdexcept>

#include "Enums.hpp"

struct Settings;


struct Move
{
	uint16_t x = 0;
	uint16_t y = 0;
	inline Move(void): x(0), y(0) {};
	inline Move(uint16_t _x, uint16_t _y) {
		// if (x >= BoardSize || y >= BoardSize)
		// 	throw (std::runtime_error("Move(x, y) creation is oob"));
		x = _x;
		y = _y;
	}
};

using Position = Move;


class Board
{
	public:
		static inline constexpr uint16_t	nCaptureToWin = 5u;
		static inline constexpr uint16_t	BoardSize = 19u;

		template<typename T>
		using BoardArr = T[BoardSize][BoardSize];

	private:
		BoardArr<Stone> 	_board;

		uint16_t	_nCapture[2] = {0, 0};
		uint16_t	_nStone[2]  = {0, 0};

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
		Stone	_positionHas5(const uint16_t x, const uint16_t y) const
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

		
		inline Stone	getStoneAt(const uint16_t x, const uint16_t y) const{
			if (x >= BoardSize || y >= BoardSize)
				throw (std::runtime_error("getStone call is OOB"));
			return (_board[x][y]);
		}
		inline Stone	getStoneAt(const Position &pos) const{
			if (pos.x >= BoardSize || pos.y >= BoardSize)
				throw (std::runtime_error("getStone call is OOB"));
			return (_board[pos.x][pos.y]);
		}

		// Stone	getStoneAt(uint16_t x, uint16_t y)	const;

		bool	done(void)	const;
		Stone	getWinner(void)	const;

};
