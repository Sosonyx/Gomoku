#pragma once

#include <cstdint>

#include "Enums.hpp"
#include "Board.hpp"
#include "SideView.hpp"

constexpr uint8_t	nDirection = 4;
class LineView {
	public:	// PLUS_DIAG is y = x, MINUS_DIAG is y = -x
		enum Direction: uint8_t {HORIZONTAL, VERTICAL, PLUS_DIAG, MINUS_DIAG};
	
		typedef int	vec2[2][2];
	private:
		static vec2	dirVec[nDirection];
	
	private:
		const Board		&_board;

		const Move		_pos;
		const Stone		_stonePos;
		const Direction	_dir;

		const uint16_t	_len;
		const uint16_t	_posI;

		const SideView<BoardSize>	_left;
		const SideView<BoardSize>	_right;

	private:


	public:
		LineView(const Board &board, Direction dir, const Move pos);					// Look at a position
		LineView(const Board &board, Direction dir, const Move move, PlayerID player);	// Look at a move

		uint16_t	getLen(void) const;	// Returns the len of consecutive piecess that the move created
		SideView<BoardSize>	getViewPositive(void);
		SideView<BoardSize>	getViewNegative(void);
		// Look right side (Positive side)
		// Look Left side (Negative side)

		// idk
};
