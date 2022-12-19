#pragma once

#include <map>

#include "color.hpp"
#include "sdl_wrappers.hpp"

struct Button {
    SDL_Rect rectangle;
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    bool visible = true;
    bool selected = false;
};

void drawButton(const Button& button, SDL_Renderer* renderer);

template<typename EnumType>
using SingleSelectButtons = std::map<EnumType, Button>;

template<typename EnumType>
bool handleMouseDown(SDL_Point mouse, SingleSelectButtons<EnumType>& buttons) {
    for (auto& item : buttons) {
        const auto rectangle = item.second.rectangle();
        if (SDL_PointInRect(&mouse, &rectangle)) {
            for (auto& item : buttons) {
                item.second.selected = false;
            }
            item.second.selected = true;
            return true;
        }
    }
    return false;
}
