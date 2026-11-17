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
	const DirVector	&vecInDir = dirVec[static_cast<uint16_t>(_dir)];

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
	SideView<Board::BoardSize>	*view = nullptr;
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

	const ssize_t	step = (viewDir == ViewDirection::NEGATIVE) ? -1 : 1;
	const auto		&vecDirX = dirVec[static_cast<uint16_t>(_dir)].x * step;
	const auto		&vecDirY = dirVec[static_cast<uint16_t>(_dir)].y * step;
	
	
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

const SideView<Board::BoardSize>	&LineView::getViewNegative(void) {
	if (!_negativeCreated)	_makeView(ViewDirection::NEGATIVE);
	return (_negativeView);
}

const SideView<Board::BoardSize>	&LineView::getViewPositive(void) {
	if (!_positiveCreated)	_makeView(ViewDirection::POSITIVE);
	return (_positiveView);
}

void	LineView::debug(void) const {
	const size_t	lenPositive = _lineLen - 1 - _iPos;
	const size_t	lenNegative = _iPos;

	std::cout << "Line Len: " 		<< static_cast<uint16_t>(_lineLen) << std::endl;
	std::cout << "Negative Len: "	<< static_cast<uint16_t>(lenNegative) << std::endl;
	std::cout << "Positive Len: "	<< static_cast<uint16_t>(lenPositive) << std::endl;
	std::cout << "Group Len: " 		<< static_cast<uint16_t>(_groupLen) << std::endl << std::endl;
}
