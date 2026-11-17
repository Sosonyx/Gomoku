#pragma once

#include <cstdint>

#include "Enums.hpp"
#include "Board.hpp"
#include "SideView.hpp"

#include <iostream>


class LineView {
	public:
		using	ssize_t = Board::ssize_t;
		using	size_t = Board::size_t;

		
		enum ViewDirection: uint8_t {
			POSITIVE,		// View to the right / bot in the vertical case
			NEGATIVE		// View top the left / top in the vertical case
		};

		enum Direction: uint8_t {
			HORIZONTAL,
			VERTICAL,
			PLUS_DIAG,	// Line equation is: y = x	// Goes left-up to right-down because of screen coordinate system
			MINUS_DIAG	// Line equation is: y = -x	// Goes left-down to right-up because of screen coordinate system
		};
	
		static constexpr uint8_t	nViewDirection = 2;

		static constexpr uint8_t	nDirection = 4;
		// static constexpr Direction	uint16ToDirection[nDirection] = {	// PLUS_DIAG is y = x, MINUS_DIAG is y = -x
		// 		Direction::HORIZONTAL,	Direction::VERTICAL,
		// 		Direction::PLUS_DIAG,	Direction::MINUS_DIAG,
		// };
		
		using StepDir = struct {Board::ssize_t x; Board::ssize_t y;};
	
	private:
		static constexpr StepDir	dirVec[nViewDirection][nDirection] = {
			[static_cast<uint16_t>(ViewDirection::POSITIVE)] = {
				[static_cast<uint16_t>(Direction::HORIZONTAL)]	= {.x = 1, .y = 0},
				[static_cast<uint16_t>(Direction::VERTICAL)]	= {.x = 0, .y = 1},
				[static_cast<uint16_t>(Direction::PLUS_DIAG)]	= {.x = 1, .y = 1},
				[static_cast<uint16_t>(Direction::MINUS_DIAG)]	= {.x = 1, .y = -1},
			},
			[static_cast<uint16_t>(ViewDirection::NEGATIVE)] = {
				[static_cast<uint16_t>(Direction::HORIZONTAL)]	= {.x = -1, .y = 0},
				[static_cast<uint16_t>(Direction::VERTICAL)]	= {.x = 0, .y = -1},
				[static_cast<uint16_t>(Direction::PLUS_DIAG)]	= {.x = -1, .y = -1},
				[static_cast<uint16_t>(Direction::MINUS_DIAG)]	= {.x = -1, .y = 1},
			}
		};

	private:
		const Board		&_board;

		const Position	_pos;
		const Stone		_stoneAtPos;
		const Direction	_dir;

		const size_t	_lineLen;
		const size_t	_iPos;
		
		size_t	_groupLen = 0;

		bool		_negativeCreated = false;
		SideView	_negativeView;
		
		bool		_positiveCreated = false;
		SideView	_positiveView;

	private:
		static constexpr size_t	_getLineLen(const Position &pos, const Direction dir);
		static constexpr size_t	_getIPos(const Position &pos, const Direction dir);

		void	_setGroupLen(void);	// Calculate the len of consecutive pieces of the _stoneAtPos type at _pos in _dir direction

		void	_makeView(const ViewDirection viewDir);

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

		const SideView	&getViewNegative(void);
		const SideView	&getViewPositive(void);	// View to the right / bot in the vertical case

		bool negativeMakesCapture2(void);
		bool positiveMakesCapture2(void);
		
		// Return a vector for the steping, based on our dir and the given ViewDir
		inline const StepDir	&getStepDir(const ViewDirection viewDir) const {
			return (dirVec[static_cast<uint16_t>(viewDir)][static_cast<uint16_t>(_dir)]);
		}
		
		// Return a vector for the steping
		inline static const StepDir	&getStepDir(const Direction dir, const ViewDirection viewDir) {
			return (dirVec[static_cast<uint16_t>(viewDir)][static_cast<uint16_t>(dir)]);
		}

		friend std::ostream	&operator<<(std::ostream &os, LineView &lineView);
};


