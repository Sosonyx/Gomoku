#include <iostream>

#include "game/Game.hpp"
#include "MinMax.hpp"
#include "core/Enums.hpp"

#define TEST

#ifdef TEST

int	main(void)
{
	const Settings	settings;	
	PlayerID	turn = PlayerID::P1;
	Board board(settings);

	for (uint64_t i = 0; !board.done() ;++i) {
		Move	bestMove = getBestMove(board, turn);

		board.playStone(bestMove, turn);
		std::cout << board;
		turn = getOther(turn);
		if (i >= 2)	break;
		(void)bestMove;
	}
	return (0);
}

#elif

int	main(void)
{
	return (Game::playGame());
}

#endif
