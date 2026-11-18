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

SideView	LineView::_getView(const Board &board, const Position pos, const StepDir& sDir) {
	auto	getDistOnAxis = [](const size_t posAxis, const ssize_t stepAxis) -> Board::size_t {
		if (!stepAxis)
			return (Board::BoardSize);
		if (stepAxis > 0)
			return (Board::BoardSize - posAxis - 1);
		else
			return (posAxis);
	};
	const size_t	nStep = std::min(getDistOnAxis(pos.x, sDir.x), getDistOnAxis(pos.y, sDir.y));
	if (!nStep)
		return SideView();
	
	SideView	view;
	
	size_t	groupLen = 1;
	Stone	groupStone = board[pos.x + sDir.x, pos.y + sDir.y];

	for (auto iStep = 2; iStep <= nStep; ++iStep) {
		const size_t nX = pos.x + (sDir.x * iStep);
		const size_t nY = pos.y + (sDir.y * iStep);

		const Stone stoneAtN = board[nX, nY];
		if (stoneAtN == groupStone) {
			++groupLen;
		} else {
			view.push(groupLen, groupStone);
			groupStone = stoneAtN;
			groupLen = 1;
		}
	}
	view.push(groupLen, groupStone);
	return (view);
}

Board::size_t	LineView::_getGroupLen(const SideView &nView, const SideView &pView, Stone stoneAt) {
	if (stoneAt == Stone::NONE)
		return (0);
	const size_t	nLen = (!nView.size() || nView[0].stone != stoneAt) ? 0 : nView[0].len;
	const size_t	pLen = (!pView.size() || pView[0].stone != stoneAt) ? 0 : pView[0].len;

	return (nLen + 1 + pLen);
}

LineView::LineView(const Board &board, Direction dir, const Position pos)
:		_board(board), _pos(pos), _stoneAtPos(board.getStoneAt(pos)), _dir(dir),
		_lineLen(_getLineLen(pos, dir)),
		_iPos(_getIPos(pos, dir)),
		_negativeView(_getView(_board, pos, -LineView::getStepDir(dir))),
		_positiveView(_getView(_board, pos, LineView::getStepDir(dir))),
		_groupLen(_getGroupLen(_negativeView, _positiveView, _stoneAtPos))
{}

LineView::LineView(const Board &board, Direction dir, const Move move, PlayerID playerID)	// Look at a move
:		_board(board), _pos(move), _stoneAtPos(toStone(playerID)), _dir(dir),
		_lineLen(_getLineLen(move, dir)),
		_iPos(_getIPos(move, dir)),
		_negativeView(_getView(_board, move, -LineView::getStepDir(dir))),
		_positiveView(_getView(_board, move, LineView::getStepDir(dir))),
		_groupLen(_getGroupLen(_negativeView, _positiveView, _stoneAtPos))
{}

const SideView	&LineView::getViewNegative(void) const {
	return (_negativeView);
}

const SideView	&LineView::getViewPositive(void) const {
	return (_positiveView);
}

std::ostream	&operator<<(std::ostream &os, const LineView &lineView) {
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

