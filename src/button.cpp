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
        const auto xmin = button.rectangle.x;
        const auto ymin = button.rectangle.y;
        const auto xmax = button.rectangle.x + button.rectangle.w - 1;
        const auto ymax = button.rectangle.y + button.rectangle.h - 1;

        const auto r = button.r;
        const auto g = button.g;
        const auto b = button.b;

        const auto r_bright = addUint8(r, 64);
        const auto g_bright = addUint8(g, 64);
        const auto b_bright = addUint8(b, 64);

        const auto r_dark = subUint8(r, 64);
        const auto g_dark = subUint8(g, 64);
        const auto b_dark = subUint8(b, 64);

        if (button.selected) {
            SDL_SetRenderDrawColor(renderer, r, g, b, 1);
            SDL_RenderFillRect(renderer, &button.rectangle);
        }
        else {
            SDL_SetRenderDrawColor(renderer, r, g, b, 1);
            SDL_RenderFillRect(renderer, &button.rectangle);
            // Top line & left line:
            SDL_SetRenderDrawColor(renderer, r_bright, g_bright, b_bright, 1);
            SDL_RenderDrawLine(renderer, xmin, ymin, xmax, ymin);
            SDL_RenderDrawLine(renderer, xmin, ymin, xmin, ymax);
            // Bottom line & right line:
            SDL_SetRenderDrawColor(renderer, r_dark, g_dark, b_dark, 1);
            SDL_RenderDrawLine(renderer, xmin, ymax, xmax, ymax);
            SDL_RenderDrawLine(renderer, xmax, ymin, xmax, ymax);
        }
    }
}
