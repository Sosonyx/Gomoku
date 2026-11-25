#pragma once

#include <ranges>
#include <iostream>

#include "board/BoardConfig.hpp"

struct Move
{
	using	size_t = BoardConfig::size_t;
	size_t x = 0;
	size_t y = 0;
	inline constexpr Move(void) noexcept = default;
	inline constexpr Move(const size_t _x,const size_t _y) noexcept:	x(_x), y(_y)  {}

	friend constexpr bool	operator==(const Move &moveA, const Move &moveB) noexcept = default;

	static const Move	InvalidMove;

	static constexpr auto	ViewAll(void) noexcept {
		constexpr size_t BoardSize = BoardConfig::BoardSize;
		return (std::views::iota(0, BoardSize * BoardSize)| std::views::transform([](int i) {
                   return (Move(static_cast<size_t>(i % BoardSize), static_cast<size_t>(i / BoardSize)));
				})
		);
	}

	friend std::ostream	&operator<<(std::ostream &os, const Move &move);
};

using Position = Move;
