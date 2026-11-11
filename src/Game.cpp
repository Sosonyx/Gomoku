#include "Game.hpp"

Board::Stone	Game::_startGame() {
	Board	board(_settings);
	
	PlayerType	players[2] = {PlayerType::AI, PlayerType::HUMAN};	// From settings, first to p[layer] 
	PlayerID	turn = PlayerID::P1;

	// if settings->gamemode != normal
	//		do the choices and shits
	while (!board.done()) {
		Move move;
		if (players[turn] == PlayerType::AI) {
			// get IA move
		} else {

		}
		// Play move on board
		// Render move 
		if (turn == PlayerID::P1)	turn = PlayerID::P2;
		else						turn = PlayerID::P1;
	}
	return (board.getWinner());
}

// True on exit
bool	Game::_menu() {
	// Get event
	// handle event
	return (true);
}

void	Game::_routine() {
	while (1){
		bool	exitMenu = _menu();
		
		if (exitMenu == true)
			break ;
		
		Board::Stone winner = _startGame();
		_renderScore(winner);
	}
}

void Game::_renderScore(Board::Stone winner) {
	
}

uint8_t	Game::playGame() {
	try {
		Render	render;
	
		Game	game(render);

		game._routine();
	} catch (const std::exception &e) {
		std::cerr << e.what() <<std::endl;
		return (1);
	}
	return (0);
}
