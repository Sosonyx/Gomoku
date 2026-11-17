#include "LineView.hpp"


constexpr Board::size_t	LineView::_getLineLen(const Position &pos, const Direction dir) {
	if (dir == Direction::HORIZONTAL || dir == Direction::VERTICAL)
		return (Board::BoardSize);
	const Board::size_t	nY = (dir == Direction::MINUS_DIAG) ? (Board::BoardSize - pos.y - 1) : pos.y;
	const Board::size_t	distanceFromMiddleDiag = (pos.x >= nY) ? pos.x - nY : nY - pos.x;
		
	return (Board::BoardSize - distanceFromMiddleDiag);
}

constexpr Board::size_t	LineView::_getIPos(const Position &pos, const Direction dir) {
	if (dir == Direction::HORIZONTAL) {
		return (pos.x);
	} else if (dir == Direction::VERTICAL) {
		return (pos.y);
	} else if (dir == Direction::PLUS_DIAG) {
		const Board::size_t	distanceFromMiddleDiag = (pos.x < pos.y) ? pos.x : pos.y;
		return (distanceFromMiddleDiag);
	} else if (dir == Direction::MINUS_DIAG) {
		const Board::size_t	newY = Board::BoardSize - pos.y - 1;
		const size_t	distanceFromMiddleDiag = (pos.x < newY) ? pos.x : newY;
		return (distanceFromMiddleDiag);
	}
	return (0);
}

void	LineView::_setGroupLen(void) {
	const StepDir	&vecInDir = dirVec[static_cast<uint16_t>(ViewDirection::POSITIVE)][static_cast<uint16_t>(_dir)];

	const Board::size_t	lenPositive = _lineLen - 1 - _iPos;
	Board::size_t		groupLenPositive = 0;

	for (auto i = 1; i <= lenPositive; ++i) {
		const Board::size_t nX = _pos.x + i * vecInDir.x;
		const Board::size_t nY = _pos.y + i * vecInDir.y;
		if (_board.getStoneAt(nX, nY) == _stoneAtPos)
			++groupLenPositive;
		else
			break;
	}
	
	const Board::size_t	lenNegative = _iPos;
	Board::size_t		groupLenNegative = 0;

	for (auto i = 1; i <= lenNegative; ++i) {
		const Board::size_t nX = _pos.x - i * vecInDir.x;
		const Board::size_t nY = _pos.y - i * vecInDir.y;
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

void	LineView::_makeView(const ViewDirection viewDir) {
	SideView	*view = nullptr;
	if (viewDir == ViewDirection::NEGATIVE) {	// If already created
		if (_negativeCreated)	return ;
		_negativeCreated = true;
		view = &_negativeView;
	} else {
		if (_positiveCreated)	return ;
		_positiveCreated = true;
		view = &_positiveView;
	}

	const size_t	nStep = (viewDir == ViewDirection::NEGATIVE) ? _iPos : _lineLen - _iPos - 1;
	if (!nStep)	return ;	// If at endOfLine

	// const ssize_t	step = (viewDir == ViewDirection::NEGATIVE) ? -1 : 1;
	const auto		&vecDirX = dirVec[static_cast<uint16_t>(viewDir)][static_cast<uint16_t>(_dir)].x;
	const auto		&vecDirY = dirVec[static_cast<uint16_t>(viewDir)][static_cast<uint16_t>(_dir)].y;
	
	
	size_t	groupLen = 1;
	Stone		groupStone = _board.getStoneAt(_pos.x + vecDirX, _pos.y + vecDirY);

	for (auto iStep = 2; iStep <= nStep; ++iStep) {
		const size_t nX = _pos.x + (vecDirX * iStep);
		const size_t nY = _pos.y + (vecDirY * iStep);

		const Stone stoneAtN = _board.getStoneAt(nX, nY);
		if (stoneAtN == groupStone) {
			++groupLen;
		} else {
			view->push(groupLen, groupStone);
			groupStone = stoneAtN;
			groupLen = 1;
		}
	}
	view->push(groupLen, groupStone);
}

bool LineView::negativeMakesCapture2(void){
	if (!_negativeCreated)	_makeView(ViewDirection::NEGATIVE);

	return (false);
}

bool LineView::positiveMakesCapture2(void){
	if (!_positiveCreated)	_makeView(ViewDirection::POSITIVE);
	
	return (false);
}

const SideView	&LineView::getViewNegative(void) {
	if (!_negativeCreated)	_makeView(ViewDirection::NEGATIVE);
	return (_negativeView);
}

const SideView	&LineView::getViewPositive(void) {
	if (!_positiveCreated)	_makeView(ViewDirection::POSITIVE);
	return (_positiveView);
}

std::ostream	&operator<<(std::ostream &os, LineView &lineView) {
	static const char	*directionToStrClr[LineView::nDirection] {
		[static_cast<int>(LineView::Direction::HORIZONTAL)]	= "HORIZONTAL",
		[static_cast<int>(LineView::Direction::VERTICAL)]	= "VERTICAL",
		[static_cast<int>(LineView::Direction::PLUS_DIAG)]	= "PLUS_DIAG",
		[static_cast<int>(LineView::Direction::MINUS_DIAG)]	= "MINUS_DIAG",
	};
	static const char	*stoneToStrClr[3] {
		[static_cast<int>(Stone::P1)] = "\e[41m",
		[static_cast<int>(Stone::P2)] = "\e[44m",
		[static_cast<int>(Stone::NONE)] = "\e[100m",
	};
	const auto	nView = lineView.getViewNegative();
	const auto	pView = lineView.getViewPositive();
	const char	*clrID = stoneToStrClr[static_cast<int>(lineView._stoneAtPos)];

	os << directionToStrClr[static_cast<int>(lineView._dir)] << " ";
	os << nView;
	os << clrID << " " "\e[0m";
	os << pView;
	os << " (" << static_cast<int>(lineView._iPos) <<" | 1 | " << static_cast<int>(lineView._lineLen - lineView._iPos - 1) << ")";
	os << std::endl;
	return (os);
}

