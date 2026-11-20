#include "analysis/LineView.hpp"

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

// only checks paterns in one direction, so it needs to be called with nView and pView inverted to check every posibilitty
bool	LineView::_posCanBeCaptured(const SideView &nView, const SideView &pView, Stone stoneAt) {
	if (stoneAt == Stone::NONE)
		return (false);
	if (pView.len() < 1 || nView.len() < 2)	// If not enought len
		return (false);
	if (!nView[0].is(1, stoneAt))	// If next to us isnt same stone as us
		return (false);
	// nView[1] isnt of type stoneAt or it would be with nView[0]
	// We must check if pView[0] isnt same as us (if it is, we have 3 / more aligned and cannot be taken)
	// pView[0].stone and nView[1].stone are not of stoneAt type
	// if they are different then one is NONE and the other is opponentStone
	return (pView[0].stone != stoneAt && nView[1].stone != pView[0].stone);
}

// -- 🟥🟦⬜ --
// ⬜🟥🟥🟥⬜⬜
// ⬜🟥🟥⬜🟥⬜
// ⬜🟥⬜🟥🟥⬜
// only checks paterns in one direction, so it needs to be called with nView and pView inverted to check every posibilitty
bool	LineView::_posMakesFree3(const SideView &nView, const SideView &pView, const PlayerID player, const size_t groupLen) {
	// Gives us the index of the first StartView group that isnt of stoneAt type
	constexpr size_t	nStoneForPaterns = Board::MinLenToWin - 2;

	if (nView.size() == 0 || pView.size() == 0 || groupLen > nStoneForPaterns)	// If a side is empty (we are on an edge)
		return (false);

	const size_t	nStart = nView.getIndexNotStone(player);
	const size_t	pStart = pView.getIndexNotStone(player);

	if (nStart >= nView.size() || pStart >= pView.size())	// If no space after the middle block
		return (false);
	if (nView[nStart].stone != Stone::NONE	// If the sides arnt free spaces
			|| pView[pStart].stone != Stone::NONE)
		return (false);

	// From there we know that nView has at least one free stone, so we only need to check pView
	// 	(We are always in the consecutive stone block next to nView)

	// ⬜🟥🟥🟥⬜⬜	-> Check for this patern
	if (groupLen == nStoneForPaterns)
		return (pView[pStart].len >= 2);	// If we have 3 stone, then we need 2+ free space in one direction

	// ⬜🟥🟥⬜🟥⬜	-> Check for those kind of paterns
	// ⬜🟥⬜🟥🟥⬜
	// Check if we have one free space, then nStoneForPaterns - patern 
	if (pView.size() - pStart < 3) // We need one free group, one StoneAt group and one free group, so 3 groups total
		return (false);

	if (pView[pStart].is(1, Stone::NONE) == false) // If we dont have ONE free space
		return (false);
	const Stone	stoneAt = toStone(player);
	if (pView[pStart + 1].is(nStoneForPaterns - groupLen, stoneAt) == false) // Then x matching
		return (false);
	return (pView[pStart + 2].stone == Stone::NONE);	// If we have a group of Stone::NONE after (If there is a group its minimum one len so we dont ened to check that)
}

bool	LineView::canBeCaptured(void) const {
	if (_negativeView.size() == 0 || _positiveView.size() == 0)	// If on extremity
		return (false);
	
	return (_posCanBeCaptured(_negativeView, _positiveView, _stoneAtPos)
			|| _posCanBeCaptured(_positiveView, _negativeView, _stoneAtPos));	
}


bool	LineView::winCanBeCaptured(void) const {
	if (!this->isWin() || _stoneAtPos == Stone::NONE)
		return (false);

	const size_t	extraLen = _groupLen - Board::MinLenToWin;	// Dandling bits on the side (if there is 6 aligned, we can just check the middle 4)
	const size_t	lenToScan = _groupLen - 2 * extraLen;		// 5 -> 5, 6 -> 4 ... The lenght to check
	const ssize_t	iStart = -((!_negativeView.size() || _negativeView[0].stone != _stoneAtPos) ? 0 : _negativeView[0].len) + extraLen;
		
	const StepDir	&vDir = getStepDir();
	for (auto iStep = iStart; iStep < lenToScan; ++iStep) {
		for (uint8_t iDir = 0; iDir < LineView::nDirection; ++iDir) {
			if (static_cast<uint8_t>(_dir) == iDir)
				continue ;

			const Position newPos(_pos.x + vDir.x * iStep, _pos.y + vDir.y * iStep);
			const LineView	LV(_board, static_cast<LineView::Direction>(iDir), newPos);

			if (LV.canBeCaptured())
				return (true);
		}
	}
	return (false); 
}

bool	LineView::isFree4(void)	const {
	if (_stoneAtPos == Stone::NONE || _groupLen != Board::MinLenToWin - 1)
		return (false);

	const size_t	pStart = _positiveView.getIndexNotStone(_stoneAtPos);
	const size_t	nStart = _positiveView.getIndexNotStone(_stoneAtPos);

	if (pStart >= _positiveView.size() && nStart >= _negativeView.size())	// If on edge
		return (false);
	// If Stone::NONE group after on each side
	return (_positiveView[pStart].stone == Stone::NONE && _negativeView[nStart].stone == Stone::NONE);
}


bool	LineView::isFree3(void) const {
	if (_stoneAtPos == Stone::NONE)
		return (false);
	if (_negativeView.size() == 0 || _positiveView.size() == 0)	// If on extremity
		return (false);
	if (_lineLen < (Board::MinLenToWin + 1)) // Or cannot make a 4 allign with 2 free on side because its too small
		return (false);
	if (_groupLen + 2 > Board::MinLenToWin) // If we created a 4, no need to check
		return (false);
	
	const PlayerID	player = _stoneAtPos == Stone::P1 ? PlayerID::P1 : PlayerID::P2;

	return (_posMakesFree3(_negativeView, _positiveView, player, _groupLen)
			|| _posMakesFree3(_positiveView, _negativeView, player, _groupLen));	
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

