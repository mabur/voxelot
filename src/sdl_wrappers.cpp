#include "sdl_wrappers.hpp"

#include <cassert>
#include <iostream>

#include <SDL2/SDL.h>

bool Input::isLeftMouseButtonDown() const
{
    // TODO: does this work for relative mouse state?
    return static_cast<bool>(mouse & SDL_BUTTON(SDL_BUTTON_LEFT));
}

bool Input::isRightMouseButtonDown() const
{
    return static_cast<bool>(mouse & SDL_BUTTON(SDL_BUTTON_RIGHT));
}

SDL_Point Input::mousePosition() const {
    return SDL_Point{mouse_x, mouse_y};
}

void printError(const char* context)
{
    std::cout << "Error in " << context << ": " << SDL_GetError() << std::endl;
}

bool Sdl::noQuitMessage()
{
    auto e = SDL_Event();
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return false;
    }
    return true;
}

Sdl::Sdl(const char* window_title, int width, int height)
    : width(width)
    , height(height)
    , window(nullptr)
    , renderer(nullptr)
    , texture(nullptr)
    , left_mouse_button(ButtonState::UP)
    , escape_button(ButtonState::UP)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printError("SDL_Init");
        throw;
    }

    window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window)
    {
        printError("SDL_CreateWindow");
        throw;
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        printError("SDL_CreateRenderer");
        throw;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height);

    if (!texture)
    {
        printError("SDL_CreateTexture");
        throw;
    }

    if (SDL_RenderSetLogicalSize(renderer, width, height) != 0) {
        printError("SDL_RenderSetLogicalSize");
        throw;
    }

    setMouseModeRelative();
}

Sdl::~Sdl()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Sdl::draw(const Uint32* pixels)
{
    SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}

void Sdl::setMouseModeRelative() {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetRelativeMouseState(nullptr, nullptr);
}

void Sdl::setMouseModeAbsolute() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

ButtonState updateButtonState(ButtonState old_state, bool is_down) {
    switch (old_state) {
    case ButtonState::UP: return is_down ? ButtonState::CLICKED : ButtonState::UP;
    case ButtonState::CLICKED: return ButtonState::DOWN;
    case ButtonState::DOWN: return is_down ? ButtonState::DOWN : ButtonState::RELEASED;
    case ButtonState::RELEASED: return ButtonState::UP;
    }
    assert(false);
}

Input Sdl::getInput() {
    auto input = Input{};

    int x_window;
    int y_window;
    float x_logical;
    float y_logical;
    SDL_GetMouseState(&x_window, &y_window);
    SDL_RenderWindowToLogical(renderer, x_window, y_window, &x_logical, &y_logical);
    input.mouse_x = static_cast<int>(x_logical);
    input.mouse_y = static_cast<int>(y_logical);

    input.mouse = SDL_GetRelativeMouseState(&input.mouse_dx, &input.mouse_dy);
    input.keyboard = SDL_GetKeyboardState(NULL);
    left_mouse_button = updateButtonState(left_mouse_button, input.isLeftMouseButtonDown());
    escape_button = updateButtonState(escape_button, input.keyboard[SDL_SCANCODE_ESCAPE]);
    input.left_mouse_button = left_mouse_button;
    input.escape_button = escape_button;
    return input;
}
