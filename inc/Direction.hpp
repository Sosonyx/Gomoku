#pragma once

#include <cstdint>

class Direction {
	public:
		enum : uint8_t {
			TOP,
			TOP_RIGHT,
			RIGHT,
			BOT_RIGHT,
			BOT,
			BOT_LEFT,
			LEFT,
			TOP_LEFT
		};

	private:
		uint8_t	_direction;

	public:
		Direction(uint8_t directionHORIZONTAL);

		uint8_t	next(void);
};
