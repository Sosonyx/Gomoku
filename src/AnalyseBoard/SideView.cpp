#include "SideView.hpp"

#include <iomanip>

#define CLR_RESET	"\e[0m"

static const char	*stoneToStrClr[3] {
	[static_cast<int>(Stone::P1)] = "\e[41m",
	[static_cast<int>(Stone::P2)] = "\e[44m",
	[static_cast<int>(Stone::NONE)] = "\e[100m",
};

std::ostream	&operator<<(std::ostream &os, const StoneGroup &stoneGroup) {
	if (stoneGroup.len != 0) {
		const char	*clrID = stoneToStrClr[static_cast<int>(stoneGroup.stone)];
		os << clrID << std::setw(stoneGroup.len) << "" << CLR_RESET;
	}
	return (os);
}

std::ostream	&operator<<(std::ostream &os, const SideView &sideView) {
	os << '[';
	for (auto i = 0; i < sideView.size(); ++i)	
		os << sideView[i];
	os << ']';
	return (os);
}

bool	SideView::makesCapture2(const PlayerID player) const {
	if (_len < 3 || _size < 2) {	// If not enought space / diferent stoneType
		return (false);
	}
	// If 2 of other then one Of me
	const Stone otherStone = (player == PlayerID::P1) ? Stone::P2 : Stone::P1;
	return (_arr[0].len == 2
			&& _arr[0].stone == otherStone
			&& _arr[1].stone == toStone(player));
}

