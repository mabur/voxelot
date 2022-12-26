#pragma once

#include <string>

#include "color.hpp"
#include "pixels.hpp"

struct Button;

void drawString(
    Pixels& pixels, const std::string& s, size_t x, size_t y, PackedColor color
);

void drawButton(const Button& button, Pixels& pixels);
