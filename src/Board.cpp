#include "Board.hpp"
#include <stdexcept>

Board::Board(const Settings &settings)
:	_settings(settings)
{
	std::memset(_board, Stone::NONE, sizeof(_board));
}


bool	Board::done(void)	const {
	return (_done);
}

Stone	Board::getStoneAt(uint16_t x, uint16_t y)	const {
	if (x >= BoardSize || y >= BoardSize)
		throw (std::out_of_range("getStoneAt: x or y out of range"));
	return (_board[x][y]);
}

Stone	Board::getWinner(void)	const {
	if (_done == false)
		throw (std::runtime_error("GetWinner call when Board is not done"));
	return (_winner);
}
