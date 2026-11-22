#pragma once

#include <cstdint>

#include "core/Enums.hpp"
#include "board/Board.hpp"
#include "analysis/SideView.hpp"

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
		using	Grid = Board::BoardArr<Stone>;	// Anything exposing a raw grid (Board, SearchState) can be analysed

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
				{.x = 1, .y = 0},
				{.x = 0, .y = 1},
				{.x = 1, .y = 1},
				{.x = 1, .y = -1},
		};

	private:
		const Grid		&_grid;

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
		
		static SideView	_getView(const Grid &grid, const Position pos, const StepDir& sDir);
		// Calculate the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction
		static size_t	_getGroupLen(const SideView &nView, const SideView &pView, Stone stoneAt);
		
		// only checks 0xXsy and yxX0, so it needs to be calls invertion + and - views (X is stoneAt)
		static bool		_posCanBeCaptured(const SideView &nView, const SideView &pView, Stone stoneAt);
		
		// only checks 2 paterns, so need revers and 2 calls gngngn
		// only checks 0xXsy and yxX0, so it needs to be calls invertion + and - views (X is stoneAt)
		static bool		_posMakesFree3(const SideView &nView, const SideView & pView,const PlayerID player, const size_t groupLen);

	public:
		LineView(const LineView &) = delete;
		LineView(void) = delete;

		LineView(const Grid &grid, Direction dir, const Position pos);					// Look at a position
		LineView(const Grid &grid, Direction dir, const Move move, PlayerID playerID);	// Look at a move

		// Same, straight from a Board
		LineView(const Board &board, Direction dir, const Position pos);
		LineView(const Board &board, Direction dir, const Move move, PlayerID playerID);
		
		inline size_t	getLen(void) const {	// Returns the line len
			return (_lineLen);
		}

		inline size_t	getGroupLen(void) const {	// Returns the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction
			return (_groupLen);
		}

		const SideView	&getViewNegative(void) const;
		const SideView	&getViewPositive(void) const;	// View to the right / bot in the vertical case

		// 4 aligned with Stone:NONE on both sides
		
		
		inline bool	isWin(void)	const {
			return (_groupLen >= Board::MinLenToWin);
		}
		
		// Position Creates a free 4
		bool	isFree4(void)	const;
		// Position Creates a free 3
		bool	isFree3(void)	const;

		// Position can be captured
		bool	canBeCaptured(void) const;

		// Check if our winning position can be taken
		bool	winCanBeCaptured(void) const;

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


