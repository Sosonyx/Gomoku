#include "search/SearchState.hpp"

#include <cstring>

SearchState::SearchState()
:	nStones(0)
{
	std::memset(sGrid, static_cast<uint8_t>(Stone::NONE), sizeof(sGrid));
	std::memset(sNear, 0, sizeof(sNear));
}

SearchState::SearchState(const Board &board)
:	nStones(0)
{
	std::memset(sGrid, static_cast<uint8_t>(Stone::NONE), sizeof(sGrid));
	std::memset(sNear, 0, sizeof(sNear));

	for (int x = 0; x < Board::BoardSize; ++x)
		for (int y = 0; y < Board::BoardSize; ++y) {
			Stone s = board.getStoneAt(x, y);
			if (s != Stone::NONE)
				place(x, y, s);
		}
}

void SearchState::place(int x, int y, Stone player) {
    sGrid[x][y] = player;
    nStones++;
    for (int i = -SRange; i <= SRange; i++) {
        for (int j = -SRange; j <= SRange; j++) {
            if (i == 0 && j == 0) continue;
            if (x + i >= 0 && x + i < Board::BoardSize && y + j >= 0 && y + j < Board::BoardSize) {
                sNear[x + i][y + j]++;
            }
        }
    }
}

void SearchState::unplace(int x, int y) {
    sGrid[x][y] = Stone::NONE;
    nStones--;
    for (int i = -SRange; i <= SRange; i++) {
        for (int j = -SRange; j <= SRange; j++) {
            if (i == 0 && j == 0) continue;
            if (x + i >= 0 && x + i < Board::BoardSize && y + j >= 0 && y + j < Board::BoardSize) {
                sNear[x + i][y + j]--;
            }
        }
    }
}

int SearchState::generateMoves(Move moves[]) const {
    if (nStones == 0) {
        moves[0] = Move(Board::BoardSize / 2, Board::BoardSize / 2);
        return (1);
    }
    
    int count = 0;
    for (int x = 0; x < Board::BoardSize; ++x) {
        for (int y = 0; y < Board::BoardSize; ++y) {
            if (sGrid[x][y] == Stone::NONE && sNear[x][y] > 0) {
                moves[count++] = Move{static_cast<Board::size_t>(x), static_cast<Board::size_t>(y)};
            }
        }
    }

    return (count);
}
