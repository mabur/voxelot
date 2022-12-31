#pragma once

#include "pixels.hpp"

struct Button;
struct TextLabel;

void drawTextLabel(Pixels& pixels, const TextLabel& text_label);
void drawButton(Pixels& pixels, const Button& button);

template<typename Buttons>
void drawButtons(Pixels& pixels, const Buttons& buttons) {
    for (auto item : buttons) {
        drawButton(pixels, item.second);
    }
}
