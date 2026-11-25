#pragma once

// Gaby min max -- subject to change

#include "board/Move.hpp"
#include "board/Board.hpp"
#include "core/Enums.hpp"

struct MoveScore
{
	using Score = int64_t;	// Maybe use a better struct ? with the game result (tie win loss)
	Move	move;
	Score	score = 0;
	// Store result (win loss tie)

	MoveScore(void) noexcept :	move(Move::InvalidMove) {};
	MoveScore(const Move &move) noexcept :	move(move) {};

	// Add a comparator method for the score and result
	
	// MoveScore &operator -=(const  MoveScore::Score &nextMoveScore)
	// MoveScore &operator +=(const  MoveScore::Score &nextMoveScore)
	
	//	-> Increment our score based on another score

	// Method to get imediate score ?
};

constexpr uint64_t maxDepth = 5;

Move	getBestMove(Board boardCpy, PlayerID player);

/*	--- TODO ---
	add a MoveScore::Score Board::evaluateMove(Move move, PlayerId player)
		-- Returns the result and score of the move, without doing the move in memory
		-- It ll check for a 5 win
		-- It ll check for a capture win
		-- It ll calcultate the move score based on the number of capture made and on the alligments created
 */
