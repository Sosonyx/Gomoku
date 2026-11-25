#include "search/Search.hpp"

Move	findBestMove(const Board &board, PlayerID who)
{
	SearchState	state(board);
	Move		moves[Board::BoardSize * Board::BoardSize];
	const int	n = state.generateMoves(moves);

	const int	center = Board::BoardSize / 2;
	int			bestDist = -1;
	Move		best(static_cast<Board::size_t>(-1), static_cast<Board::size_t>(-1));

	for (int i = 0; i < n; ++i) {
		if (!board.isMoveLegal(moves[i], who))
			continue;
		const int dx = moves[i].x - center;
		const int dy = moves[i].y - center;
		const int dist = dx * dx + dy * dy;
		if (bestDist == -1 || dist < bestDist) {
			bestDist = dist;
			best = moves[i];
		}
	}
	return (best);
}
