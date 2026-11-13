#pragma once

#include <cstdint>

#include "Board.hpp"
#include "Game.hpp"

constexpr uint8_t	nDirection = 4;


struct StoneGroup {
	uint16_t	len;
	Stone		stone;
};

class Line {
	public:	// PLUS_DIAG is y = x, MINUS_DIAG is y = -x
		enum Direction: uint8_t {HORIZONTAL, VERTICAL, PLUS_DIAG, MINUS_DIAG};
	
	
	private:
		typedef int16_t	vec2[2];
		static vec2	dirVec[nDirection];
	
	private:
		const Board		&_board;

		const Move		_pos;
		const Stone		_stonePos;
		const Direction	_dir;

		const uint16_t	_len;
		const uint16_t	_posI;

	public:
		Line(const Board &board, Direction dir, const Move pos);					// Look at a position
		Line(const Board &board, Direction dir, const Move move, PlayerID player);	// Look at a move

		uint16_t	getCreated(void) const;	// Returns the len that the move created
		// Look left side (Minus side)
		// Look right side (Positive side)

		// idk
};

struct Analyse {
	public:
		enum AnalyseType {MOVE, POSITION};
		
	
	public:
		uint16_t	nCaptures;
		Line		lines[nDirection];

	public:
		Analyse(const Board &board, const Move pos);					// Look at a position
		Analyse(const Board &board, const Move move, PlayerID player);	// Look at a move
	
	
};
