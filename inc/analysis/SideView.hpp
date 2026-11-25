#pragma once

#include <cstdint>
#include <exception>

#include <iostream>

#include "core/Enums.hpp"
#include "board/Board.hpp"


struct StoneGroup {
	Board::size_t	len;	// Number of stone
	Stone			stone;	// Stone type

	StoneGroup(void)
		:	len(0), stone(Stone::NONE)	{}
	StoneGroup(const Board::size_t _len, const Stone _stone)
		:	len(_len), stone(_stone)	{}
	inline bool	is(Board::size_t _len, Stone _stone) const {return (len == _len && stone == _stone);};
};

std::ostream	&operator<<(std::ostream &os, const StoneGroup &stoneGroup);

class	SideView {
	public:
		using	size_t = Board::size_t;
	private:
		StoneGroup	_arr[Board::BoardSize];
		size_t	_size = 0;
		size_t	_len = 0;

	public:
		SideView(void) {};
		~SideView(void) {};

		size_t	size(void)	const	{	return (_size);	};	// Returns numbers of indexes in the array
		size_t	len(void)	const	{	return (_len);	};	// Returns distance to the end of the board

		inline void	push(const size_t len, const Stone stone) {
			if (_size >= Board::BoardSize)
				throw(std::runtime_error("StoneGroup.push(): _size == N"));
			_arr[_size].stone = stone;
			_arr[_size].len = len;
			_len += len;
			_size++;
		};
		inline void	push(const StoneGroup &stoneGroup) {
			push(stoneGroup.len, stoneGroup.stone);
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

		// Returns first index that isnt Stone from player, returns >= size() on error
		inline size_t	getIndexNotStone(const Stone stone) const {
			if (_size == 0)
				return (0);
			if (_arr[0].stone == stone)	// If we match the center, then skip
				return (1);
			return (0);
		}
		inline size_t	getIndexNotStone(const PlayerID player) const {
			return (getIndexNotStone(toStone(player)));
		}


		bool	makesCapture2(const PlayerID player) const;
};

std::ostream	&operator<<(std::ostream &os, const SideView &sideView);
