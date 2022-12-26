#pragma once

#include "pixels.hpp"

struct Button;
struct TextLabel;

void drawTextLabel(Pixels& pixels, const TextLabel& text_label);
void drawButton(Pixels& pixels, const Button& button);
