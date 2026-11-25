#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "core/Enums.hpp"
#include "board/BoardConfig.hpp"
#include "board/Move.hpp"

class RestoreMoveInfo;

struct Settings;

class Board
{
	public:
		using	ssize_t	= BoardConfig::ssize_t;
		using	size_t	= BoardConfig::size_t;

		static inline constexpr size_t	nCaptureToWin	= BoardConfig::nCaptureToWin;
		static inline constexpr size_t	BoardSize		= BoardConfig::BoardSize;
		static inline constexpr size_t	MinLenToWin		= BoardConfig::MinLenToWin;


		template<typename T>
		using BoardArr = T[BoardSize][BoardSize];

	private:
		BoardArr<Stone> 	_board;

		size_t		_nCapture[2] = {0, 0};
		uint16_t	_nStone[2]  = {0, 0};

		// Did we have a winner last move
		Stone		_lastMoveWinner = Stone::NONE;
		Position	_lastMoveWinningPos = Position(0, 0);

		Stone	_winner = Stone::NONE;
		bool	_done = false;

		const Settings	&_settings;

	private:

		bool	_makes2Free3(const Move move, const PlayerID _player)	const;
		// Handle captures
		void	_capture2(const Move move, const PlayerID _player, RestoreMoveInfo *pRMI = nullptr);
	
		void	_playStone(const Move move, const PlayerID player, RestoreMoveInfo *pRMI = nullptr);
		
		Stone	_positionHas5(const Position pos) const;
		Stone	_positionHas5(const size_t x, const size_t y) const
		{	return (_positionHas5(Position(x, y)));	};

		void	_iterOnWinPosition(void);

		bool	_checkLastMoveWin(void);
		bool	_checkWinBy5(const Position pos, const PlayerID player);
		bool	_checkWinByCapture(void);

		bool	_checkWinner(const Position pos, const PlayerID player);

		inline bool	_setWinner(const Stone stone = Stone::NONE) {
			_winner = stone;
			_done = true;
			return (true);
		}
		inline bool	_setWinner(const PlayerID player) {
			return (_setWinner(toStone(player)));
		}

		// TODO: Maybe have every single view in a member, update them so you dont have to ask
		// Todo: Look at the type system and if it could help me
		void	_updateInsideAnalyse(const Position pos);

		// Updates stone count based on previous stone
		// must be called to change _board, expects a valid position
		inline void	_placeStoneAt(const Position pos, const Stone stone) {
			const Stone	oldStone = getStoneAt(pos);
	
			if (stone == oldStone)
				return;
			_board[pos.x][pos.y] = stone;
			if (stone != Stone::NONE)
				++_nStone[toInt(toPlayer(stone))];
			if (oldStone != Stone::NONE)
				--_nStone[toInt(toPlayer(oldStone))];
			_updateInsideAnalyse(pos);
		}

		inline void	_placeStoneAt(const size_t x, const size_t y, const Stone stone) {
			_placeStoneAt(Position(x, y), stone);
		}

	public:
		Board(const Settings &settings);
		Board(const Board &oldBoard) = default;

		bool	hasLegalMove(const PlayerID player)	const;

		bool	isMoveLegal(const Move move, const PlayerID player)	const;
		void	playStone(const Move move, const PlayerID player);

		// Get RestoreMoveInfo of a Move
		RestoreMoveInfo	RMI_playStone(const Move move, const PlayerID player);
		// Restore the board from a Move
		void			RestoreMove(const RestoreMoveInfo RMI);
		
		inline Stone	getStoneAt(const size_t x, const size_t y) const{
			if (x >= BoardSize || y >= BoardSize)
				throw (std::runtime_error("getStone call is OOB"));
			return (_board[x][y]);
		}
		inline Stone	getStoneAt(const Position &pos) const{
			if (pos.x >= BoardSize || pos.y >= BoardSize)
				throw (std::runtime_error("getStoneAt call is OOB"));
			return (_board[pos.x][pos.y]);
		}
		inline Stone	operator[](const size_t x, const size_t y) const{
			if (x >= BoardSize || y >= BoardSize) 
				throw (std::runtime_error("const Board[x, y] call is OOB"));
			return (_board[x][y]);
		}

		inline bool	empty(void)	const {	return (_nStone[0] == 0 && _nStone[1] == 0);	}
		inline bool	done(void)	const {	return (_done);	}

		inline Stone	getWinner(void)	const {
			if (_done == false)
				throw (std::runtime_error("GetWinner call when Board is not done"));
			switch (_winner) {
				case (Stone::NONE):
					std::cout << " -- Tie -- " << std::endl;
					break ;
				case (Stone::P1):
					std::cout << " -- P1 -- " << std::endl;
					break ;
				case (Stone::P2):
					std::cout << " -- P2 -- " << std::endl;
					break ;
			}
			return (_winner);
		}

		uint64_t evaluate(const Board &b, PlayerID p);
		uint16_t getCapture(PlayerID player) const;

		friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
