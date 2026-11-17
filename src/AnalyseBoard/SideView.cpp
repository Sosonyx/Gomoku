#include "SideView.hpp"

#include <iomanip>

#define CLR_RESET	"\e[0m"

static const char	*stoneToStrClr[3] {
	[static_cast<SideView<>::size_t>(Stone::P1)] = "\e[41m",
	[static_cast<SideView<>::size_t>(Stone::P2)] = "\e[44m",
	[static_cast<SideView<>::size_t>(Stone::NONE)] = "\e[100m",
};

std::ostream	&operator<<(std::ostream &os, const StoneGroup &stoneGroup) {
	if (stoneGroup.len != 0) {
		const char	*clrID = stoneToStrClr[static_cast<SideView<>::size_t>(stoneGroup.stone)];
		os << clrID << std::setw(stoneGroup.len) << "" << CLR_RESET;
	}
	return (os);
}

template <SideView<>::size_t N>
std::ostream	&operator<<(std::ostream &os, const SideView<N> &sideView) {
	os << '[';
	for (auto i = 0; i < sideView.size(); ++i)	
		os << sideView[i];
	os << ']';
	return (os);
}

// bool	makesCapture2(void)	const;

// void	debug(void) const;
