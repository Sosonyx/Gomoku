#pragma once

#include <cstdint>
#include <compare>


enum GameResult : uint8_t 
{
	NotDone,
	Lose,
	Tie,
	Win,
};

// Idea is to store a move score, result (if it ends un in a lose, tie or win)
//	and give a simple way of comparing them to chose the best
//  opfully allows us to use std::algorithm on it

struct Score
{
	int64_t		score = 0;
	GameResult	result = GameResult::NotDone;

	inline Score(void)	{};
	inline Score(int64_t _score): score(_score) {};
	inline Score(const Score &other): score(other.score), result(other.result) {};
	Score &operator=(const Score &other);

	auto	operator<=>(const Score &other) const;

	inline constexpr bool	done(void) const
	{	return (result != GameResult::NotDone);	}
	
	inline constexpr Score &operator-=(const int64_t _score)
	{	score += _score;	return (*this);	}

	inline constexpr Score &operator+=(const int64_t _score)
	{	score += _score;	return (*this);	}
};
