#include "LineView.hpp"

LineView::DirVector	LineView::dirVec[nDirection] = {
	[static_cast<uint16_t>(Direction::HORIZONTAL)]	= {.x = 1, .y = 0},
	[static_cast<uint16_t>(Direction::VERTICAL)]	= {.x = 0, .y = 1},
	[static_cast<uint16_t>(Direction::PLUS_DIAG)]	= {.x = 1, .y = 1},
	[static_cast<uint16_t>(Direction::MINUS_DIAG)]	= {.x = 1, .y = -1},
};


constexpr uint16_t	LineView::_getLineLen(const Position &pos, const Direction dir) {
	if (dir == Direction::HORIZONTAL || dir == Direction::VERTICAL)
		return (BoardSize);
	const uint16_t	distanceFromMiddleDiag = (pos.x >= pos.y) ? pos.x - pos.y : pos.y - pos.x;
		
	return (BoardSize - distanceFromMiddleDiag);
}

constexpr uint16_t	LineView::_getIPos(const Position &pos, const Direction dir) {
	if (dir == Direction::HORIZONTAL) {
		return (pos.x);
	} else if (dir == Direction::VERTICAL) {
		return (pos.y);
	} else if (dir == Direction::PLUS_DIAG) {
		const uint16_t	distanceFromMiddleDiag = (pos.x >= pos.y) ? pos.x - pos.y : pos.y - pos.x;
		return (BoardSize - distanceFromMiddleDiag);
	} else if (dir == Direction::MINUS_DIAG) {
		const uint16_t	newY = BoardSize - pos.y - 1;
		const uint16_t	distanceFromMiddleDiag = (pos.x >= newY) ? pos.x - newY : newY - pos.x;
		return (BoardSize - distanceFromMiddleDiag);
	}
	return (0);
}

void	LineView::_setGroupLen(void) {
	const DirVector	&vecInDir = dirVec[static_cast<uint16_t>(_dir)];

	uint16_t		groupLenPositive = 0;
	const uint16_t	lenPositive = _lineLen - 1 - _iPos;

	for (uint16_t i = 1; i <= lenPositive; ++i) {
		const uint16_t nX = _pos.x + i * vecInDir.x;
		const uint16_t nY = _pos.y + i * vecInDir.y;
		if (_board.getStoneAt(nX, nY) == _stoneAtPos)
			++groupLenPositive;
		else
			break;
	}
	
	uint16_t		groupLenNegative = 0;
	const uint16_t	lenNegative = _lineLen - lenPositive - 1;

	for (uint16_t i = 1; i <= lenNegative; ++i) {
		const uint16_t nX = _pos.x - i * vecInDir.x;
		const uint16_t nY = _pos.y - i * vecInDir.y;
		if (_board.getStoneAt(nX, nY) == _stoneAtPos)
			++groupLenNegative;
		else
			break;
	}
	_groupLen = 1 + groupLenPositive + groupLenNegative;
}

LineView::LineView(const Board &board, Direction dir, const Position pos)
:		_board(board), _pos(pos), _stoneAtPos(board.getStoneAt(pos)), _dir(dir),
		_lineLen(_getLineLen(pos, dir)),
		_iPos(_getIPos(pos, dir))
{
	if (_stoneAtPos != Stone::NONE)
		_setGroupLen();
}

LineView::LineView(const Board &board, Direction dir, const Move move, PlayerID playerID)	// Look at a move
:		_board(board), _pos(move), _stoneAtPos(toStone(playerID)), _dir(dir),
		_lineLen(_getLineLen(move, dir)),
		_iPos(_getIPos(move, dir))
{
	_setGroupLen();
}
