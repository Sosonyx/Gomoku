#pragma once

#include <cstdint>
#include <exception>

#include <iostream>

#include "Enums.hpp"
#include "Board.hpp"


struct StoneGroup {
	Board::size_t	len;
	Stone			stone;

	StoneGroup(void)
		:	len(0), stone(Stone::NONE)	{}
	StoneGroup(uint16_t _len, Stone _stone)
		:	len(_len), stone(_stone)	{}
	
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

std::ostream	&operator<<(std::ostream &os, const StoneGroup &stoneGroup);

template <Board::size_t N = Board::BoardSize>
class	SideView {
	public:
		using	size_t = Board::size_t;
	private:
		StoneGroup	_arr[N];
		size_t	_size = 0;
		size_t	_len = 0;

	public:
		SideView(void) {};
		~SideView(void) {};

		size_t	size(void)	const	{	return (_size);	};	// Returns numbers of indexes in the array
		size_t	len(void)	const	{	return (_len);	};	// Returns distance to the end of the board

		inline void	push(const size_t len, const Stone stone) {
			if (_size >= N)
				throw(std::runtime_error("StoneGroup.push(): _size == N"));
			_arr[_size].stone = stone;
			_arr[_size].len = len;
			_len += len;
			_size++;
		};
		inline void	push(const StoneGroup &stoneGroup) {
			if (_size >= N)
				throw(std::runtime_error("StoneGroup.push(): _size == N"));
			_arr[_size] = stoneGroup;
			_len += stoneGroup.len;
			_size++;
		};
		inline void	pop(void) {
			if (_size == 0)
				throw(std::runtime_error("StoneGroup.pop(): _size == 0"));
			_size--;
			_len -= _arr[_size].len;
		};

		inline StoneGroup	&operator[](size_t n) {
			if (n >= _size)
				throw(std::runtime_error("StoneGroup[] call is OOB"));
			return (_arr[n]);
		};
		inline const StoneGroup	&operator[](size_t n) const {
			if (n >= _size)
				throw(std::runtime_error("const StoneGroup[] call is OOB"));
			return (_arr[n]);
		}

		bool	makesCapture2(void)	const;
};

template <SideView<>::size_t N = Board::BoardSize>
std::ostream	&operator<<(std::ostream &os, const SideView<N> &sideView);
