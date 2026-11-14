#include "Render.hpp"

#include <iostream>

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

    this->_window = SDL_CreateWindow("Gomoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
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
}


void Render::renderStone(int x, int y)
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }
    SDL_Rect stoneRect = { x * 30, y * 30, 30, 30 };
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(this->_renderer, &stoneRect);
}

void Render::renderStones(Board &board)
{
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            Stone stone = board.getStoneAt(x, y);
            if (stone != Stone::NONE) {
                renderStone(x, y);
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

    SDL_Rect startButton = {300, 250, 200, 80};
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(this->_renderer, &startButton);

    SDL_RenderPresent(this->_renderer);
}

bool Render::isStartButton(int x, int y) const
{
    return (x >= 300 && x < 500 && y >= 250 && y < 330);
}

void Render::detectPlayerMove(Board &board, Stone player)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            uint16_t x = event.button.x / 30;
            uint16_t y = event.button.y / 30;
            Move move{x, y};
            if (board.isMoveLegal(move, player)) {
                board.playStone(move, player);
            }
        }
    }
}

void Render::renderGame(Board &board)
{
    if (!this->_renderer || !this->_window) {
        std::cerr << "Renderer or window not initialized." << std::endl;
        return;
    }

    renderBackground();
    renderStones(board);

    SDL_RenderPresent(this->_renderer);
}
