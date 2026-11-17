#pragma once

#include <cstdint>

#include "Enums.hpp"
#include "Board.hpp"
#include "SideView.hpp"

#include <iostream>	//tmp


class LineView {
	public:	// PLUS_DIAG is y = x, MINUS_DIAG is y = -x
		enum Direction: uint8_t {
			HORIZONTAL,
			VERTICAL,
			PLUS_DIAG,	// Line equation is: y = x	// Goes left-up to right-down because of screen coordinate system
			MINUS_DIAG	// Line equation is: y = -x	// Goes left-down to right-up because of screen coordinate system
		};
	
		static constexpr uint8_t	nDirection = 4;
		static constexpr Direction	uint16ToDirection[nDirection] = {
				Direction::HORIZONTAL,	Direction::VERTICAL,
				Direction::PLUS_DIAG,	Direction::MINUS_DIAG,
		};
		
		private:
		static inline constexpr auto BoardSize = Board::BoardSize;
	
		using DirVector = struct {int x; int y;};

		static constexpr DirVector	dirVec[nDirection] = {
			[static_cast<uint16_t>(Direction::HORIZONTAL)]	= {.x = 1, .y = 0},
			[static_cast<uint16_t>(Direction::VERTICAL)]	= {.x = 0, .y = 1},
			[static_cast<uint16_t>(Direction::PLUS_DIAG)]	= {.x = 1, .y = 1},
			[static_cast<uint16_t>(Direction::MINUS_DIAG)]	= {.x = 1, .y = -1},
		};

	private:
		const Board		&_board;

		const Position	_pos;
		const Stone		_stoneAtPos;
		const Direction	_dir;

		const uint16_t	_lineLen;
		const uint16_t	_iPos;
		
		uint16_t			_groupLen = 0;
		SideView<Board::BoardSize>	_left;
		bool				_leftCreated = false;
		SideView<Board::BoardSize>	_right;
		bool				_rightCreated = false;

	private:
		static constexpr uint16_t	_getLineLen(const Position &pos, const Direction dir);
		static constexpr uint16_t	_getIPos(const Position &pos, const Direction dir);

		void	_setGroupLen(void);	// Calculate the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction

		
	public:
		LineView(const LineView &) = delete;
		LineView(void) = delete;
		LineView(const Board &board, Direction dir, const Position pos);					// Look at a position
		LineView(const Board &board, Direction dir, const Move move, PlayerID playerID);	// Look at a move
		
		inline uint16_t	getLen(void) const {	// Returns the line len
			return (_lineLen);
		}
		inline uint16_t	getGroupLen(void) const {	// Returns the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction
			return (_groupLen);
		}
		
		const SideView<BoardSize>	&getViewNegative(void);	// View top the left / top in the vertical case
		const SideView<BoardSize>	&getViewPositive(void);	// View to the right / bot in the vertical case
		
		void	debug(void) const;
};
