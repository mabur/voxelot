#pragma once

#include <string>
#include <valarray>
#include <vector>

#include "color.hpp"
#include "array2.hpp"

struct World;

using Pixels = Array2<Color>;

void drawString(
    Pixels& pixels, const std::string& s, size_t x, size_t y, Color color
);

void draw(Pixels& pixels, const World& world);
