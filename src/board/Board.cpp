#include "board/Board.hpp"
#include "game/Game.hpp"
#include "analysis/LineView.hpp"
#include "board/RestoreMoveInfo.hpp"

#include <stdexcept>

Board::Board(const Settings &settings)
:	_settings(settings)
{
	std::memset(_board, static_cast<uint8_t>(Stone::NONE), sizeof(_board));
}

Stone	Board::_positionHas5(const Position pos) const {
	if (getStoneAt(pos) == Stone::NONE)
		return (Stone::NONE);

	for (auto i = 0; i < LineView::nDirection; ++i) {
		LineView	LV(*this, static_cast<LineView::Direction>(i), pos);
		
		if (LV.isWin())
			return (getStoneAt(pos));
	}
	return (Stone::NONE);
}

void	Board::_iterOnWinPosition(void) {
	static constexpr size_t	IndexToCheckByCol[MinLenToWin] = {4, 1, 3, 0, 2};	// HardCoded ? care if MinLenToWin is changed

	for (auto x = 0; x < BoardSize; ++x) {
		for (auto y = IndexToCheckByCol[x % Board::MinLenToWin]; y < BoardSize; y += Board::MinLenToWin) {
			playStone(Position(x, y), PlayerID::P1);
		}
	}
	
}

// Check if we took the 5 aligned from previous play
bool	Board::_checkLastMoveWin(void) {
	if (_lastMoveWinner != Stone::NONE) {	// If last turn was winning for the other
		Stone checkAgain = _positionHas5(_lastMoveWinningPos);
		if (_lastMoveWinner == checkAgain) {	// Check again the winner position
			return (_setWinner(_lastMoveWinner));
		}
		_lastMoveWinner = Stone::NONE;
	}
	return (false);
}

// Check if we did a 5 that cannot be taken, if a 5 can be taken then store in in lastMoveWinner
bool	Board::_checkWinBy5(const Position pos, const PlayerID player) {
	bool	winCanBeCaptured = false;

	for (auto i = 0; i < LineView::nDirection; ++i) {
		const LineView	LV(*this, static_cast<LineView::Direction>(i), pos);

		if (LV.isWin()) {
			if (LV.winCanBeCaptured()) {
				winCanBeCaptured = true;	// Check next turn
			} else {
				return (_setWinner(player));
			}
		}
	}
	if (winCanBeCaptured) {
		_lastMoveWinner = toStone(player);
		_lastMoveWinningPos = pos;
	}
	return (false);
}

bool	Board::_checkWinByCapture(void) {
	{		// TODO: If can win by capture
		if (_nCapture[toInt(PlayerID::P1)] >= nCaptureToWin)	return (_setWinner(PlayerID::P1));
		if (_nCapture[toInt(PlayerID::P2)] >= nCaptureToWin)	return (_setWinner(PlayerID::P2));
	}
	return (_done);
}


bool	Board::_checkWinner(const Position pos, const PlayerID player) {
	// Todo: add can win by capture to settings
	// if can win by capture
	_checkLastMoveWin() || _checkWinBy5(pos, player) || _checkWinByCapture();
	return (_done);
}

void	Board::_capture2(const Position pos, const PlayerID player, RestoreMoveInfo *pRMI) {
	if (!_settings.capture2)	return ;
	
	for (auto i = 0; i < LineView::nDirection; ++i) {
		const LineView::Direction	dir = static_cast<LineView::Direction>(i);
		const LineView	LV(*this, dir, pos);
	
		auto nView = LV.getViewNegative();
		auto pView = LV.getViewPositive();

		if (nView.makesCapture2(player)) {
			const StepDir	stepDirN = -LV.getStepDir();

			++_nCapture[toInt(player)];
			_placeStoneAt(pos.x + stepDirN.x, pos.y + stepDirN.y, Stone::NONE);			
			_placeStoneAt(pos.x + 2 * stepDirN.x, pos.y + 2 * stepDirN.y, Stone::NONE);
			if (pRMI != nullptr)	pRMI->addCapture(dir, LineView::DirectionSign::NEGATIVE);
		}
		if (pView.makesCapture2(player)) {
			const StepDir	stepDirP = LV.getStepDir();
			
			++_nCapture[toInt(player)];
			_placeStoneAt(pos.x + stepDirP.x, pos.y + stepDirP.y, Stone::NONE);			
			_placeStoneAt(pos.x + 2 * stepDirP.x, pos.y + 2 * stepDirP.y, Stone::NONE);			
			if (pRMI != nullptr)	pRMI->addCapture(dir, LineView::DirectionSign::POSITIVE);
		}
	}
}

bool	Board::_makes2Free3(const Move move, const PlayerID player) const {
	constexpr uint16_t	maxFree3 = 2;
	uint16_t			nFree3 = 0;

	for (auto i = 0; i < LineView::nDirection; ++i) {
		const LineView	LV(*this, static_cast<LineView::Direction>(i), move, player);

		if (LV.isFree4() || LV.isWin())	// If move makes a free win
			return (false);
		if (nFree3 < maxFree3 && LV.isFree3())
			++nFree3;
	}
	return (nFree3 >= maxFree3);
}

bool	Board::isMoveLegal(const Move move, const PlayerID player) const {
	if (move.x >= Board::BoardSize || move.y >= Board::BoardSize)
		return (false);
	if (_board[move.x][move.y] != Stone::NONE)
		return (false);
	if (_settings.free3 && _makes2Free3(move, player))
		return (false);
	return (true);
}

bool	Board::hasLegalMove(const PlayerID player)	const {
	for (auto x = 0; x < Board::BoardSize; ++x) {
		for (auto y = 0; y < Board::BoardSize; ++y) {
			if (_board[x][y] != Stone::NONE)
				continue ;
			if (isMoveLegal(Position(x, y), player))
				return (true);
		}
	}
	return (false);
}

void	Board::_playStone(const Move move, const PlayerID player, RestoreMoveInfo *pRMI)
{
	if (!isMoveLegal(move, player))
		throw (std::runtime_error("Stone played on board is not legal"));

	_placeStoneAt(move, toStone(player));

	if (_settings.capture2)
		_capture2(move, player, pRMI);
	_checkWinner(move, player);
}

void	Board::playStone(const Move move, const PlayerID player)
{
	_playStone(move, player);
}

RestoreMoveInfo	Board::RMI_playStone(const Move move, const PlayerID player)
{
	RestoreMoveInfo	RMI(move, player, _lastMoveWinner, _lastMoveWinningPos);
	_playStone(move, player, &RMI);
	return (RMI);
}
void			Board::RestoreMove(const RestoreMoveInfo RMI)
{
	const Stone	stoneMove = getStoneAt(RMI._move);
	if (stoneMove == Stone::NONE)
		throw ("Board::RestoreMove called with a nonexisting move");

	std::cout << *this;
	const PlayerID	playerMove = toPlayer(stoneMove);

	_placeStoneAt(RMI._move, Stone::NONE);
	_lastMoveWinner = RMI._lastMoveWinner;
	_lastMoveWinningPos = RMI._lastMoveWinningPos;

	_winner = Stone::NONE;
	_done = false;

	if (RMI.captured())
	{
		const Move		move = RMI._move;
		const Stone		stoneOther = getOther(stoneMove);

		for (auto i = 0; i < LineView::nDirection; ++i) {
			const LineView::Direction	dir = static_cast<LineView::Direction>(i);
			const StepDir	stepDir = LineView::getStepDir(dir);

			if (RMI.captured(dir, LineView::DirSign::POSITIVE)) {
				_placeStoneAt(move.x + stepDir.x, move.y + stepDir.y, stoneOther);			
				_placeStoneAt(move.x + 2 * stepDir.x, move.y + 2 * stepDir.y, stoneOther);
				--_nCapture[toInt(playerMove)];
			}
			if (RMI.captured(dir, LineView::DirSign::NEGATIVE)) {
				_placeStoneAt(move.x - stepDir.x, move.y - stepDir.y, stoneOther);			
				_placeStoneAt(move.x - 2 * stepDir.x, move.y - 2 * stepDir.y, stoneOther);			
				--_nCapture[toInt(playerMove)];
			}
		}
		std::cout << *this;
	}
}

void	Board::_updateInsideAnalyse(const Position pos) {
	(void)pos;
}

uint16_t Board::getCapture(PlayerID player) const {
	return (_nCapture[toInt(player)]);
}

