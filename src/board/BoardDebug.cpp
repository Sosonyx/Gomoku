#include "board/Board.hpp"
#include <string>

#define CLR_RESET	"\e[0m"

static
std::string	getLine(const Board &board, const uint64_t y)
{
	std::string	ss;

	for (Board::size_t x = 0; x < Board::BoardSize; ++x)
	{
		if (board[x, y] == Stone::P1)
			ss += "\e[41m";
		else if (board[x, y] == Stone::P2)
			ss += "\e[44m";
		else {
			if (((x + y) & 1) == 0)
				ss += "\e[100m";
		}
		ss += ' ';
		ss += CLR_RESET;
	}
	return ss;
}

std::ostream &operator<<(std::ostream &os, const Board &board)
{
	static const char *stoneToStr[] = {"P1", "P2", "None"};

	for (Board::size_t y = 0; y < Board::BoardSize; ++y)
	{
		os << getLine(board, y);

		if (y == 1) {
			os << std::format("\tP1 capture: {}", board.getCapture(PlayerID::P1));
		} else if (y == 2) {
			os << std::format("\tP1 stone: {}", board._nStone[toInt(PlayerID::P1)]);
		} else if (y == 4) {
			os << std::format("\tP2 capture: {}", board.getCapture(PlayerID::P2));
		} else if (y == 5) {
			os << std::format("\tP2 stone: {}", board._nStone[toInt(PlayerID::P2)]);
		} else if (y == 8) {
			os << std::format("\tLast move winner: {}", stoneToStr[static_cast<uint8_t>(board._lastMoveWinner)]);
		} else if (y == 9 && board._lastMoveWinner != Stone::NONE) {
			os << std::format("\tLast move pos: {}, {}", board._lastMoveWinningPos.x, board._lastMoveWinningPos.y);
		} else if (y == 12 && board._done) {
			os << std::format("\tWinner: {}", stoneToStr[static_cast<uint8_t>(board._winner)]);
		}
		os << '\n';
	}
	return os << std::endl;
}
