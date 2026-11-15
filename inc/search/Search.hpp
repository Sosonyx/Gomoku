#pragma once

#include "board/Board.hpp"
#include "SearchState.hpp"

class Search {
    public:
        Search();
        ~Search();
        Move findBestMove(const Board &board, PlayerID player);
};