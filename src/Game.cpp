#include "Game.hpp"

#include <chrono>

Stone	Game::_startGame() {
	Board		board(_settings);
	PlayerID	turn = PlayerID::P1;
	PlayerID	lastPlayer = PlayerID::P1;
	double	lastMoveTime = 0.0;

	while (!board.done()) {
		_render.renderGame(board, lastPlayer, lastMoveTime,
			board.getCapture(PlayerID::P1), board.getCapture(PlayerID::P2));

		Move move;
		const auto moveStart = std::chrono::steady_clock::now();
		if (_players[toInt(turn)] == PlayerType::AI) {
			// get IA move
			// move = _getIAMove
		} else {
			move = _render.detectPlayerMove(board, turn, lastPlayer, lastMoveTime,
				board.getCapture(PlayerID::P1), board.getCapture(PlayerID::P2));
		}
		if (move.x == static_cast<uint16_t>(-1) && move.y == static_cast<uint16_t>(-1)) {
			//handle invalid move if there is ones
			continue; // temporary, just to test the loop
		}
		board.playStone(move, turn);
		const auto moveEnd = std::chrono::steady_clock::now();
		lastMoveTime = std::chrono::duration<double>(moveEnd - moveStart).count();
		lastPlayer = turn;

		if (turn == PlayerID::P1)	turn = PlayerID::P2;
		else						turn = PlayerID::P1;
	}
	_render.renderGame(board, lastPlayer, lastMoveTime,
		board.getCapture(PlayerID::P1), board.getCapture(PlayerID::P2));

	return (board.getWinner());
}

// True on exit
bool	Game::_menu() {
	SDL_Event	event;

	while (true) {
		_render.renderMenu(_players[0], _players[1]);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return (true);
			if (event.type == SDL_MOUSEBUTTONDOWN
				&& event.button.button == SDL_BUTTON_LEFT
				&& _render.isStartButton(event.button.x, event.button.y))
				return (false);
			if (event.type == SDL_MOUSEBUTTONDOWN
				&& event.button.button == SDL_BUTTON_LEFT
				&& _render.isExitButton(event.button.x, event.button.y))
				return (true);
			if (event.type == SDL_MOUSEBUTTONDOWN
				&& event.button.button == SDL_BUTTON_LEFT
				&& _render.isPlayerButton(PlayerID::P1,
					event.button.x, event.button.y))
				_players[0] = _players[0] == PlayerType::AI
					? PlayerType::HUMAN : PlayerType::AI;
			if (event.type == SDL_MOUSEBUTTONDOWN
				&& event.button.button == SDL_BUTTON_LEFT
				&& _render.isPlayerButton(PlayerID::P2,
					event.button.x, event.button.y))
				_players[1] = _players[1] == PlayerType::AI
					? PlayerType::HUMAN : PlayerType::AI;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				return (true);
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
