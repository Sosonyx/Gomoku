#include "Render.hpp"
#include "Board.hpp"

#include <cstdio>
#include <iostream>
#include <SDL2/SDL2_gfxPrimitives.h>

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

Render::Render()
    : _renderer(nullptr), _window(nullptr), _texture(nullptr), _surface(nullptr),
      _font(nullptr), _hoveredMove(static_cast<uint16_t>(-1),
          static_cast<uint16_t>(-1)), _hoveredMoveLegal(false),
      _hoveredStone(Stone::NONE)
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
    if (this->_font)
        TTF_CloseFont(this->_font);
    TTF_Quit();
}

// ---------------- Initialization ----------------
void Render::initRender()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL not initialized - SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf not initialized - TTF_Error: "
                  << TTF_GetError() << std::endl;
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

    this->_font = TTF_OpenFont(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 32);
    if (!this->_font) {
        this->_font = TTF_OpenFont(
            "/usr/share/fonts/google-noto-vf/NotoSans[wght].ttf", 32);
    }
    if (!this->_font) {
        std::cerr << "Font could not be loaded - TTF_Error: "<< TTF_GetError() << std::endl;
    }
}

// ---------------- Rendering ----------------
void Render::renderBackground()
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(this->_renderer, 102, 51, 0, 255);
    SDL_RenderClear(this->_renderer);
}

void Render::renderGrid()
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }
    SDL_Rect boardSurface = {
        GridHorizontalMargin,
        GridVerticalMargin,
        GridLineLen + 1,
        GridLineLen + 1
    };
    SDL_SetRenderDrawColor(this->_renderer, 194, 137, 75, 255);
    SDL_RenderFillRect(this->_renderer, &boardSurface);

    SDL_SetRenderDrawColor(this->_renderer, 76, 45, 24, 255);
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

    const int centerX = GridHorizontalMargin + (x + 1) * GridSpacing;
    const int centerY = GridVerticalMargin + (y + 1) * GridSpacing;
    const int radius = StoneSize / 2;

    Uint8 red = 235;
    Uint8 green = 232;
    Uint8 blue = 220;
    Uint8 alpha = preview ? 100 : 255;

    if (stone == Stone::P2) {
        red = 12;
        green = 12;
        blue = 14;
    }

    filledCircleRGBA(
        this->_renderer,
        centerX,
        centerY,
        radius,
        red,
        green,
        blue,
        alpha
    );

    if (stone == Stone::P2) {
        filledCircleRGBA(
            this->_renderer,
            centerX - radius / 3,
            centerY - radius / 3,
            radius / 4,
            255,
            255,
            255,
            preview ? 80 : 180
        );
    }
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

void Render::renderText(const char *text, int x, int y, int size, bool light)
{
    if (!this->_font)
        return;
    if (TTF_SetFontSize(this->_font, size) < 0) {
        std::cerr << "Font size could not be changed - TTF_Error: "
                  << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color color = light
        ? SDL_Color{255, 255, 255, 255}
        : SDL_Color{70, 45, 25, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(this->_font, text, color);
    if (!surface) {
        std::cerr << "Text could not be rendered - TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
    if (!texture) {
        std::cerr << "Text texture could not be created - SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect destination = {
        x - surface->w / 2,
        y - surface->h / 2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(this->_renderer, texture, nullptr, &destination);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Render::renderMenu(PlayerType player1, PlayerType player2)
{
    if (!this->_renderer) {
        std::cerr << "Renderer not initialized" << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(this->_renderer, 102, 51, 0, 255);
    SDL_RenderClear(this->_renderer);

    renderText("GOMOKU", 700, 110, 72, true);
    renderText("GAME", 350, 220, 32, true);
    renderText("SETTINGS", 1050, 220, 32, true);

    roundedBoxRGBA(this->_renderer, 150, 280, 550, 400, 20,
        225, 205, 180, 255);
    roundedBoxRGBA(this->_renderer, 150, 450, 550, 570, 20,
        225, 205, 200, 255);
    renderText("PLAY", 350, 340, 36);
    renderText("EXIT", 350, 510, 36);

    roundedBoxRGBA(this->_renderer, 800, 280, 1300, 440, 20,
        225, 205, 180, 255);
    roundedBoxRGBA(this->_renderer, 800, 480, 1300, 640, 20,
        205, 185, 155, 255);
    renderText("PLAYER 1", 930, 330, 30);
    renderText(player1 == PlayerType::AI ? "AI" : "HUMAN", 1150, 390, 36);
    renderText("PLAYER 2", 930, 530, 30);
    renderText(player2 == PlayerType::AI ? "AI" : "HUMAN", 1150, 590, 36);

    SDL_RenderPresent(this->_renderer);
}

bool Render::isStartButton(int x, int y) const
{
    return (x >= 150 && x <= 550 && y >= 280 && y <= 400);
}

bool Render::isExitButton(int x, int y) const
{
    return (x >= 150 && x <= 550 && y >= 450 && y <= 570);
}

bool Render::isPlayerButton(PlayerID player, int x, int y) const
{
    if (player == PlayerID::P1)
        return (x >= 800 && x <= 1300 && y >= 280 && y <= 440);
    return (x >= 800 && x <= 1300 && y >= 480 && y <= 640);
}

Move Render::pixelToGrid(int x, int y) const
{
    const int gridX = (x - GridHorizontalMargin + GridSpacing / 2)
        / GridSpacing - 1;
    const int gridY = (y - GridVerticalMargin + GridSpacing / 2)
        / GridSpacing - 1;

    if (gridX < 0 || gridX >= BoardSize || gridY < 0
        || gridY >= BoardSize)
        return Move(static_cast<uint16_t>(-1), static_cast<uint16_t>(-1));
    return Move(static_cast<uint16_t>(gridX), static_cast<uint16_t>(gridY));
}

Move Render::detectPlayerMove(Board &board, PlayerID player,
    PlayerID lastPlayer, double moveTime,
    uint16_t player1Captures, uint16_t player2Captures)
{
    SDL_Event event;
    _hoveredMove = Move(static_cast<uint16_t>(-1), static_cast<uint16_t>(-1));
    _hoveredMoveLegal = false;
    _hoveredStone = toStone(player);

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit(0);

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                exit(0);

            if (event.type == SDL_MOUSEMOTION) {
                Move move = pixelToGrid(event.motion.x, event.motion.y);
                if (move.x == _hoveredMove.x && move.y == _hoveredMove.y)
                    continue;

                _hoveredMove = move;
                _hoveredMoveLegal = move.x != static_cast<uint16_t>(-1)
                    && move.y != static_cast<uint16_t>(-1)
                    && board.isMoveLegal(move, player);
                renderGame(board, lastPlayer, moveTime,
                    player1Captures, player2Captures);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN
                && event.button.button == SDL_BUTTON_LEFT) {
                Move move = pixelToGrid(event.button.x, event.button.y);
                if (move.x == _hoveredMove.x && move.y == _hoveredMove.y) {
                    if (_hoveredMoveLegal)
                        return move;
                } else if (move.x != static_cast<uint16_t>(-1)
                    && move.y != static_cast<uint16_t>(-1)
                    && board.isMoveLegal(move, player)) {
                    return move;
                }
            }
        }
        SDL_Delay(10);
    }
}

void Render::renderGame(Board &board, PlayerID player, double moveTime,
    uint16_t player1Captures, uint16_t player2Captures)
{
    if (!this->_renderer || !this->_window) {
        std::cerr << "Renderer or window not initialized." << std::endl;
        return;
    }

    renderBackground();
    renderGrid();
    renderStones(board);

    if (_hoveredMoveLegal)
        renderStone(_hoveredMove.x, _hoveredMove.y, _hoveredStone, true);

    roundedBoxRGBA(this->_renderer, 1050, 700, 1350, 860, 20,
        225, 205, 180, 255);
    renderText("LAST MOVE", 1200, 745, 26);

    char playerText[32];
    std::snprintf(playerText, sizeof(playerText), "PLAYER %d",
        toInt(player) + 1);
    renderText(playerText, 1200, 790, 26);

    char timeText[32];
    std::snprintf(timeText, sizeof(timeText), "%.2f s", moveTime);
    renderText(timeText, 1200, 830, 34);

    roundedBoxRGBA(this->_renderer, 1050, 520, 1350, 680, 20,
        205, 185, 155, 255);
    char capturesText[64];
    std::snprintf(capturesText, sizeof(capturesText), "P1: %u   P2: %u",
        player1Captures, player2Captures);
    renderText("CAPTURES", 1200, 565, 26);
    renderText(capturesText, 1200, 625, 30);

    SDL_RenderPresent(this->_renderer);
}
