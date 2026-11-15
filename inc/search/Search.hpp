#pragma once

#include "board/Board.hpp"
#include "search/SearchState.hpp"

Move findBestMove(const Board &board, PlayerID player);

class Search {
    public:
        Move findBestMove(const Board &board, PlayerID player);
};