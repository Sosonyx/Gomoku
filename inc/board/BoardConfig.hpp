#pragma once

#include <cstdint>

namespace BoardConfig {
	using ssize_t	= int8_t;
	using size_t	= uint8_t;
	inline constexpr size_t nCaptureToWin	= 5u;
	inline constexpr size_t MinLenToWin		= 5u;
	inline constexpr size_t BoardSize		= 19u;
}
