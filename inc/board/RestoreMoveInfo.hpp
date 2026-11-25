#pragma once

#include "board/Move.hpp"
#include "board/Board.hpp"
#include "core/Enums.hpp"
#include "analysis/LineView.hpp"

#include <iostream>
#include <format>

class RestoreMoveInfo
{
	public:
		// The move
		const Move		_move;	
		const PlayerID	_player;	

		// To easly restore lastMoveWinner in case of a capture
		const Stone		_lastMoveWinner = Stone::NONE;	
		const Position	_lastMoveWinningPos = Position(0, 0);

	private:
		// Did the move capture in every 8 direction
		uint8_t		_flagsCapture = 0x00;

	private:
		static constexpr inline uint8_t	_getMaskCapture(const LineView::Direction dir, const LineView::DirSign sign)
		{
			return ((0b1 << static_cast<uint8_t>(dir)) << ((sign == LineView::DirSign::NEGATIVE) ? 4 : 0));
		}

	public:
		RestoreMoveInfo(const Move move, const PlayerID	player, const Stone lastMoveWinner, Position lastMoveWinningPos)
			: _move(move), _player(player), _lastMoveWinner(lastMoveWinner), _lastMoveWinningPos(lastMoveWinningPos) {};

		// Did the Move do a capture
		inline bool	captured(void) const
		{	return (_flagsCapture != 0);	}

		// Check if a direction and sign captured
		inline bool	captured(const LineView::Direction dir, const LineView::DirSign sign) const
		{	return (_flagsCapture & _getMaskCapture(dir, sign));	}

		// Add a capture direction and sign to the info
		inline void	addCapture(const LineView::Direction dir, const LineView::DirSign sign)
		{	_flagsCapture |= _getMaskCapture(dir, sign);	}

		inline friend std::ostream &operator<<(std::ostream &os, const RestoreMoveInfo &RMI)
		{
			using Direction = LineView::Direction;
			using DirSign = LineView::DirSign;
			os << std::format("RMI: [{}:{}], Captured: ", RMI._move.x, RMI._move.y);
		
			if (RMI.captured(Direction::HORIZONTAL, DirSign::POSITIVE))	os << "+Hor ";
			if (RMI.captured(Direction::VERTICAL, DirSign::POSITIVE))	os << "+Ver ";
			if (RMI.captured(Direction::PLUS_DIAG, DirSign::POSITIVE))	os << "+\\ ";
			if (RMI.captured(Direction::MINUS_DIAG, DirSign::POSITIVE))	os << "+/ ";
			if (RMI.captured(Direction::HORIZONTAL, DirSign::NEGATIVE))	os << "-Hor ";
			if (RMI.captured(Direction::VERTICAL, DirSign::NEGATIVE))	os << "-Ver ";
			if (RMI.captured(Direction::PLUS_DIAG, DirSign::NEGATIVE))	os << "-\\ ";
			if (RMI.captured(Direction::MINUS_DIAG, DirSign::NEGATIVE))	os << "-/ ";
			return os << std::endl;
		}
};
