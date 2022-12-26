#pragma once

#include <string>
#include <valarray>
#include <vector>

#include "array2.hpp"
#include "color.hpp"
#include "pixels.hpp"

struct World;

void drawString(
    Pixels& pixels, const std::string& s, size_t x, size_t y, PackedColor color
);

void draw(Pixels& pixels, const World& world);
