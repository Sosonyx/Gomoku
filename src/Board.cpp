#include "Board.hpp"
#include "Game.hpp"
#include <stdexcept>

Board::Board(const Settings &settings)
:	_settings(settings)
{
	std::memset(_board, static_cast<uint8_t>(Stone::NONE), sizeof(_board));
}

Board::Board(const Board &oldBoard, const Move &playedMove, PlayerID player)
:	_settings(oldBoard._settings)
{
	_nCapture[0] = oldBoard._nCapture[0];
	_nCapture[1] = oldBoard._nCapture[1];
	_nStone[0] = oldBoard._nStone[0];
	_nStone[1] = oldBoard._nStone[1];

	std::memcpy(_board, oldBoard._board, sizeof(_board));
	playStone(playedMove, player);	// Update board
}


bool	Board::_checkWinner(const Move move, PlayerID player) {
	// Todo: add can win by capture to settings
	// if can win by capture
	if (_nCapture[toInt(PlayerID::P1)] >= nCaptureToWin) {
		_winner = Stone::P1;
		return (_done = true);
	}
	if (_nCapture[toInt(PlayerID::P2)] >= nCaptureToWin) {
		_winner = Stone::P2;
		return (_done = true);
	}
	if	(false)  {// Check if we did 5 in a row
		(void)move;
		_winner = player == PlayerID::P1 ? Stone::P1 : Stone::P2;
		return (_done = true);
	}
	return (_done);
}

bool	Board::_makesFree3(const Move move, Stone player) const {
	// Check stuff
	(void)move;
	(void)player;
	return (false);
}

bool	Board::isMoveLegal(const Move move, PlayerID player) const {
	if (move.x >= BoardSize || move.y >= BoardSize)
		return (false);
	if (_board[move.y][move.x] != Stone::NONE)
		return (false);
	if (_settings.free3 && _makesFree3(move, toStone(player)))
		return (false);
	return (true);
}

void	Board::_capture2(const Move move, Stone player) {
	// Loop over all around move
	//		if dir has capture
	// 			Incrise _nCapture[player]
	// 			decrease nStone[otherPlayer]
	(void)move;
	(void)player;
}

void	Board::playStone(const Move move, PlayerID player) {
	if (!isMoveLegal(move, player))
		throw ("Stone played on board is not legal");

	_board[move.x][move.y] = toStone(player);
	++_nStone[toInt(player)];

	if (_settings.capture2)
		_capture2(move, toStone(player));
	
	_checkWinner(move, player);
}

bool	Board::done(void)	const {
	return (_done);
}

Stone	Board::getWinner(void)	const {
	if (_done == false)
		throw (std::runtime_error("GetWinner call when Board is not done"));
	return (_winner);
}
