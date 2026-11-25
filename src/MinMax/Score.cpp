#include "Score.hpp"

Score &Score::operator=(const Score &other)
{
	if (this != &other)
	{
		score = other.score;
		result = other.result;
	}
	return (*this);
}

std::weak_ordering	Score::operator<=>(const Score &other) const
{
	if (this->done() && other.done())
		return (result <=> other.result);
	else if (done())
	{
		if (result == GameResult::Win)			return (weak_ordering::greater);
		else if (result == GameResult::Lose)	return (weak_ordering::lower);
	}
	else if (other.done())
	{

	}
	return std::weak_ordering();
}
