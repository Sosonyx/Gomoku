#pragma once

#include <cstdint>

#include "board/Board.hpp"
#include "core/Enums.hpp"

constexpr int SRange = 2;           // Search range around a stone

struct SearchState {
    Board::BoardArr<Stone>   sGrid; // Just a copy of the board, to avoid using the board class 
    Board::BoardArr<uint8_t> sNear; // Each case represents the number of stones that has that case in its range
    int                      nStones;

    SearchState();
    SearchState(Board const &board);

    void place(int x, int y, Stone player);
    void unplace(int x, int y);

    int generateMoves(Move moves[]) const;

};