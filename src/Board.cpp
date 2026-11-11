#include "Board.hpp"

Board::Board(const Settings &settings)
:	_settings(settings)
{
	std::memset(_board, Stone::NONE, sizeof(_board));
}


bool	Board::done(void)	const {
	return (_done);
}

Stone	Board::getWinner(void)	const {
	if (_done == false)
		throw (std::runtime_error("GetWinner call when Board is not done"));
	return (_winner);
}
