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

	for (int x = 0; x < BoardSize; ++x)
		for (int y = 0; y < BoardSize; ++y) {
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
            if (x + i >= 0 && x + i < BoardSize && y + j >= 0 && y + j < BoardSize) {
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
            if (x + i >= 0 && x + i < BoardSize && y + j >= 0 && y + j < BoardSize) {
                sNear[x + i][y + j]--;
            }
        }
    }
}

int SearchState::generateMoves(Move moves[]) const {
    if (nStones == 0) {
        moves[0] = Move(BoardSize / 2, BoardSize / 2);
        return (1);
    }
    
    int count = 0;
    for (int x = 0; x < BoardSize; ++x) {
        for (int y = 0; y < BoardSize; ++y) {
            if (sGrid[x][y] == Stone::NONE && sNear[x][y] > 0) {
                moves[count++] = Move{static_cast<uint16_t>(x), static_cast<uint16_t>(y)};
            }
        }
    }

    return (count);
}

bool	SearchState::makesFive(int x, int y) const {
	const Stone	me = sGrid[x][y];
	const int	dirs[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

	for (const auto &d : dirs) {
		int	count = 1;

		for (int k = 1; k < 5; ++k) {
			const int nx = x + d[0] * k;
			const int ny = y + d[1] * k;
			if (nx < 0 || nx >= BoardSize || ny < 0 || ny >= BoardSize
				|| sGrid[nx][ny] != me)
				break ;
			++count;
		}
		for (int k = 1; k < 5; ++k) {
			const int nx = x - d[0] * k;
			const int ny = y - d[1] * k;
			if (nx < 0 || nx >= BoardSize || ny < 0 || ny >= BoardSize
				|| sGrid[nx][ny] != me)
				break ;
			++count;
		}
		if (count >= 5)
			return (true);
	}
	return (false);
}