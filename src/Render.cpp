#include "Render.hpp"

#include <iostream>
#include "Board.hpp"

namespace
{
	constexpr int WindowWidth = 1400;
	constexpr int WindowHeight = 1000;


	constexpr int GridSize = BoardSize + 2;
	constexpr int GridVerticalMargin = 30;
	constexpr int GridHorizontalMargin = 60;
	constexpr int GridHeight = WindowHeight - 2 * GridVerticalMargin;
	constexpr int GridWidth = WindowWidth - 2 * GridHorizontalMargin;

	constexpr int GridSpacing = (GridWidth < GridHeight ? 
                                    GridWidth /(GridSize - 1)
                                :   GridHeight / (GridSize - 1));
	constexpr int GridLineLen = GridWidth < GridHeight ? GridWidth : GridHeight;

    constexpr int StoneSize = GridSpacing * 0.8f; // Size of the stone based on the board size
}

Render::Render() : _renderer(nullptr), _window(nullptr), _texture(nullptr), _surface(nullptr)
{}

Render::~Render()
{
    if (this->_renderer)
        SDL_DestroyRenderer(this->_renderer);
    if (this->_window)
        SDL_DestroyWindow(this->_window);
    if (this->_texture)
        SDL_DestroyTexture(this->_texture);
    if (this->_surface)
        SDL_FreeSurface(this->_surface);
}

// ---------------- Initialization ----------------
void Render::initRender()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL not initialized - SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    this->_window = SDL_CreateWindow("Gomoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
    if (!this->_window) {
        std::cerr << "Window could not be created - SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
    if (!this->_renderer) {
        std::cerr << "Renderer could not be created -SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

// ---------------- Rendering ----------------
void Render::renderBackground()
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 255); // White background
    SDL_RenderClear(this->_renderer);
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255); // Black for grid lines
    SDL_RenderClear(this->_renderer);
}

void Render::renderGrid()
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 255); // White background
    SDL_RenderClear(this->_renderer);

    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255); // Black grid lines
    for (int i = 0; i < GridSize; ++i) {
        const int coordinateX = GridHorizontalMargin + i * GridSpacing;
        const int coordinateY = GridVerticalMargin + i * GridSpacing;
        SDL_RenderDrawLine(this->_renderer, coordinateX, GridVerticalMargin,
            coordinateX, GridVerticalMargin + GridLineLen);
        SDL_RenderDrawLine(this->_renderer, GridHorizontalMargin, coordinateY,
            GridHorizontalMargin + GridLineLen, coordinateY);
    }
}

void Render::renderStone(int x, int y, Stone stone, bool preview)
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }
    SDL_Rect stoneRect = {
        GridHorizontalMargin + (x + 1) * GridSpacing - StoneSize / 2,
        GridVerticalMargin + (y + 1) * GridSpacing - StoneSize / 2,
        StoneSize,
        StoneSize
    };

    if (preview) {
        if (stone == Stone::P1)
            SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 100); // Red for P1
        else if (stone == Stone::P2)
            SDL_SetRenderDrawColor(this->_renderer, 0, 0, 255, 100); // Blue for P2
    } 
    else {
        if (stone == Stone::P1)
            SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255); // Red for P1
        else if (stone == Stone::P2)
            SDL_SetRenderDrawColor(this->_renderer, 0, 0, 255, 255); // Blue for P2
    }
    SDL_RenderFillRect(this->_renderer, &stoneRect);
}

void Render::renderStones(Board &board)
{
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            Stone stone = board.getStoneAt(x, y);
            if (stone != Stone::NONE) {
                renderStone(x, y, stone);
            }
        }
    }
}

void Render::renderMenu()
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->_renderer);

    SDL_Rect startButton = {300, 250, 200, 80}; // x, y, width, height
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(this->_renderer, &startButton);
    
    SDL_Rect exitButton = {300, 350, 200, 80}; // x, y, width, height
    SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(this->_renderer, &exitButton);

    SDL_RenderPresent(this->_renderer);
}

bool Render::isStartButton(int x, int y) const
{
    return (x >= 300 && x <= 500 && y >= 250 && y <= 330);
}

Move Render::pixelToGrid(int x, int y) const
{
    uint16_t gridX = (x - GridHorizontalMargin + GridSpacing / 2)
        / GridSpacing - 1;
    uint16_t gridY = (y - GridVerticalMargin + GridSpacing / 2)
        / GridSpacing - 1;

    return Move{(gridX),(gridY)};
}

Move Render::detectPlayerMove(Board &board, PlayerID player)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        // if (event.type == SDL_MOUSEMOTION) {
        //     Move move = pixelToGrid(event.motion.x, event.motion.y);

        //     if (move.x != static_cast<uint16_t>(-1) && board.isMoveLegal(move, player)) {
        //         renderStone(move.x, move.y, player == PlayerID::P1 ? Stone::P1 : Stone::P2, true);
        //         SDL_RenderPresent(this->_renderer);
        //     }
        // }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            Move move = pixelToGrid(event.button.x, event.button.y);
            if (move.x != static_cast<uint16_t>(-1)
                && board.isMoveLegal(move, player)) {
                return move;
            }
        }
    }
    return Move{static_cast<uint16_t>(-1), static_cast<uint16_t>(-1)}; // Invalid move
}

void Render::renderGame(Board &board)
{
    if (!this->_renderer || !this->_window) {
        std::cerr << "Renderer or window not initialized." << std::endl;
        return;
    }

    renderBackground();
    renderGrid();
    renderStones(board);

    SDL_RenderPresent(this->_renderer);
}
