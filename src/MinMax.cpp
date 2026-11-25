#include <vector>
#include <ranges>

#include "MinMax.hpp"
#include "board/RestoreMoveInfo.hpp"

// Only take good moves
//
std::vector<MoveScore>	getAllMoves(const Board &board, PlayerID player) {
	std::vector<MoveScore>	allMoves;

	if (board.empty())
		allMoves.push_back(Position(Board::BoardSize / 2, Board::BoardSize / 2));
	else {
		for (Move move: Move::ViewAll()) {
			if (!board.isMoveLegal(move, player))
				continue;
			allMoves.push_back(move);
			// std::cout << move << std::endl;
		}
	}
	if (allMoves.size() > 5)
		allMoves.resize(5);
	return (allMoves);
}

MoveScore::Score	getImmediateScore(Board &board, const Move move, PlayerID player) {
	RestoreMoveInfo	RMI = board.RMI_playStone(move, player);
	
	MoveScore::Score score = 0;
	
	board.RestoreMove(RMI);
	return (score);
}

MoveScore	getBestMoveScore(Board &board, PlayerID player, uint64_t depth)
{
	std::vector<MoveScore>	moves(getAllMoves(board, player));	// get all Moves

	// Goal is to store imediate (no recursive) win / lose / tie
	for (auto &oneMove: moves) {
		// std::cout << oneMove.move << std::endl;
		oneMove.score = getImmediateScore(board, oneMove.move, player);

		// If one of them is a win -> return it
		// if (oneMove.isWin())
		// 	return (oneMove);
	}

	// Remove the insta lose
	//		Also remove bad scores ?	<- alpha beta prunning ??
	//		Keep at least N MoveScore ?
	
	if (depth < maxDepth) {
		for (auto &oneMove: moves) {
			// If is done (tie win lose): continue;
			// If move not usefull continue; // <- alpha beta prunning ??
			RestoreMoveInfo	RMI = board.RMI_playStone(oneMove.move, player);

			// Negate because its the other player score
			oneMove.score -= getBestMoveScore(board, getOther(player), depth + 1).score;

			board.RestoreMove(RMI);
		}
	}
	// select best using std::algorithm and Score comparator member fnc
	return (moves[0]);
}

Move	getBestMove(Board boardCpy, PlayerID player) {
	return (getBestMoveScore(boardCpy, player, 0).move);
}
