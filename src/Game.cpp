#include "Game.hpp"
#include "LineView.hpp"

Stone	Game::_startGame() {
	Board		board(_settings);
	PlayerType	players[2] = {PlayerType::HUMAN, PlayerType::HUMAN};	// From settings, first to p[layer] 
	PlayerID	turn = PlayerID::P1;

	while (!board.done()) {
		_render.renderGame(board); // temporary there jsp si ca reste ici

		Move move;
		if (players[toInt(turn)] == PlayerType::AI) {
			// get IA move
			// move = _getIAMove
		} else {
			move =_render.detectPlayerMove(board, turn); // temporaryyy
		}
		if (move.x == static_cast<uint16_t>(-1) && move.y == static_cast<uint16_t>(-1)) {
			//handle invalid move if there is ones
			continue; // temporary, just to test the loop
		}
		board.playStone(move, turn);

		LineView	LV(board, LineView::Direction::MINUS_DIAG, move);
		LV.debug();

		if (turn == PlayerID::P1)	turn = PlayerID::P2;
		else						turn = PlayerID::P1;
	}
	// _render.renderGame(board); // temporary

	return (board.getWinner());
}

// True on exit
bool	Game::_menu() {
	SDL_Event	event;

	while (true) { // temporary, just to test the menu
		_render.renderMenu();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return (true);
			if (event.type == SDL_MOUSEBUTTONDOWN
				&& event.button.button == SDL_BUTTON_LEFT
				&& _render.isStartButton(event.button.x, event.button.y))
				return (false);
		}
		SDL_Delay(20); // delay to prevent high CPU usage from polling
	}
}

void	Game::_routine() {
	while (1){
		bool	exitMenu = _menu();
		
		if (exitMenu == true)
			break ;
		
		Stone winner = _startGame();
		_renderScore(winner);
	}
}



void Game::_renderScore(Stone winner) {
	(void)winner;
}

Game::Game(Render &render)
	:	_render(render)
{}

Game::~Game() {}

uint8_t	Game::playGame() {
	try {
		Render	render;
		render.initRender();
		Game	game(render);

		game._routine();
	} catch (const std::exception &e) {
		std::cerr << e.what() <<std::endl;
		return (1);
	}
	return (0);
}
