#pragma once

#include <string>
#include <map>

#include <SDL2/SDL_Rect.h>

#include "color.hpp"

struct TextLabel;

struct Button {
    std::string text;
    SDL_Rect rectangle;
    Color color;
    bool visible = true;
    bool selected = false;
    TextLabel textLabel() const;
    TextLabel textLabelSelected() const;
};

template<typename Key>
using SingleSelectButtons = std::map<Key, Button>;

template<typename Key>
bool handleMouseDown(SDL_Point mouse, SingleSelectButtons<Key>& buttons) {
    for (auto& item : buttons) {
        if (SDL_PointInRect(&mouse, &item.second.rectangle)) {
            for (auto& item : buttons) {
                item.second.selected = false;
            }
            item.second.selected = true;
            return true;
        }
    }
    return false;
}

template<typename Key>
Key getSelectedButton(const SingleSelectButtons<Key>& buttons) {
    for (auto& item : buttons) {
        if (item.second.selected) {
            return item.first;
        }
    }
    return Key{};
}
