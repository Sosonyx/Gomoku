#pragma once

#include <cstdint>
#include <vector>

#include "board/Board.hpp"
#include "game/Game.hpp"
#include "analysis/LineView.hpp"

struct Analyse {
	public:
		enum AnalyseType {MOVE, POSITION};
		
	
	public:
		uint16_t	nCaptures;
		LineView		lines[nDirection];

	public:
		Analyse(const Board &board, const Move pos);					// Look at a position
		Analyse(const Board &board, const Move move, PlayerID player);	// Look at a move
};
