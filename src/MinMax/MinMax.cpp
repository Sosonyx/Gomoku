#include <vector>
#include "Board.hpp"

// Score Flat, pas de recursivite
// Player is us or the other

struct MM_Move
{
	Move	move;
	int64_t	score;
};

constexpr int64_t maxDepth = 10;

// Get the score of the actual move without recursion
int64_t	getBoardScore(Move move, Board &board, bool player) {

}

MM_Move	getMovePlayer(uint64_t depth, Board &board, bool player) {
	// Get all moves for ou player (Human or IA)
	// Check if one move is final (Win / Lose / StaleMate) or max depth reached
	//		return ;
	// Iter on moves
	//		Copy Board
	//		Make Move on Board
	//		moveScore = getMovePlayer( depth + 1, Board, otherPlayer)
	//		
	//		Restore Board
	// return best score
}

Move	getMoveAI(Board board) {
	std::vector<Move>	Moves;	// get all Moves

	if (Moves.size() == 0)
		return ;
	Move		bestMove;
	uint64_t	bestScore = 0;

	// Check if one is insta lose or win
}
