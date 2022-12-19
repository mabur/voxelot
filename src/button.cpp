#include "button.hpp"

#include <algorithm>

Uint8 addUint8(Uint8 a, Uint8 b) {
    return static_cast<Uint8>(
        std::min(static_cast<int>(a) + static_cast<int>(b), 255)
    );
}

Uint8 subUint8(Uint8 a, Uint8 b) {
    return static_cast<Uint8>(
        std::max(static_cast<int>(a) - static_cast<int>(b), 0)
    );
}

void drawButton(const Button& button, SDL_Renderer* renderer) {
    if (button.visible) {
        if (button.selected) {
            SDL_SetRenderDrawColor(renderer, button.r, button.g, button.b, 1);
            SDL_RenderFillRect(renderer, &button.rectangle);
        }
        else {
            SDL_SetRenderDrawColor(renderer, button.r, button.g, button.b, 1);
            SDL_RenderFillRect(renderer, &button.rectangle);
            // Top line & left line:
            SDL_SetRenderDrawColor(
                renderer,
                addUint8(button.r, 128),
                addUint8(button.g, 128),
                addUint8(button.b, 128),
                1
            );
            SDL_RenderDrawLine(
                renderer,
                button.rectangle.x,
                button.rectangle.y,
                button.rectangle.x + button.rectangle.w - 1,
                button.rectangle.y
            );
            SDL_RenderDrawLine(
                renderer,
                button.rectangle.x,
                button.rectangle.y,
                button.rectangle.x,
                button.rectangle.y + button.rectangle.h - 1
            );
            // Bottom line & right line:
            SDL_SetRenderDrawColor(
                renderer,
                subUint8(button.r, 128),
                subUint8(button.g, 128),
                subUint8(button.b, 128),
                1
            );
            SDL_RenderDrawLine(
                renderer,
                button.rectangle.x,
                button.rectangle.y + button.rectangle.h - 1,
                button.rectangle.x + button.rectangle.w - 1,
                button.rectangle.y + button.rectangle.h - 1
            );
            SDL_RenderDrawLine(
                renderer,
                button.rectangle.x + button.rectangle.w - 1,
                button.rectangle.y,
                button.rectangle.x + button.rectangle.w - 1,
                button.rectangle.y + button.rectangle.h - 1
            );
        }
    }
}
