#include "Board.hpp"
#include "Game.hpp"
#include "LineView.hpp"

#include <stdexcept>

Board::Board(const Settings &settings)
:	_settings(settings)
{
	std::memset(_board, static_cast<uint8_t>(Stone::NONE), sizeof(_board));
}

Board::Board(const Board &oldBoard, const Move &playedMove, const PlayerID player)
:	_settings(oldBoard._settings)
{
	_nCapture[0] = oldBoard._nCapture[0];
	_nCapture[1] = oldBoard._nCapture[1];
	_nStone[0] = oldBoard._nStone[0];
	_nStone[1] = oldBoard._nStone[1];

	std::memcpy(_board, oldBoard._board, sizeof(_board));
	playStone(playedMove, player);	// Update board
}

Stone	Board::_positionHas5(const Position pos) const {
	if (getStoneAt(pos) == Stone::NONE)
		return (Stone::NONE);

	for (uint16_t i = 0; i < LineView::nDirection; ++i) {
		LineView	LV(*this, static_cast<LineView::Direction>(i), pos);
		if (LV.getGroupLen() >= 5)
			return (getStoneAt(pos));
	}
	return (Stone::NONE);
}


void	Board::_iterOnWinPosition(void) {
	constexpr uint16_t LenWin = 5;
	static constexpr uint16_t	IndexToCheckByCol[LenWin] = {4, 1, 3, 0, 2};

	for (uint16_t x = 0; x < BoardSize; ++x) {
		for (uint16_t y = IndexToCheckByCol[x % LenWin]; y < BoardSize; y += LenWin) {
			playStone(Position(x, y), PlayerID::P1);
		}
	}
	
}

bool	Board::_checkWinBy5(const Position pos, const PlayerID player) {
	if (_lastMoveWinner != Stone::NONE) {	// If last turn was winning for the other
		Stone checkAgain = _positionHas5(_lastMoveWinningPos);
		if (_lastMoveWinner == checkAgain) {	// Check again the winner position
			return (_setWinner(_lastMoveWinner));
		}
		_lastMoveWinner = Stone::NONE;
	}

	if (_positionHas5(pos) == toStone(player)) {
		if (true)  {// One of the made 5 cannot be taken
			return (_setWinner(player));
		} else { // Save position for next turn
			_lastMoveWinner = toStone(player);
			_lastMoveWinningPos = pos;
		}
	}
	return (_done);
}

bool	Board::_checkWinByCapture(void) {
	{		// If can win by capture
		if (_nCapture[toInt(PlayerID::P1)] >= nCaptureToWin)	return (_setWinner(PlayerID::P1));
		if (_nCapture[toInt(PlayerID::P2)] >= nCaptureToWin)	return (_setWinner(PlayerID::P2));
	}
	return (_done);
}


bool	Board::_checkWinner(const Position pos, const PlayerID player) {
	// Todo: add can win by capture to settings
	// if can win by capture
	do {
		if (_checkWinBy5(pos, player))
			break ;
		if (_checkWinByCapture())
			break ;
	} while(0);
	return (_done);
}

void	Board::_capture2(const Position pos, const Stone player) {
	// Loop over all around move
	//		if dir has capture
	// 			Incrise _nCapture[player]
	// 			decrease nStone[otherPlayer]
	(void)pos;
	(void)player;
}

bool	Board::_makesFree3(const Move move, const Stone player) const {
	// Check stuff
	(void)move;
	(void)player;
	return (false);
}

bool	Board::isMoveLegal(const Move move, const PlayerID player) const {
	if (move.x >= BoardSize || move.y >= BoardSize)
		return (false);
	if (_board[move.x][move.y] != Stone::NONE)
		return (false);
	if (_settings.free3 && _makesFree3(move, toStone(player)))
		return (false);
	return (true);
}

bool	Board::hasLegalMove(PlayerID player)	const {
	// Iter on nonStone pos
	// if no free3
	// return true
	(void)player;
	return (false);
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
	switch (_winner) {
		case (Stone::NONE):
			std::cout << " -- Tie -- " << std::endl;
			break ;
		case (Stone::P1):
			std::cout << " -- P1 -- " << std::endl;
			break ;
		case (Stone::P2):
			std::cout << " -- P2 -- " << std::endl;
			break ;
	}
	return (_winner);
}
