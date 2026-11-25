#include <format>

#include "board/Move.hpp"

const Move	Move::InvalidMove(static_cast<Move::size_t>(-1), static_cast<Move::size_t>(-1));

std::ostream	&operator<<(std::ostream &os, const Move &move) {
	return os << std::format("[ {}, {} ]", move.x, move.y);
}
