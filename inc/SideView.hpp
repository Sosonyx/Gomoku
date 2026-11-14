#pragma once

#include <cstdint>
#include <exception>

#include "Enums.hpp"
#include "Board.hpp"

struct StoneGroup {
	const uint16_t	len;
	const StoneSide	stone;

	StoneGroup(uint16_t len, StoneSide stone)
		:	len(len), stone(stone)	{}
	
	inline bool	operator==(const StoneGroup &other) {
		return (stone == other.stone && len == other.len);
	}
	inline bool	operator>=(const StoneGroup &other) {
		return (stone == other.stone && len >= other.len);
	}
	inline bool	operator<=(const StoneGroup &other) {
		return (stone == other.stone && len <= other.len);
	}
	inline bool	operator>(const StoneGroup &other) {
		return (stone == other.stone && len > other.len);
	}
	inline bool	operator<(const StoneGroup &other) {
		return (stone == other.stone && len < other.len);
	}
};

template <std::uint16_t N = BoardSize>
class	SideView {
	private:
		StoneGroup	_arr[N] = {};
		uint16_t	_size = 0;
		uint16_t	_len = 0;

	public:
		SideView(void) {};
		~SideView(void) {};

		uint16_t	size(void)	const	{	return (_size);	};
		uint16_t	len(void)	const	{	return (_len);	};

		uint16_t	push(const StoneGroup &stoneGroup) {
			if (_size >= N)
				throw(std::runtime_error("StoneGroup.push(): _size == N"));
			_arr[_size] = stoneGroup;
			_len += stoneGroup.len;
			_size++;
		};
		void	pop(void) {
			if (_size == 0)
				throw(std::runtime_error("StoneGroup.pop(): _size == 0"));
			_size--;
			_len -= _arr[_size].len;
		};

		inline StoneGroup	&operator[](uint16_t n) {
			if (n >= _size)
				throw(std::runtime_error("StoneGroup[] call is OOB"));
			return (_arr[n]);
		};
		inline const StoneGroup	&operator[](uint16_t n) const {
			if (n >= _size)
				throw(std::runtime_error("StoneGroup[] call is OOB"));
			return (_arr[n]);
		}
};
