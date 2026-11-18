#pragma once

#include <cstdint>

#include "Enums.hpp"
#include "Board.hpp"
#include "SideView.hpp"

#include <iostream>

struct StepDir {
	using ssize_t = Board::ssize_t;
	ssize_t x;
	ssize_t y;
};

constexpr StepDir operator-(const StepDir dir) {
	return {static_cast<StepDir::ssize_t>(-dir.x), static_cast<StepDir::ssize_t>(-dir.y)};
}

class LineView {
	public:
		using	ssize_t = Board::ssize_t;
		using	size_t = Board::size_t;

		enum Direction: uint8_t {
			HORIZONTAL,
			VERTICAL,
			PLUS_DIAG,	// Line equation is: y = x	// Goes left-up to right-down because of screen coordinate system
			MINUS_DIAG	// Line equation is: y = -x	// Goes left-down to right-up because of screen coordinate system
		};
	
		static constexpr uint8_t	nViewDirection = 2;
		static constexpr uint8_t	nDirection = 4;
		
	private:
		static constexpr StepDir	dirVec[nDirection] =  {
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

		const size_t	_lineLen;
		const size_t	_iPos;
		
		const SideView	_negativeView;
		const SideView	_positiveView;

		const size_t	_groupLen = 0;

	private:
		static constexpr size_t	_getLineLen(const Position &pos, const Direction dir);
		static constexpr size_t	_getIPos(const Position &pos, const Direction dir);
		
		static SideView	_getView(const Board &board, const Position pos, const StepDir& sDir);
		// Calculate the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction
		static size_t	_getGroupLen(const SideView &nView, const SideView &pView, Stone stoneAt);


	public:
		LineView(const LineView &) = delete;
		LineView(void) = delete;

		LineView(const Board &board, Direction dir, const Position pos);					// Look at a position
		LineView(const Board &board, Direction dir, const Move move, PlayerID playerID);	// Look at a move
		
		inline size_t	getLen(void) const {	// Returns the line len
			return (_lineLen);
		}

		inline size_t	getGroupLen(void) const {	// Returns the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction
			return (_groupLen);
		}

		const SideView	&getViewNegative(void) const;
		const SideView	&getViewPositive(void) const;	// View to the right / bot in the vertical case

		// Return a vector for the steping, based on our dir,s Always positive
		inline const StepDir	&getStepDir(void) const {
			return (dirVec[static_cast<uint16_t>(_dir)]);
		}
		
		// Return a vector for the steping
		inline static const StepDir	&getStepDir(const Direction dir) {
			return (dirVec[static_cast<uint16_t>(dir)]);
		}

		friend std::ostream	&operator<<(std::ostream &os, const LineView &lineView);
};


